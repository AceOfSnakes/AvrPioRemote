#ifndef AVRPIOREMOTE_H
#define AVRPIOREMOTE_H

#include <QDialog>
#include <QMenu>
#include <QIntValidator>
#include <QSettings>
#include <QtNetwork/QTcpSocket>
#include <QThread>
#include "Defs.h"
#include "netradiodialog.h"
#include "aboutdialog.h"
#include "loudspeakersettingsdialog.h"
#include "tunerdialog.h"

namespace Ui {
class AVRPioRemote;
}

class Xsleep : public QThread
{
public:
    static void msleep(int ms)
    {
        QThread::msleep(ms);
    }
};

//class ConnectWorker : public QObject {
//    Q_OBJECT

//public:
//    ConnectWorker(QTcpSocket &socket, QString& address, int port, QThread* mainThread);
//    ~ConnectWorker();

//public slots:
//    void process();

//signals:
//    void finished();
//    void error(QString err);

//private:
//    // add your variables here
//    QTcpSocket& Socket;
//    QString&    Address;
//    int         Port;
//    QThread&    MainThread;
//};

class LMAction : public QAction
{
    Q_OBJECT
public:
    explicit LMAction(const QString& Text, const QString& Number, QObject *Parent);
    ~LMAction();

    QString m_Number;

public slots:
    void ActionTriggered();
signals:
    void SendCmd(const QString& Cmd);
};

class AVRPioRemote : public QDialog
{
    Q_OBJECT
    
public:
    explicit AVRPioRemote(QWidget *parent = 0);
    ~AVRPioRemote();
    
private:
    Ui::AVRPioRemote *ui;
    int             m_Port;
    bool            m_Connected;
    QString         m_Ip;
    QIntValidator   m_IpValidator;
    QIntValidator   m_IpPortValidator;
    QSettings       m_Settings;
    QTcpSocket      m_Socket;
    QString         m_RestString;
    NetRadioDialog* m_NetRadioDialog;
    LoudspeakerSettingsDialog*  m_LoudspeakerSettingsDialog;
    TunerDialog*    m_TunerDialog;
//    QThread*        m_TCPThread;
    bool            m_ReceiverOnline;

    void ConnectTCP();
    void Log(const QString& text);
    void Log(const QString& text, const QColor& color);
    void InterpretString(const QString& str);
//    void RequestStatus(bool input = true);
    void SelectInputButton(int idx);
    void ClearScreen();

    QString DecodeHexString(const QString& hex);

    void closeEvent(QCloseEvent *event);
    QVector<QPushButton*> m_InputButtons;

public slots:
    void EnableControls(bool enable);
    void EnableIPInput(bool enable);
    void ReadString();
    //void SocketStateChanged(QAbstractSocket::SocketState State);
    void TcpError(QAbstractSocket::SocketError socketError);
    void TcpConnected();
    void TcpDisconnected();
    void RequestStatus(bool input = true);
    bool SendCmd(const QString& cmd);
private slots:
    void ShowNetDialog();
    void ShowAboutDialog();
    void ShowLoudspeakerSettingsDialog();
    void on_MoreButton_clicked();
    void on_VolumeUpButton_clicked();
    void onConnect();
    void on_pushButtonConnect_clicked();
    //void ConnectWorkerErrorString(QString);
    //void onConnectWorkerFinished();
    void on_VolumeDownButton_clicked();
    void on_PowerButton_clicked();
    void on_VolumeMuteButton_clicked();
    void on_InputLeftButton_clicked();
    void on_InputRightButton_clicked();
    void on_PhaseButton_clicked();
    void on_PqlsButton_clicked();
    void on_DFiltButton_clicked();
    void on_HiBitButton_clicked();
    void on_InputBdButton_clicked();
    void on_InputDvdButton_clicked();
    void on_InputDvrButton_clicked();
    void on_InputTvButton_clicked();
    void on_InputCdButton_clicked();
    void on_InputIpodButton_clicked();
    void on_InputSatButton_clicked();
    void on_InputAdptButton_clicked();
    void on_InputHdmiButton_clicked();
    void on_InputNetButton_clicked();
    void on_InputTunerButton_clicked();
    void on_Num1Button_clicked();
    void on_Num2Button_clicked();
    void on_Num3Button_clicked();
    void on_ShowAllListeningModesButton_clicked();

signals:
    void NetData(QString data);
    void DataReceived(QString data);
};

#endif // AVRPIOREMOTE_H
