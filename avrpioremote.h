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
#ifndef AVRPIOREMOTE_H
#define AVRPIOREMOTE_H

#include <QDialog>
#include <QMenu>
#include <QThread>
#include "Defs.h"
#include "netradiodialog.h"
#include "bluraydialog.h"
#include "aboutdialog.h"
#include "loudspeakersettingsdialog.h"
#include "tunerdialog.h"
#include "receiverinterface.h"
#include "testdialog.h"
#include "oldfavoritesdialog.h"
#include "settingsdialog.h"
#include "eqdialog.h"
#include "logger.h"
#include <QTranslator>
#include <QTimer>
#include "listeningmodedialog.h"
#include "usbdialog.h"
#include "zonecontroldialog.h"
#include "avsettingsdialog.h"
#include "mcacceqdialog.h"
#include "autosearchdialog.h"
#include "wiringdialog.h"

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
    ReceiverInterface           m_ReceiverInterface;
    PlayerInterface             m_PlayerInterface;
    int                         m_IpPort;
    QString                     m_IpAddress;
    QSettings                   m_Settings;
    NetRadioDialog*             m_NetRadioDialog;
    BluRayDialog*               m_BluRayDialog;
    LoudspeakerSettingsDialog*  m_LoudspeakerSettingsDialog;
    TunerDialog*                m_TunerDialog;
    TestDialog*                 m_TestDialog;
    OldFavoritesDialog*         m_OldFavoritesDialog;
    SettingsDialog*             m_SettingsDialog;
    EQDialog*                   m_EQDialog;
    ListeningModeDialog*        m_Listendiag;
    usbDialog*                  m_usbDialog;
    ZoneControlDialog*          m_ZoneControlDialog;
    AVSettingsDialog*           m_AVSettingsDialog;
    MCACCEQDialog*              m_MCACCEQDialog;
    AutoSearchDialog*           m_AutoSearchDialog;
    WiringDialog*               m_WiringDialog;

    //    QThre;ad*        m_TCPThread;
    bool            m_ReceiverOnline;
    bool            m_FavoritesCompatibilityMode;
    QTranslator     m_Translater;
    QTimer          m_StatusLineTimer;
    QPushButton*    m_SelectedInput;
    QPushButton*    m_SelectedInputZ2;
    QPushButton*    m_SelectedInputZ3;
    bool            m_Zone2PowerOn;
    bool            m_Zone3PowerOn;

    QIcon           m_PowerButtonOnIcon;
    QIcon           m_PowerButtonOffIcon;
    bool            m_PowerOn;

    void SelectInputButton(int idx, int zone = 1);
    void ClearScreen();
    void ConnectReceiver();
    QPushButton* FindInputButton(int idx);

    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *e);
    QVector<QPushButton*> m_InputButtons;

public slots:
    void EnableControls(bool enable);
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
    void ZoneInput (int zone, int input);
    void PhaseData(int phase);
    void InputNameData(QString name);
    void ListeningModeData(QString name);
    void HiBitData(bool set);
    void PqlsData(bool set);
    void DFiltData(bool set);
    void ReceiverType (QString no, QString name);
    void ReceiverNetworkName (QString name);
    void onConnect();
    void ZonePower (int zone, bool on);
private slots:
    void CommError(QString socketError);
    void CommConnected();
    void CommDisconnected();
    void ShowAboutDialog();
    void LMSelectedAction(QString Param);
    void on_MoreButton_clicked();
    void on_VolumeUpButton_clicked();
    void StatusLineTimeout();
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

    void on_ZoneControlButton_clicked();

signals:
    void NetData(QString data);
    void DataReceived(QString data);
};

#endif // AVRPIOREMOTE_H
