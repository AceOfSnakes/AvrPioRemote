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
#include <QTextCodec>
#include <QStringList>
#include <QRegExp>

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


ReceiverInterface::ReceiverInterface()
{
    m_Connected = false;

    // socket
    connect((&m_Socket), SIGNAL(connected()), this, SLOT(TcpConnected()));
    connect((&m_Socket), SIGNAL(disconnected()), this, SLOT(TcpDisconnected()));
    connect((&m_Socket), SIGNAL(readyRead()), this, SLOT(ReadString()));
    //connect((&m_Socket), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SocketStateChanged(QAbstractSocket::SocketState)));
    connect((&m_Socket), SIGNAL(error(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));
}


void ReceiverInterface::ConnectToReceiver(const QString& IpAddress, const int IpPort)
{
    m_Socket.connectToHost(IpAddress, IpPort);
}


void ReceiverInterface::Disconnect()
{
    m_Connected = false;
    m_Socket.disconnectFromHost();
    m_Socket.close();
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


void ReceiverInterface::ReadString()
{
    // read all available data
    int count = m_Socket.bytesAvailable();
    std::vector<char> data;
    data.resize(count + 1);
    m_Socket.read(&data[0], count);
    data[count] = '\0';

    // split lines
    int lineLength = 0;
    int lineStartPos = 0;
    for(int i = 0; i < count; i++)
    {
        if (data[i] != '\n' && data[i] != '\r')
        {
            continue;
        }
        lineLength = i - lineStartPos;
        if (lineLength > 0)
        {
            m_ReceivedString.append((const char*)&data[lineStartPos], 0, lineLength);
            m_ReceivedString = trim(m_ReceivedString, "\r");
            m_ReceivedString = trim(m_ReceivedString, "\n");
            if (m_ReceivedString != "")
            {
                QString str;
                //QTextCodec::setCodecForCStrings();
                str = str.fromUtf8(m_ReceivedString.c_str());

//                QByteArray encodedString = m_ReceivedString.c_str();
//                QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
//                str = codec->toUnicode(encodedString);
                //str = str.fromStdString(m_ReceivedString);
//                str = str.trimmed();
//                str.remove(QChar('\r'));
//                str.remove(QChar('\n'));
                InterpretString(str);
                MsgDistributor::NotifyListener(str);
                emit DataReceived(str);
            }
            m_ReceivedString = "";
        }
        lineStartPos = i + 1;
    }
    if (lineStartPos < count)
        m_ReceivedString.append((const char*)&data[lineStartPos]);
}


void ReceiverInterface::TcpError(QAbstractSocket::SocketError socketError)
{
    //             QMessageBox::information(this, tr("Pioneer Remote"),
    //                                      tr("The host was not found. Please check the "
    //                                         "host name and port settings."));
    QString str;
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        str = QString("Host closed connection: %1").arg(m_Socket.errorString());
        break;
    case QAbstractSocket::HostNotFoundError:
        str = QString("Host not found: %1").arg(m_Socket.errorString());
        //Log(tr("The host was not found. Please check the host name and port settings."), QColor(255, 0, 0));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        str = QString("Host refused connection: %1").arg(m_Socket.errorString());
//        Log(tr("The connection was refused by the peer. "
//               "Make sure the receiver is on "
//               "and check ip address and port."), QColor(255, 0, 0));
        break;
    default:
        str = QString("The following error occurred: %1.").arg(m_Socket.errorString());
        //Log(tr("The following error occurred: %1.").arg(m_Socket.errorString()), QColor(255, 0, 0));
    }
    emit CommError(str);
//    if (m_StartConnection)
//    {
//        Xsleep::msleep(10000);
//        ConnectTCP();
//    }
}


bool ReceiverInterface::SendCmd(const QString& cmd)
{
//    Log("--> " + cmd, QColor(0, 200, 0));
    CmdToBeSend(cmd);
    //Logger::Log("--> " + cmd);
    QString tmp = cmd + "\r";
    return m_Socket.write(tmp.toLatin1(), tmp.length()) == tmp.length();
}


void ReceiverInterface::InterpretString(const QString& data)
{
    if (data.startsWith("SS") || data.startsWith("CLV") || data.startsWith("SPK"))
    {
        emit SpeakerData(data);
    }
    else if (data.startsWith("SR"))
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
             data.startsWith("GCH") ||
             data.startsWith("GDH") ||
             data.startsWith("GEH") ||
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
        QStringList l = data.mid(3).split(QRegExp("[<>]"), QString::SkipEmptyParts);
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

