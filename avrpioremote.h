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
#include "receiver_interface/receiverinterface.h"
#include "netradiodialog.h"
#include "bluraydialog.h"
#include "aboutdialog.h"
#include "loudspeakersettingsdialog.h"
#include "tunerdialog.h"
#include "testdialog.h"
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
#include "hdmicontroldialog.h"
#include "infodialog.h"

namespace Ui {
class AVRPioRemote;
}

//class Xsleep : public QThread
//{
//public:
//    static void msleep(int ms)
//    {
//        QThread::msleep(ms);
//    }
//};


class AVRPioRemote : public QDialog, public ResponseListener
{
    Q_OBJECT
    
public:
    explicit AVRPioRemote(QWidget *parent = 0);
    ~AVRPioRemote();
    void ResponseReceived(ReceivedObjectBase *);


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
    TestDialog*                 m_PlayerTestDialog;
    SettingsDialog*             m_SettingsDialog;
    EQDialog*                   m_EQDialog;
    ListeningModeDialog*        m_Listendiag;
    usbDialog*                  m_usbDialog;
    ZoneControlDialog*          m_ZoneControlDialog;
    AVSettingsDialog*           m_AVSettingsDialog;
    MCACCEQDialog*              m_MCACCEQDialog;
    AutoSearchDialog*           m_AutoSearchDialog;
    WiringDialog*               m_WiringDialog;
    HdmiControlDialog*          m_HdmiControlDialog;
    InfoDialog*                 m_InfoDialog;

    //    QThread*        m_TCPThread;
    bool            m_ReceiverOnline;
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
    bool            m_Connected;
    bool            m_PassThroughLast;

    void SelectInputButton(int idx, int zone = 1);
    void ClearScreen();
    void ConnectReceiver();
    void ConnectPlayer();
    QPushButton* FindInputButton(int idx);

    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *e);
    QVector<QPushButton*> m_InputButtons;

private:
    void InputChanged(int no, QString name);

public slots:
    void EnableControls(bool enable);
    void RequestStatus(bool input = true);
    bool SendCmd(const QString& cmd);
    void NewDataReceived(QString data);
    void AudioStatusData(QString codec, QString samplingRate);
    void ZoneInput (int zone, int input);
    void ListeningModeData(QString name);
    void ReceiverType (QString no, QString name);
    void ReceiverNetworkName (QString name);
    void onConnect();
    void onConnectBD();
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
    void on_SRetrButton_clicked();
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
    void on_InfoButton_clicked();

    void on_ZoneControlButton_clicked();

signals:
    void NetData(QString data);
    void DataReceived(QString data);
};

#endif // AVRPIOREMOTE_H
