#include "autosearchdialog.h"
#include "ui_autosearchdialog.h"
#include "logger.h"
#include <QHostAddress>
#include <QThread>
#include <QRegExp>

RemoteDevice::RemoteDevice()
{
    port = 0;
    socket = NULL;
}

void RemoteDevice::Connect(QString ip, int port)
{
    this->port = port;
    this->ip = ip;
    delete socket;
    socket = new QTcpSocket();
    connect((socket), SIGNAL(connected()), this, SLOT(_TcpConnected()));
    connect((socket), SIGNAL(disconnected()), this, SLOT(_TcpDisconnected()));
    connect((socket), SIGNAL(readyRead()), this, SLOT(_DataAvailable()));
    //connect((&m_Socket), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SocketStateChanged(QAbstractSocket::SocketState)));
    connect((socket), SIGNAL(error(QAbstractSocket::SocketError)), this,  SLOT(_TcpError(QAbstractSocket::SocketError)));
    socket->connectToHost(ip, port);
}

RemoteDevice::~RemoteDevice()
{
    delete socket;
    socket = NULL;
}


AutoSearchDialog::AutoSearchDialog(QWidget *parent) :
    QDialog(parent),
    m_Result(0),
    m_SelectedIndex(-1),
    m_HUpnpWrapper(NULL),
    m_lib(this),
    ui(new Ui::AutoSearchDialog)
{
    ui->setupUi(this);

    ui->label->setVisible(false);
    ui->timeLabel->setVisible(false);

    QString path = qApp->applicationDirPath();
    m_lib.setFileName(path + "/HUpnpWrapper");
    if (!m_lib.load())
    {
        Logger::Log("Could not load " + path + "/HUpnpWrapper");
        Logger::Log(m_lib.errorString());
        return;
    }
    Q_ASSERT(m_lib.isLoaded());
    typedef HUpnpWrapper* (*_GetInstance) ();
    _GetInstance getInstance = (_GetInstance)m_lib.resolve("GetInstance");
    if (getInstance == NULL)
    {
        Logger::Log("Could not get C function GetInstance from " + path + "/HUpnpWrapper");
        Logger::Log(m_lib.errorString());
        return;
    }
    m_HUpnpWrapper = getInstance();
    if(m_HUpnpWrapper == NULL)
    {
        Logger::Log("Could not get the instance of HUpnpWrapper");
        return;
    }
    connect(m_HUpnpWrapper,SIGNAL(NewDevice(QString,QString)), this, SLOT(NewDevice(QString,QString)));
    //connect(this,SIGNAL(UpdateLabel(QString)), this, SLOT(SetLabel(QString)));
    if(!m_HUpnpWrapper->Init())
    {
        Logger::Log("Could not initialize HUpnpWrapper");
        FreeLib();
        return;
    }
}

AutoSearchDialog::~AutoSearchDialog()
{
    FreeLib();
    foreach (RemoteDevice* tmp, m_RemoteDevices) {
        delete tmp;
    }
    m_RemoteDevices.clear();
    foreach (RemoteDevice* tmp, m_DeviceInList) {
        delete tmp;
    }
    m_DeviceInList.clear();
    delete ui;
}

void AutoSearchDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AutoSearchDialog::FreeLib()
{
    if (m_HUpnpWrapper != NULL)
    {
        typedef void (*_FreeInstance) (HUpnpWrapper*);
        _FreeInstance freeInstance = (_FreeInstance)m_lib.resolve("FreeInstance");
        if (freeInstance == NULL)
        {
            Logger::Log("Could not get C function FreeInstance from HUpnpWrapper");
        }
        else
        {
            freeInstance(m_HUpnpWrapper);
            m_HUpnpWrapper = NULL;
        }
    }
    if (m_lib.isLoaded())
        m_lib.unload();
}

void AutoSearchDialog::NewDevice(QString name, QString url)
{

    //emit UpdateLabel("Device " + name + " " + url);
    QString tmp = url;
    if (tmp.startsWith("http://"))
        tmp = tmp.mid(7);
    int n = tmp.indexOf(":");
    if (n != -1)
        tmp = tmp.mid(0, n);

    Logger::Log(name + " " + url + " " + tmp);
    RemoteDevice* device = new RemoteDevice();
    connect((device), SIGNAL(TcpConnected()), this, SLOT(TcpConnected()));
    connect((device), SIGNAL(TcpDisconnected()), this, SLOT(TcpDisconnected()));
    connect((device), SIGNAL(DataAvailable()), this, SLOT(ReadString()));
    connect((device), SIGNAL(TcpError(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));
    device->Connect(tmp, 23);
    m_RemoteDevices.append(device);
}

void AutoSearchDialog::TcpConnected()
{
    Logger::Log("TcpConnected");
    QObject* sender = QObject::sender();
    RemoteDevice* device = dynamic_cast<RemoteDevice*>(sender);
    device->socket->write("?RGD\r\n");
}

void AutoSearchDialog::TcpDisconnected()
{
    Logger::Log("TcpDisconnected");
//    QObject* sender = QObject::sender();
//    RemoteDevice* device = dynamic_cast<RemoteDevice*>(sender);
//    int port = device->port;
//    QString ip = device->ip;
//    int idx = m_RemoteDevices.indexOf(device);
//    if (idx != -1)
//        m_RemoteDevices.remove(idx);
//    delete device;
//    if (port == 23)
//    {
//        device = new RemoteDevice();
//        connect((device), SIGNAL(TcpConnected()), this, SLOT(TcpConnected()));
//        connect((device), SIGNAL(TcpDisconnected()), this, SLOT(TcpDisconnected()));
//        connect((device), SIGNAL(DataAvailable()), this, SLOT(ReadString()));
//        connect((device), SIGNAL(TcpError(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));
//        device->Connect(ip, 8102);
//        m_RemoteDevices.append(device);
//    }
}

void AutoSearchDialog::ReadString()
{
    Logger::Log("ReadString");
    QObject* sender = QObject::sender();
    RemoteDevice* device = dynamic_cast<RemoteDevice*>(sender);
    int count = device->socket->bytesAvailable();
    std::vector<char> data;
    data.resize(count + 1);
    device->socket->read(&data[0], count);
    data[count] = '\0';
    QString str = QString::fromAscii((const char*)&data[0]);
    Logger::Log(str);
    Logger::Log(QString("QHostAddress: %1:%2").arg(device->ip).arg(device->port));

    if (str.startsWith("RGD"))
    {
        QStringList l = str.mid(3).split(QRegExp("[<>]"), QString::SkipEmptyParts);
        QString str1, str2;
        if (l.count() > 0)
            str1 = l[0];
        if (l.count() > 1)
            str2 = l[1];
        if (str2 != "")
        {
            RemoteDevice* rd = new RemoteDevice();
            rd->ip = device->ip;
            rd->port = device->port;
            m_DeviceInList.append(rd);
            ui->listWidget->addItem(QString("%1 (%2:%3)").arg(str2).arg(device->ip).arg(device->port));
            int idx = m_RemoteDevices.indexOf(device);
            if (idx != -1)
                m_RemoteDevices.remove(idx);
            delete device;
            return;
        }
    }

    int port = device->port;
    QString ip = device->ip;
    int idx = m_RemoteDevices.indexOf(device);
    if (idx != -1)
        m_RemoteDevices.remove(idx);
    delete device;
    if (port == 23)
    {
        device = new RemoteDevice();
        connect((device), SIGNAL(TcpConnected()), this, SLOT(TcpConnected()));
        connect((device), SIGNAL(TcpDisconnected()), this, SLOT(TcpDisconnected()));
        connect((device), SIGNAL(DataAvailable()), this, SLOT(ReadString()));
        connect((device), SIGNAL(TcpError(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));
        device->Connect(ip, 8102);
        m_RemoteDevices.append(device);
    }
}

void AutoSearchDialog::TcpError(QAbstractSocket::SocketError socketError)
{
    //             QMessageBox::information(this, tr("Pioneer Remote"),
    //                                      tr("The host was not found. Please check the "
    //                                         "host name and port settings."));
    QObject* sender = QObject::sender();
    RemoteDevice* device = dynamic_cast<RemoteDevice*>(sender);
    //str="ui->"+sender->objectName();
    QString str;
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        str = QString("Host closed connection: %1").arg(device->socket->errorString());
        break;
    case QAbstractSocket::HostNotFoundError:
        str = QString("Host not found: %1").arg(device->socket->errorString());
        //Log(tr("The host was not found. Please check the host name and port settings."), QColor(255, 0, 0));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        str = QString("Host refused connection: %1").arg(device->socket->errorString());
//        Log(tr("The connection was refused by the peer. "
//               "Make sure the receiver is on "
//               "and check ip address and port."), QColor(255, 0, 0));
        break;
    default:
        str = QString("The following error occurred: %1.").arg(device->socket->errorString());
        //Log(tr("The following error occurred: %1.").arg(m_Socket.errorString()), QColor(255, 0, 0));
    }
    Logger::Log(str);
    Logger::Log(QString("Error: QHostAddress: %1:%2").arg(device->ip).arg(device->port));
    int port = device->port;
    QString ip = device->ip;
    int idx = m_RemoteDevices.indexOf(device);
    if (idx != -1)
        m_RemoteDevices.remove(idx);
    delete device;
    if (port == 23)
    {
        Logger::Log("again");
        device = new RemoteDevice();
        connect((device), SIGNAL(TcpConnected()), this, SLOT(TcpConnected()));
        connect((device), SIGNAL(TcpDisconnected()), this, SLOT(TcpDisconnected()));
        connect((device), SIGNAL(DataAvailable()), this, SLOT(ReadString()));
        connect((device), SIGNAL(TcpError(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));
        device->Connect(ip, 8102);
        m_RemoteDevices.append(device);
    }
//    if (m_StartConnection)
//    {
//        Xsleep::msleep(10000);
//        ConnectTCP();
//    }
}

void AutoSearchDialog::on_CancelButton_clicked()
{
    m_Result = 0;
    close();
}

void AutoSearchDialog::on_continueButton_clicked()
{
    m_Result = 1;
    close();
}

void AutoSearchDialog::on_repeatButton_clicked()
{
    m_Result = 2;
    close();
}

void AutoSearchDialog::on_listWidget_clicked(const QModelIndex &index)
{
    m_SelectedIndex = index.row();
}

void AutoSearchDialog::on_listWidget_doubleClicked(const QModelIndex &index)
{
    m_Result = 1;
    m_SelectedIndex = index.row();
    close();
}
