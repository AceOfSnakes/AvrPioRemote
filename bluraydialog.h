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
#ifndef BLURAYDIALOG_H
#define BLURAYDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QListWidgetItem>
#include "playerinterface.h"
#include "settingsdialog.h"
#include <QMoveEvent>
#include <QIcon>
#include <QMap>
#include <inttypes.h>

namespace Ui {
class BluRayDialog;
}

class BluRayDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit BluRayDialog(QWidget *parent, QSettings& settings, PlayerInterface& Comm, SettingsDialog * settingsDialog);
    ~BluRayDialog();
    void ResponseReceived(ReceivedObjectBase *);
private:
    QSettings&          m_Settings;
    SettingsDialog*     m_SettingsDialog;
    Ui::BluRayDialog*   ui;
    PlayerInterface&    m_PlayerInterface;
    bool                m_PositionSet;
    bool                m_offline;
    QIcon           m_PowerButtonOnIcon;
    QIcon           m_PowerButtonOffIcon;
    
//    PlayerInterface m_PlayerInterface;
    int             m_PlayerIpPort;
    QString         m_PlayerIpAddress;
//    QIntValidator   m_PlayerIpValidator;
//    QIntValidator   m_PlayerIpPortValidator;
 //   QSettings       m_Settings;
    
    bool            m_PlayerOnline;

    

    void moveEvent(QMoveEvent*event);
    virtual void resizeEvent(QResizeEvent *event);

    void ConnectPlayer();
public slots:
    void ManualShowBluRayDialog();
    void ShowBluRayDialog(bool autoShow);
    void EnableControls(bool enable);
  //  void EnableIPInput(bool enable);
    void onConnect();
    void PlayerOffline(bool);
    void PlayerType (QString name);
    void UpdateDisplayInfo (QRegExp &rx);
private slots:
    void CheckOnline();
    void CheckOnlineInternal();
    void CommConnected();
    void CommDisconnected();
    void CommError(QString socketError);
    bool SendCmd(const QString& cmd);
    void on_pushButtonConnect_clicked();
    void on_BdPowerButton_clicked();
    void on_BdContinuedButton_clicked();
    void on_BdOpen_CloseButton_clicked();   
    void on_BdAudioButton_clicked();
    void on_BdSubtitleButton_clicked();
    void on_BdAngleButton_clicked();
    void on_BdFlDimmerButton_clicked();
    void on_BdCD_DVDButton_clicked();
    void on_BdHDMIButton_clicked();
    void on_BdTopMenuButton_clicked();
    void on_BdFunctionButton_clicked();
    void on_BdExitButton_clicked();
    void on_BdMediaGalleryButton_clicked();
    void on_BdPopUpMenuButton_clicked();
    void on_CursorUpButton_clicked();
    void on_CursorLeftButton_clicked();
    void on_CursorEnterButton_clicked();
    void on_CursorRightButton_clicked();
    void on_CursorDownButton_clicked();
    void on_BdHomeMenuButton_clicked();
    void on_BdReturnButton_clicked();
    void on_BdProgramButton_clicked();
    void on_BdBookmarkButton_clicked();
    void on_BdZoomButton_clicked();
    void on_BdIndexButton_clicked();
    void on_BdPrevButton_clicked();
    void on_BdPlayButton_clicked();   
    void on_BdNextButton_clicked();  
    void on_BdRevButton_clicked();   
    void on_BdPauseButton_clicked();   
    void on_BdStopButton_clicked();
    void on_BdFwdButton_clicked();
    void on_Bd1Button_clicked();
    void on_Bd2Button_clicked();
    void on_Bd3Button_clicked();
    void on_Bd2ndVideoButton_clicked();
    void on_Bd4Button_clicked();  
    void on_Bd5Button_clicked();  
    void on_Bd6Button_clicked();  
    void on_Bd2AudioButton_clicked();
    void on_Bd7Button_clicked();    
    void on_Bd8Button_clicked();  
    void on_Bd9Button_clicked();  
    void on_BdA_BButton_clicked();
    void on_BdClearButton_clicked();
    void on_Bd0Button_clicked();
    void on_BdEnterButton_clicked();
    void on_BdRepeatButton_clicked();
    void on_BdDisplayButton_clicked();
    void on_BdKeylockButton_clicked();
    void on_BdReplayButton_clicked();
    void on_BdSkipSearchButton_clicked();

signals:
//	    void SendCmd(QString data);
};

#endif // BLURAYDIALOG_H
