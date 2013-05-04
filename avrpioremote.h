#ifndef AVRPIOREMOTE_H
#define AVRPIOREMOTE_H

#include <QDialog>
#include <QMenu>
#include <QIntValidator>
#include <QThread>
#include "Defs.h"
#include "netradiodialog.h"
#include "aboutdialog.h"
#include "loudspeakersettingsdialog.h"
#include "tunerdialog.h"
#include "receiverinterface.h"
#include "testdialog.h"
#include "oldfavoritesdialog.h"
#include "settingsdialog.h"
#include "eqdialog.h"
#include "logger.h"


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

class AVRPioRemote : public QDialog
{
    Q_OBJECT
    
public:
    explicit AVRPioRemote(QWidget *parent = 0);
    ~AVRPioRemote();
    
private:
    Ui::AVRPioRemote *ui;
    ReceiverInterface m_ReceiverInterface;
    int             m_IpPort;
    QString         m_IpAddress;
    QIntValidator   m_IpValidator;
    QIntValidator   m_IpPortValidator;
    QSettings       m_Settings;
    NetRadioDialog* m_NetRadioDialog;
    LoudspeakerSettingsDialog*  m_LoudspeakerSettingsDialog;
    TunerDialog*    m_TunerDialog;
    TestDialog*     m_TestDialog;
    OldFavoritesDialog* m_OldFavoritesDialog;
    SettingsDialog* m_SettingsDialog;
    EQDialog*       m_EQDialog;
//    QThread*        m_TCPThread;
    bool            m_ReceiverOnline;
    bool            m_FavoritesCompatibilityMode;

//    void Log(const QString& text);
//    void Log(const QString& text, const QColor& color);
//    void RequestStatus(bool input = true);
    void SelectInputButton(int idx);
    void ClearScreen();
    void ConnectReceiver();

    void closeEvent(QCloseEvent *event);
    QVector<QPushButton*> m_InputButtons;

public slots:
    void EnableControls(bool enable);
    void EnableIPInput(bool enable);
    void RequestStatus(bool input = true);
    bool SendCmd(const QString& cmd);
    void NewDataReceived(QString data);
    void DisplayData(int DispNo, QString data);
    void PowerData(bool powerOn);
    void VolumeData(double dB);
    void MuteData(bool muted);
    void ErrorData(int type);
    void AudioStatusData(QString codec, QString samplingRate);
    void InputFunctionData(int no, QString name);
    void PhaseData(int phase);
    void InputNameData(QString name);
    void ListeningModeData(QString name);
    void HiBitData(bool set);
    void PqlsData(bool set);
    void DFiltData(bool set);
private slots:
    void CommError(QString socketError);
    void CommConnected();
    void CommDisconnected();
    void ShowAboutDialog();
    void LMSelectedAction(QString Param);
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

    void on_InputVideoButton_clicked();

    void on_ATBEQModesButton_clicked();

signals:
    void NetData(QString data);
    void DataReceived(QString data);
};

#endif // AVRPIOREMOTE_H
