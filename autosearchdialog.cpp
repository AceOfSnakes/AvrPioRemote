#include "autosearchdialog.h"
#include <string>
#include "ui_autosearchdialog.h"
#include "logger.h"
#include <QHostAddress>
#include <QThread>
#include <QClipboard>
#include <QRegExp>
#include <QtNetwork>
#include <QtXml>
#include <QMenu>

string trim(const string &t, const string &ws);

RemoteDevice::RemoteDevice() {
    port = 0;
    socket = NULL;
}

RemoteDevice::RemoteDevice(QString &url):RemoteDevice()
{
    m_url = url;
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
    connect((socket), SIGNAL(error(QAbstractSocket::SocketError)), this,  SLOT(_TcpError(QAbstractSocket::SocketError)));
    socket->connectToHost(ip, port);
}

RemoteDevice::~RemoteDevice()
{
    delete socket;
    socket = NULL;
}

AutoSearchDialog::AutoSearchDialog(QSettings& settings, QWidget *parent, bool receiver,QString pingCommand, QString pingResponseStart
                                   , QString pingResponseStartOff) :
    QDialog(parent),
    m_Result(0),
    m_SelectedPort(0),
    m_GroupAddress("239.255.255.250"),
    m_FindReceivers(receiver),
    m_Settings(settings),
    m_pingCommand(pingCommand),
    m_pingResponseStart(pingResponseStart),
    m_pingResponseStartOff(pingResponseStartOff),
    ui(new Ui::AutoSearchDialog)
{
    qDebug()<<"AutoSearchDialog";
    ui->setupUi(this);

    ui->label->setVisible(false);
    ui->timeLabel->setVisible(false);

    foreach (const QNetworkInterface& iface, QNetworkInterface::allInterfaces())
    {
        if (iface.flags() & QNetworkInterface::IsUp && !(iface.flags() & QNetworkInterface::IsLoopBack))
        {
            QUdpSocket* socket = new QUdpSocket(this);
            if (!socket->bind(QHostAddress::AnyIPv4, 0, QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress))
            {
                qDebug("1: Error %s", socket->errorString().toStdString().c_str());
                delete socket;
                continue;
            }
            if (!socket->joinMulticastGroup(m_GroupAddress))
            {
                qDebug("2: Error %s", socket->errorString().toStdString().c_str());
                delete socket;
                continue;
            }
            socket->setSocketOption(QAbstractSocket::MulticastTtlOption, 5);
            socket->setMulticastInterface(iface);

            connect(socket, SIGNAL(readyRead()),
                    this, SLOT(ProcessPendingDatagrams()));
            m_MulticatsSockets.push_back(socket);
        }
    }
    if(receiver) {
        ui->label_2->setText(ui->label_2->text().replace(QString("%device%"),tr("receiver")));
    } else {
        ui->label_2->setText(ui->label_2->text().replace(QString("%device%"),tr("player")));
    }
    SendMsg();
}

AutoSearchDialog::~AutoSearchDialog()
{
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

void AutoSearchDialog::NewDevice(QString name, QString ip, QString location)
{
    Logger::Log("Found UPnP device: " + name + " " + ip + " " + location);
    QUrl url = QUrl(location);
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req(QUrl(QString(location.toLatin1())));
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    QString manufacturer;
    QString friendlyName;
    QString modelName;
    QString remoteSupported("0");
    QString remotePort(QString::number(url.port()));

    if (reply->error() == QNetworkReply::NoError) {
        //success
        //Get your xml into xmlText(you can use QString instead og QByteArray)
        QString data = reply->readAll();
        QDomDocument document;
        document.setContent(data);
        //QDomElement root = document.firstChildElement();
        QDomNodeList nodes = document.childNodes();
        for (int i = 0; i < nodes.count(); i++) {
            //qDebug() << node.nodeName() << node.nodeValue();
            if (nodes.at(i).nodeName() == "root") {
                QDomNodeList nodes1 = nodes.at(i).childNodes();
                for (int j = 0; j < nodes1.count(); j++) {
                    if (nodes1.at(j).nodeName() == "device") {
                        QDomNodeList nodes2 = nodes1.at(j).childNodes();
                        for (int k = 0; k < nodes2.count(); k++) {
                            QString name = nodes2.at(k).nodeName();
                            QDomNodeList nodes3 = nodes2.at(k).childNodes();
                            QString text;
                            for (int m = 0; m < nodes3.count(); m++) {
                                if (nodes3.at(m).isText())
                                    text += nodes3.at(m).nodeValue();
                            }
                            if (name == "manufacturer" || name == "manufacture")
                            {
                                manufacturer = text;
                                //qDebug() << elementName << manufacturer;
                            }
                            else if (name == "friendlyName")
                            {
                                friendlyName = text;
                                //qDebug() << elementName << friendlyName;
                            }
                            else if (name == "modelName")
                            {
                                modelName = text;
                                //qDebug() << elementName << modelName;
                            } else if(name.endsWith("X_ipRemoteReady")) {
                                remoteSupported = text;
                            } else if(name.endsWith("X_ipRemoteTcpPort")) {
                                remotePort = text;
                            }
                            //qDebug() << name << text;
                        }
                    }
                }
            }
        }
        delete reply;
    }
    else {
        //failure
        Logger::Log("Failure" + reply->errorString());
        delete reply;\
        eventLoop.quit();
        return;
    }
    eventLoop.quit();
    QString deviceKey = modelName.append("/").append("23");
    if(m_RemoteDevices.contains(deviceKey)) {
        Logger::Log("already in remote devices list "+modelName+" "+ip);
    }
    RemoteDevice* device = new RemoteDevice(location);
    connect((device), SIGNAL(TcpConnected()), this, SLOT(TcpConnected()));
    connect((device), SIGNAL(TcpDisconnected()), this, SLOT(TcpDisconnected()));
    connect((device), SIGNAL(DataAvailable()), this, SLOT(ReadString()));
    connect((device), SIGNAL(TcpError(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));
    device->Connect(ip, 23);
    m_RemoteDevices.insert(deviceKey,device);
}

void AutoSearchDialog::TcpConnected()
{
    QObject* sender = QObject::sender();
    RemoteDevice* device = dynamic_cast<RemoteDevice*>(sender);
    qDebug()<<device->ip<<device->port<<QString().append(m_pingCommand).append("\r\n").toLatin1().data();
    device->socket->write(QString().append(m_pingCommand).append("\r\n").toLatin1().data());
}

void AutoSearchDialog::TcpDisconnected()
{
}

QString AutoSearchDialog::removeDevice(QMap<QString,RemoteDevice*>  &m_RemoteDevices,RemoteDevice* device) {
    foreach(QString key,m_RemoteDevices.keys()) {
        if(device == m_RemoteDevices.value(key)) {
            m_RemoteDevices.remove(key);
            int index=key.lastIndexOf("/");
            key.truncate(index);
            return key;
        }
    }
    return QString();
}
void AutoSearchDialog::reconnect(QString & key,QString & ip,int port,RemoteDevice* device){
    if (key != NULL && (port == 23 || port != 8102))
    {
        device = new RemoteDevice();
        connect((device), SIGNAL(TcpConnected()), this, SLOT(TcpConnected()));
        connect((device), SIGNAL(TcpDisconnected()), this, SLOT(TcpDisconnected()));
        connect((device), SIGNAL(DataAvailable()), this, SLOT(ReadString()));
        connect((device), SIGNAL(TcpError(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));
        if(port == 23) {
            device->Connect(ip, 8102);
            m_RemoteDevices.insert(key.append("/8102"), device);
        } else {
            device->Connect(ip, 23);
            m_RemoteDevices.insert(key.append("/23"), device);
        }
    }
}
void AutoSearchDialog::ReadString()
{
    QObject* sender = QObject::sender();
    RemoteDevice* device = dynamic_cast<RemoteDevice*>(sender);
    int count = device->socket->bytesAvailable();
    std::vector<char> data;
    data.resize(count + 1);
    device->socket->read(&data[0], count);
    data[count] = '\0';
    string m_ReceivedString="";
    m_ReceivedString.append((const char*)&data[0], 0, count);
    m_ReceivedString = trim(m_ReceivedString, "\r");
    m_ReceivedString = trim(m_ReceivedString, "\n");
    QString str;
    str = str.fromUtf8(m_ReceivedString.c_str());
    qDebug()<<device->ip<<device->port<<str<<m_pingResponseStart<<m_pingResponseStartOff;
    Logger::Log(str);
    Logger::Log(QString("QHostAddress: %1:%2").arg(device->ip).arg(device->port));

    if (str.startsWith(m_pingResponseStart)||
            (!m_pingResponseStartOff.isEmpty() && str.startsWith(m_pingResponseStartOff)))
    {
        foreach(RemoteDevice *dev ,m_DeviceInList) {
            if(QString::compare(device->ip,dev->ip)==0 && device->port == dev->port) {
                qDebug()<<"already in list"<<device->ip<<device->port;
                return;
            }
        }

        RemoteDevice* rd = new RemoteDevice();
        rd->ip = device->ip;
        rd->port = device->port;
        QString key = removeDevice(m_RemoteDevices,device);
        m_DeviceInList.append(rd);
        ui->listWidget->addItem(QString("%1 (%2:%3)").arg(key).arg(device->ip).arg(device->port));
        if (ui->listWidget->count() == 1) {
            ui->listWidget->setCurrentRow(0);
            m_SelectedAddress = device->ip;
            m_SelectedPort = device->port;
        }
        ui->listWidget->item(ui->listWidget->count() - 1)->setData(Qt::UserRole, device->ip);
        ui->listWidget->item(ui->listWidget->count() - 1)->setData(Qt::UserRole + 1, device->port);


        device->socket->close();
        device->socket->disconnect();
        device->deleteLater();
        return;
    }

    int port = device->port;
    QString ip = device->ip;
    QString key = removeDevice(m_RemoteDevices,device);
    device->deleteLater();
    reconnect(key,ip,port,device);
}

void AutoSearchDialog::TcpError(QAbstractSocket::SocketError socketError)
{
    QObject* sender = QObject::sender();
    RemoteDevice* device = dynamic_cast<RemoteDevice*>(sender);
    QString str;
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        str = QString("Host closed connection: %1").arg(device->socket->errorString());
        break;
    case QAbstractSocket::HostNotFoundError:
        str = QString("Host not found: %1").arg(device->socket->errorString());
        break;
    case QAbstractSocket::ConnectionRefusedError:
        str = QString("Host refused connection: %1").arg(device->socket->errorString());
        break;
    default:
        str = QString("The following error occurred: %1.").arg(device->socket->errorString());
    }
    int port = device->port;
    QString ip = device->ip;
    QString key = removeDevice(m_RemoteDevices,device);
    delete device;
    reconnect(key,ip,port,device);
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
    m_SelectedAddress = ui->listWidget->item(index.row())->data(Qt::UserRole).toString();
    m_SelectedPort = ui->listWidget->item(index.row())->data(Qt::UserRole + 1).toInt();
    QString url = ui->listWidget->item(index.row())->data(Qt::UserRole + 2).toString();
}

void AutoSearchDialog::on_listWidget_doubleClicked(const QModelIndex &index)
{
    m_Result = 1;
    m_SelectedAddress = ui->listWidget->item(index.row())->data(Qt::UserRole).toString();
    m_SelectedPort = ui->listWidget->item(index.row())->data(Qt::UserRole + 1).toInt();
    close();
}

void AutoSearchDialog::closeEvent(QCloseEvent *event)
{
    foreach(QUdpSocket* socket, m_MulticatsSockets)
    {
        socket->leaveMulticastGroup(m_GroupAddress);
        socket->close();
        delete socket;
    }
    m_MulticatsSockets.clear();
    QWidget::closeEvent(event);
}

void AutoSearchDialog::ProcessPendingDatagrams()
{
    foreach( QUdpSocket* socket, m_MulticatsSockets)
    {

        while (socket->hasPendingDatagrams()) {
            QByteArray datagram;
            QHostAddress remoteAddr;
            datagram.resize(socket->pendingDatagramSize());
            socket->readDatagram(datagram.data(), datagram.size(), &remoteAddr);
            QString data = QString(datagram);
            if (data.contains("200 OK", Qt::CaseInsensitive) && data.contains("rootdevice", Qt::CaseInsensitive)) {
                QStringList ll = data.split(QRegExp("[\n\r]"), QString::SkipEmptyParts);
                QString location;
                foreach (QString s, ll)
                {
                    if (s.startsWith("LOCATION: ", Qt::CaseInsensitive))
                    {
                        location = s.mid(10);
                        break;
                    }
                }
                NewDevice("", remoteAddr.toString(), location);
            }
        }
    }
}

void AutoSearchDialog::SendMsg()
{
    QByteArray datagram = "M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\n"
                          "MAN: \"ssdp:discover\"\r\n"
                          "MX: 1\r\n"
                          "ST: upnp:rootdevice\r\n"
                          "USER-AGENT: AVRPioRemote\r\n\r\n\r\n";

    foreach(QUdpSocket* socket, m_MulticatsSockets)
    {
        if (socket->writeDatagram(datagram, m_GroupAddress, 1900) == -1)
        {
            Logger::Log(QString("Error on %1!!!").arg(socket->localAddress().toString()));
        }
    }
}


void AutoSearchDialog::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->listWidget->currentItem()== NULL ) {
        return;
    }
    if(ui->listWidget->currentItem() != ui->listWidget->itemAt(pos)) {
        return;
    }
    QString url = ui->listWidget->itemAt(pos)->data(Qt::UserRole + 2).toString();
    if (!url.isEmpty()) {
        QMenu menu(ui->listWidget);
        QAction *copyAction = menu.addAction(tr("Copy device URL to clipboard"));
        QList<QAction*> act;
        act.append(copyAction);
        QAction *selectedAction = menu.exec(ui->listWidget->mapToGlobal(pos));
        if(selectedAction == copyAction) {
            QApplication::clipboard()->setText(url);
        }
    }
}
