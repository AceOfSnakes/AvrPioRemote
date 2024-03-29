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
#include <QMainWindow>
#include <QMenu>
#include <QThread>
#include "Defs.h"
#include "receiver_interface/receiverinterface.h"
#include "netradiodialog.h"
#include "netonkyodialog.h"
#include "bluraydialog.h"
#include "aboutdialog.h"
#include "loudspeakersettingsdialog.h"
#include "loudspeakersettingsdialogonkyo.h"
#include "tunerdialog.h"
#include "testdialog.h"
#include "settingsdialog.h"
#include "eqdialog.h"
#include "logger.h"
#include <QTranslator>
#include <QSystemTrayIcon>
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
#include "graphiclswidget.h"
#include "mcaccprogressdialog.h"

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


class AVRPioRemote : public QMainWindow, public ResponseListener
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
    bool                        m_IsPioneer;
    QSettings                   m_Settings;
    NetRadioDialog*             m_NetRadioDialog;
    NetOnkyoDialog*             m_NetOnkyoDialog;
    BluRayDialog*               m_BluRayDialog;
    LoudspeakerSettingsDialog*  m_LoudspeakerSettingsDialog;
    LoudspeakerSettingsOnkyoDialog*  m_LoudspeakerSettingsOnkyoDialog;
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
    GraphicLSWidget*            m_InputLSConfiguration;
    GraphicLSWidget*            m_OutputLSConfiguration;
    MCACCProgressDialog*        m_MCACCProgressDialog;

    //    QThread*        m_TCPThread;
    bool            m_ReceiverOnline;
    QTranslator     m_Translater;
    QTimer          m_StatusLineTimer;
    QTimer          m_RefreshTimer;
    QPushButton*    m_SelectedInput;
    QPushButton*    m_SelectedInputZ2;
    QPushButton*    m_SelectedInputZ3;
    QSystemTrayIcon* m_tray_icon;
    bool            m_Zone2PowerOn;
    bool            m_Zone3PowerOn;

    QIcon           m_PowerButtonOnIcon;
    QIcon           m_PowerButtonOffIcon;
    bool            m_PowerOn;
    bool            m_Connected;
    bool            m_PassThroughLast;

    QVector<QPushButton*> m_PioneerInputButtons;
    QMap<int, QPushButton*> m_OnkyoInputButtons;

    int padding_left;
    int padding_right;
    int padding_top;
    int padding_bottom;
    int width;
    int height;

    void SelectInputButton(int idx, int zone = 1);
    void ClearScreen();
    void ConnectReceiver();
    void ConnectPlayer();
    QPushButton* FindInputButton(int idx);

    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
    void SetTheme(QString theme_name);
private:
    void InputChanged(int no, QString name);

public slots:
    void EnableControls(bool enable);
    void RequestStatus(bool input = true);
    bool SendCmd(const QString& cmd);
    void NewDataReceived(const QString &, bool);
    void ZoneInput (int zone, int input);
    void ListeningModeData(QString name);
    void ReceiverType (QString no, QString name);
    void ReceiverNetworkName (QString name);
    void onConnect();
    void onConnectBD();
    void exitNormally();
private slots:
    void CommError(QString socketError);
    void minimize();
    void CommConnected();
    void CommDisconnected();
    void ShowAboutDialog();
    void LMSelectedAction(QString Param);
    void on_MoreButton_clicked();
    void on_VolumeUpButton_clicked();
    void StatusLineTimeout();
    void RefreshTimeout();
    void on_pushButtonConnect_clicked();
    void on_show_hide(QSystemTrayIcon::ActivationReason reason);
    void on_show_hide();

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
    void on_AutoAlcDirectButton_clicked();
    void on_StandardButton_clicked();
    void on_AdvSurrButton_clicked();
    void on_ShowAllListeningModesButton_clicked();
    void on_InputVideoButton_clicked();
    void on_ATBEQModesButton_clicked();
    void on_InfoButton_clicked();
    void on_MinimizeToTrayChanged();
    void on_ZoneControlButton_clicked();

signals:
    void NetData(QString data);
    void DataReceived(const QString&, bool);
};

#endif // AVRPIOREMOTE_H
