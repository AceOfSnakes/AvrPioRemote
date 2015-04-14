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
#include "playerinterface.h"
#include "Defs.h"
#include <QTextCodec>
#include <QStringList>
#include <QRegExp>



string trim(const string &t, const string &ws);

PlayerInterface::PlayerInterface()
{
    m_Connected = false;
    // socket
    connect((&m_Socket), SIGNAL(connected()), this, SLOT(TcpConnected()));
    connect((&m_Socket), SIGNAL(disconnected()), this, SLOT(TcpDisconnected()));
    connect((&m_Socket), SIGNAL(readyRead()), this, SLOT(ReadString()));
    //connect((&m_Socket), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SocketStateChanged(QAbstractSocket::SocketState)));
    connect((&m_Socket), SIGNAL(error(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));
}


void PlayerInterface::ConnectToPlayer(const QString& PlayerIpAddress, const int PlayerIpPort)
{

    m_Socket.connectToHost(PlayerIpAddress, PlayerIpPort);
}


void PlayerInterface::Disconnect()
{
    m_Connected = false;
    m_Socket.disconnectFromHost();
    m_Socket.close();
}


void PlayerInterface::TcpConnected()
{
      qDebug()<<"PlayerInterface::TcpConnected()";
    m_Connected = true;
    emit Connected();
}


void PlayerInterface::TcpDisconnected()
{
    qDebug()<<"PlayerInterface::TcpDisconnected()";
    m_Connected = false;
    emit Disconnected();
}


bool PlayerInterface::IsConnected()
{
    return m_Connected;
}


void PlayerInterface::ReadString()
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
                str = str.fromUtf8(m_ReceivedString.c_str());
                InterpretString(str);
                emit DataReceived(str);
            }
            m_ReceivedString = "";
        }
        lineStartPos = i + 1;
    }
    if (lineStartPos < count)
        m_ReceivedString.append((const char*)&data[lineStartPos]);
}


void PlayerInterface::TcpError(QAbstractSocket::SocketError socketError)
{
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
        break;
    default:
        str = QString("The following error occurred: %1.").arg(m_Socket.errorString());
    }
     qDebug()<<"PlayerInterface::TcpError";
    emit CommError(str);
}



bool PlayerInterface::SendCmd(const QString& cmd)
{
    CmdToBeSend(cmd);
    QString tmp = cmd + "\r";
    return m_Socket.write(tmp.toLatin1(), tmp.length()) == tmp.length();
}



void PlayerInterface::InterpretString(const QString& data)
{
   if (data.startsWith("P0")) {
       emit PlayerOffline(false);
   } else if(data.startsWith("E04")) {
       emit PlayerOffline(true);
   }
   else if (data.startsWith("BDP")) {
       emit PlayerType(data);
   }
}

