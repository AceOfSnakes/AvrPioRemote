#include "autosearchdialog.h"
#include "ui_autosearchdialog.h"
#include "logger.h"
#include <QHostAddress>
#include <QThread>
#include <QRegExp>
#include <QtNetwork>
#include <QtXml>

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


AutoSearchDialog::AutoSearchDialog(QWidget *parent, bool receiver) :
    QDialog(parent),
    m_Result(0),
    m_SelectedPort(0),
    m_GroupAddress("239.255.255.250"),
    m_FindReceivers(receiver),
    ui(new Ui::AutoSearchDialog)
{
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
    //connect(m_HUpnpWrapper,SIGNAL(NewDevice(QString,QString)), this, SLOT(NewDevice(QString,QString)));
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

    //emit UpdateLabel("Device " + name + " " + url);

    Logger::Log("Found UPnP device: " + name + " " + ip + " " + location);
    qDebug () << ("Found UPnP device: " + name + " " + ip + " " + location);

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
    QString remotePort("8102");
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

        qDebug() << "Success" << manufacturer << friendlyName << modelName;
        delete reply;
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        delete reply;\
        eventLoop.quit();
        return;
    }
    eventLoop.quit();
    if (m_FindReceivers) {
        foreach(RemoteDevice *dev ,m_RemoteDevices) {
            qDebug()<<ip<<dev->ip;
            if(QString::compare(ip,dev->ip)==0) {
                qDebug()<<"already in list"<<ip;
                return;
            }
        }
        RemoteDevice* device = new RemoteDevice();
        connect((device), SIGNAL(TcpConnected()), this, SLOT(TcpConnected()));
        connect((device), SIGNAL(TcpDisconnected()), this, SLOT(TcpDisconnected()));
        connect((device), SIGNAL(DataAvailable()), this, SLOT(ReadString()));
        connect((device), SIGNAL(TcpError(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));
        device->Connect(ip, 23);
        m_RemoteDevices.append(device);

    } else if(remoteSupported.toInt() == 1 && modelName.startsWith("BDP")) {
        QString name=QString("%1: (%2:%03)").arg(modelName).arg(ip).arg(remotePort);
        QList<QListWidgetItem*> find = ui->listWidget->findItems(name,Qt::MatchContains| Qt::MatchRecursive);

//        ui->listWidget->findItems()
        if(find.size()==0) {
            ui->listWidget->addItem(name);
            if (ui->listWidget->count() == 1) {
                ui->listWidget->setCurrentRow(0);
                m_SelectedAddress = ip;
                m_SelectedPort = remotePort.toInt();
            }
            ui->listWidget->item(ui->listWidget->count() - 1)->setData(Qt::UserRole, ip);
            ui->listWidget->item(ui->listWidget->count() - 1)->setData(Qt::UserRole + 1, remotePort.toInt());
        } else {
            qDebug()<<name<<"already in list";
        }
    }
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
    QString str = QString::fromLatin1((const char*)&data[0]);
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
            if (ui->listWidget->count() == 1) {
                ui->listWidget->setCurrentRow(0);
                m_SelectedAddress = device->ip;
                m_SelectedPort = device->port;
            }
            ui->listWidget->item(ui->listWidget->count() - 1)->setData(Qt::UserRole, device->ip);
            ui->listWidget->item(ui->listWidget->count() - 1)->setData(Qt::UserRole + 1, device->port);
            int idx = m_RemoteDevices.indexOf(device);
            if (idx != -1)
                m_RemoteDevices.remove(idx);
            device->socket->close();
            device->socket->disconnect();
            device->deleteLater();
            return;
        }
    }

    int port = device->port;
    QString ip = device->ip;
    int idx = m_RemoteDevices.indexOf(device);
    if (idx != -1)
        m_RemoteDevices.remove(idx);
    device->deleteLater();
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
    //m_SelectedIndex = index.row();
    m_SelectedAddress = ui->listWidget->item(index.row())->data(Qt::UserRole).toString();
    m_SelectedPort = ui->listWidget->item(index.row())->data(Qt::UserRole + 1).toInt();
}

void AutoSearchDialog::on_listWidget_doubleClicked(const QModelIndex &index)
{
    m_Result = 1;
    //m_SelectedIndex = index.row();
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
            //qDebug() << data;

            if (data.contains("200 OK", Qt::CaseInsensitive) && data.contains("rootdevice", Qt::CaseInsensitive)) {
                //qDebug() << remoteAddr.toString() << data;
                QStringList ll = data.split(QRegExp("[\n\r]"), QString::SkipEmptyParts);
                QString location;
                foreach (QString s, ll)
                {
                    //qDebug() << s;
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
    QByteArray datagram = "M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: \"ssdp:discover\"\r\nMX: 1\r\nST: upnp:rootdevice\r\nUSER-AGENT: AVRPioRemote\r\n\r\n\r\n";

    foreach(QUdpSocket* socket, m_MulticatsSockets)
    {
        //qDebug() << "Send to :" << socket->multicastInterface().humanReadableName();
        if (socket->writeDatagram(datagram, m_GroupAddress, 1900) == -1)
        {
            qDebug() << QString("Error on %1!!!").arg(socket->localAddress().toString());
        }
    }
}

