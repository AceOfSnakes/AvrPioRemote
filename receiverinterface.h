#ifndef RECEIVERINTERFACE_H
#define RECEIVERINTERFACE_H

#include <QSettings>
#include <QtNetwork/QTcpSocket>
#include <string>
#include "logger.h"

using namespace std;

class ReceiverInterface : public QObject
{
    Q_OBJECT
public:
    explicit ReceiverInterface();

    bool IsConnected();

private:
    QTcpSocket      m_Socket;
    string          m_ReceivedString;
    bool            m_Connected;

    void InterpretString(const QString& data);
    QString DecodeHexString(const QString& hex);

private slots:
    void ReadString();
    void TcpError(QAbstractSocket::SocketError socketError);
    void TcpConnected();
    void TcpDisconnected();
public slots:
    void ConnectToReceiver(const QString &IpAddress, const int IpPort);
    void Disconnect();
    //void SocketStateChanged(QAbstractSocket::SocketState State);
    bool SendCmd(const QString& cmd);

signals:
    bool CmdToBeSend(const QString& cmd); // for logging
    void CommError(QString error);
    void Connected();
    void Disconnected();
    void DataReceived(QString);
    void DisplayData(int dispNo, QString data);
    void PowerData(bool powerOn);
    void VolumeData(double dB);
    void MuteData(bool muted);
    void ErrorData(int type);
    void AudioStatusData(QString codec, QString samplingRate);
    void InputFunctionData(int no, QString name);
    void PhaseData(int phase);
    void InputNameData(QString name);
    void Listenextended(QString id, QString name);
    void ListeningModeData(QString name);
    void HiBitData(bool set);
    void PqlsData(bool set);
    void NetData(QString data);
    void DFiltData(bool set);
    void SpeakerData(QString name);
    void usbData (QString name);
    void ReceiverType (QString no, QString name);
    void ReceiverNetworkName (QString name);
};

#endif // RECEIVERINTERFACE_H
