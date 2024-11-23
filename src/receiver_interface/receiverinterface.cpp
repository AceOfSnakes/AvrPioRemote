/*
 * AVRPioRemote
 * Copyright (C) 2013  Andreas Müller, Ulrich Mensfeld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "receiverinterface.h"

#include "Defs.h"
//#include <QTextCodec>
#include <QStringList>
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  #include <QRegExp>
#else
  #include <QRegularExpression>
#endif
#include <QMutexLocker>

string trim(const string &t, const string &ws)
{
    string str = t;
    size_t found;
    found = str.find_last_not_of(ws);
    if (found != string::npos)
        str.erase(found+1);
    else
        return "";

    return str.erase(0, str.find_first_not_of(ws.c_str()));
}

/**

MESSAGE SENDER

**/

MessageSender::MessageSender(ReceiverInterface& comm) :
    m_Comm(comm)
{

}

MessageSender::~MessageSender()
{
    Stop();
}

void MessageSender::SendMessage(const QString& msg)
{
    QMutexLocker locker(&m_QueueMutex);
    m_Queue.enqueue(msg);
    m_Semaphore.release();
}

void MessageSender::ClearQueue()
{
    QMutexLocker locker(&m_QueueMutex);
}

void MessageSender::Stop()
{
    m_Exit = true;
    m_Semaphore.release();
}

void MessageSender::run() {
    m_Exit = false;
    qDebug() << "MessageSender started";
    bool isEmpty = true;
    while(!m_Exit)
    {
        m_QueueMutex.lock();
        isEmpty = m_Queue.isEmpty();
        if (!isEmpty)
        {
            QString msg = m_Queue.dequeue();
            //m_Comm.SendCmdImmediately(msg);
            emit SendCmdImmediately(msg);
        }
        m_QueueMutex.unlock();
        msleep(50);
        m_Semaphore.acquire();
        qDebug() << "MessageSender running";
    }
    //QString result;
    /* ... here is the expensive or blocking operation ... */
    //emit resultReady(result);
}


/**

RECEIVER INTERFACE

**/

ReceiverInterface::ReceiverInterface() :
    m_MessageSender(*this)
{
    m_Connected = false;
    m_Device = NULL;
    m_IsPioneer = true;
    m_MessageSender.start();

    connect((&m_MessageSender), SIGNAL(SendCmdImmediately(const QString&)), this, SLOT(SendCmdImmediately(const QString&)));

//    // socket
//    connect((&m_Socket), SIGNAL(connected()), this, SLOT(TcpConnected()));
//    connect((&m_Socket), SIGNAL(disconnected()), this, SLOT(TcpDisconnected()));
//    connect((&m_Socket), SIGNAL(readyRead()), this, SLOT(ReadString()));
//    //connect((&m_Socket), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SocketStateChanged(QAbstractSocket::SocketState)));
//    connect((&m_Socket), SIGNAL(error(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));
}

void ReceiverInterface::Stop()
{
    m_MessageSender.Stop();
}

void ReceiverInterface::ConnectToReceiver(const QString& IpAddress, const int IpPort, const bool IsPioneer)
{
    Disconnect();

    m_IsPioneer = IsPioneer;
    if (IsPioneer)
    {
        m_Device = new PioneerReceiver;
    }
    else
    {
        m_Device = new OnkyoReceiver;
    }
    // socket
    connect((m_Device), SIGNAL(TcpConnected()), this, SLOT(TcpConnected()));
    connect((m_Device), SIGNAL(TcpDisconnected()), this, SLOT(TcpDisconnected()));
    connect((m_Device), SIGNAL(DataReceived(const QString&)), this, SLOT(ReadString(const QString&)));
    //connect((&m_Socket), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SocketStateChanged(QAbstractSocket::SocketState)));
    connect((m_Device), SIGNAL(TcpError(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));

    //m_Socket.connectToHost(IpAddress, IpPort);
    m_Device->Connect(IpAddress, IpPort);
}

void ReceiverInterface::Disconnect()
{
    m_Connected = false;
    if (m_Device != NULL)
    {
        m_Device->close();
        delete m_Device;
    }
    m_Device = NULL;
//    m_Socket.disconnectFromHost();
//    m_Socket.close();
}

void ReceiverInterface::TcpConnected()
{
    m_Connected = true;
    emit Connected();
}

void ReceiverInterface::TcpDisconnected()
{
    m_Connected = false;
    emit Disconnected();
}

bool ReceiverInterface::IsConnected()
{
    return m_Connected;
}

void ReceiverInterface::ReadString(const QString &data)
{
    m_ReceivedString = data;
    if (m_IsPioneer)
    {
        InterpretPioneerString(m_ReceivedString);
    }
    else
    {
        InterpretOnkyoString(m_ReceivedString);
    }
    MsgDistributor::NotifyListener(m_ReceivedString, m_IsPioneer);
    emit DataReceived(m_ReceivedString, m_IsPioneer);
    qDebug() << "received:" << m_ReceivedString;

}

void ReceiverInterface::TcpError(QAbstractSocket::SocketError socketError)
{
    //             QMessageBox::information(this, tr("Pioneer Remote"),
    //                                      tr("The host was not found. Please check the "
    //                                         "host name and port settings."));
    QString str;
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        str = QString("Host closed connection: %1").arg(m_Device->errorString());
        break;
    case QAbstractSocket::HostNotFoundError:
        str = QString("Host not found: %1").arg(m_Device->errorString());
        //Log(tr("The host was not found. Please check the host name and port settings."), QColor(255, 0, 0));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        str = QString("Host refused connection: %1").arg(m_Device->errorString());
//        Log(tr("The connection was refused by the peer. "
//               "Make sure the receiver is on "
//               "and check ip address and port."), QColor(255, 0, 0));
        break;
    default:
        str = QString("The following error occurred: %1.").arg(m_Device->errorString());
        //Log(tr("The following error occurred: %1.").arg(m_Socket.errorString()), QColor(255, 0, 0));
    }
    emit CommError(str);
//    if (m_StartConnection)
//    {
//        Xsleep::msleep(10000);
//        ConnectTCP();
//    }
}

void ReceiverInterface::SendCmd(const QString& cmd)
{
    m_MessageSender.SendMessage(cmd);
}

bool ReceiverInterface::SendCmdImmediately(const QString& cmd)
{
//    Log("--> " + cmd, QColor(0, 200, 0));
    CmdToBeSend(cmd);
    //Logger::Log("--> " + cmd);

    QString tmp = cmd;
    if (m_IsPioneer)
    {
        tmp = cmd + "\r";
    }
    if (m_Device != NULL)
    {
        return m_Device->write(tmp);
    }
    return false;
}

void ReceiverInterface::InterpretPioneerString(const QString& data)
{
    if (data.startsWith("SR"))
    {
        QString text = FindValueByKey(LISTENING_MODE, data.mid(2, 4));
        if (text == "")
            text = "---";
        //ui->lineEditListeningMode->setText(text);
        emit Listenextended(data.mid(2, 4), text);
    }
    else if (data.startsWith("LM"))
    {
        QString text = FindValueByKey(PLAYING_LISTENING_MODE, data.mid(2, 4));
        if (text == "")
            text = "---";
        emit ListeningModeData(text);
    }
    else if (data.startsWith("GBH") ||
             data.startsWith("GBP") ||
             data.startsWith("GCH") ||
             data.startsWith("GCP") ||
             data.startsWith("GDH") ||
             data.startsWith("GDP") ||
             data.startsWith("GEH") ||
             data.startsWith("GEP") ||
             data.startsWith("GHP") ||
             data.startsWith("GHH"))
    {
        emit NetData(data);
    }
    else if (data.startsWith("GBI") ||
             data.startsWith("GCI") ||
             data.startsWith("GDI") ||
             data.startsWith("GEI"))
    {
        emit usbData(data);
    }
    else if (data.startsWith("RGD"))
    {
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        QStringList l = data.mid(3).split(QRegExp("[<>]"),
                                          QString::SkipEmptyParts);
#else
        QStringList l = data.mid(3).split(QRegularExpression("[<>]"),
                                          Qt::SkipEmptyParts);
#endif
        QString str1, str2;
        if (l.count() > 0)
            str1 = l[0];
        if (l.count() > 1)
            str2 = l[1];
        emit ReceiverType(str1, str2);
    }
    else if (data.startsWith("SSO"))
    {
        QString name = data.mid(3);
        name = name.trimmed();
        while (name.startsWith("\""))
            name.remove(0, 1);
        while (name.endsWith("\""))
            name.chop(1);
        emit ReceiverNetworkName(name);
    }
    else if (data.startsWith("Z2F"))
    {
        int n = 0;
        sscanf(data.toLatin1(), "Z2F%d", &n);
        emit ZoneInput(2, n);
    }
    else if (data.startsWith("Z3F"))
    {
        int n = 0;
        sscanf(data.toLatin1(), "Z3F%d", &n);
        emit ZoneInput(3, n);
    }
    else if (data.startsWith("ZEA"))
    {
        int n = 0;
        sscanf(data.toLatin1(), "ZEA%d", &n);
        emit ZoneInput(4, n);
    }
}

void ReceiverInterface::InterpretOnkyoString(const QString& data)
{

    if (data.endsWith("N/A"))
    {
        MsgDistributor::NotifyListener("E04", true);
    }
    else if (data.startsWith("LMD"))
    {
        QString text = FindValueByKey(LISTENING_MODE_ONKYO, data.mid(3, 2));
        if (text == "")
            text = "---";
        emit ListeningModeData(text);
    }
    else if (/*data.startsWith("NLS") || */data.startsWith("NLT")  || data.startsWith("NTM") || data.startsWith("NJA")
              || data.startsWith("NAT") || data.startsWith("NAL")  || data.startsWith("NTI") || data.startsWith("NTR")
              || data.startsWith("NST") || data.startsWith("NLA"))
    {
        emit NetData(data);
    }
    else if (data.startsWith("SLZ"))
    {
        int n = 0;
        sscanf(data.toLatin1(), "SLZ%X", &n);
        emit ZoneInput(2, n);
    }
    else if (data.startsWith("SL3"))
    {
        int n = 0;
        sscanf(data.toLatin1(), "SL3%X", &n);
        emit ZoneInput(3, n);
    }
}
