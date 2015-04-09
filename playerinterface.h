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
#ifndef PLAYERINTERFACE_H
#define PLAYERINTERFACE_H

#include <QSettings>
#include <QtNetwork/QTcpSocket>
#include <string>
#include "logger.h"

using namespace std;

class PlayerInterface : public QObject
{
    Q_OBJECT
public:
    explicit PlayerInterface();

    bool IsConnected();

private:
    QTcpSocket      m_Socket;
    string          m_ReceivedString;
    bool            m_Connected;

    void InterpretString(const QString& data);
//    QString DecodeHexString(const QString& hex);

private slots:

    void TcpError(QAbstractSocket::SocketError socketError);
    void TcpConnected();
    void TcpDisconnected();
        void ReadString();
public slots:
    void ConnectToPlayer(const QString &PlayerIpAddress, const int PlayerIpPort);
    void Disconnect();
    //void SocketStateChanged(QAbstractSocket::SocketState State);
    bool SendCmd(const QString& cmd);

signals:
    void PlayerOffline(bool);
    bool CmdToBeSend(const QString& cmd); // for logging
    void CommError(QString error);
    void Connected();
    void Disconnected();
    void DataReceived(QString);
  //  void AudioStatusData(QString codec, QString samplingRate);
  //  void InputChanged(int no, QString name);
  //  void Listenextended(QString id, QString name);
  //  void ListeningModeData(QString name);
  //  void NetData(QString data);
  //  void SpeakerData(QString name);
  //  void usbData (QString name);
  //  void PlayerType (QString no, QString name);
 //   void PlayerNetworkName (QString name);
};

#endif // PLAYERINTERFACE_H
