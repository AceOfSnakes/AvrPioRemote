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
#include "bluraydialog.h"
#include "settingsdialog.h"
#include "ui_bluraydialog.h"
#include <QDebug>
#include <qtextcodec.h>
#include <QDateTime>
#include <QThread>
#include <QTimer>

BluRayDialog::BluRayDialog(QWidget *parent, QSettings &settings, PlayerInterface &Comm, SettingsDialog *settingsDialog) :
    QDialog(parent),
    m_Settings(settings),
    m_SettingsDialog(settingsDialog),
    ui(new Ui::BluRayDialog),
    m_PlayerInterface(Comm),
    m_PositionSet(false)
  //   m_PlayerIpValidator(0, 255, this),
  //   m_PlayerIpPortValidator(0, 35535, this),
{
    m_PlayerIpPort = 8102;
    m_PlayerOnline = false;


    ui->setupUi(this);

    // restore the position of the window
    if (m_Settings.value("SaveBlueRayWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("BlueRayWindowGeometry").toByteArray());
    }
    EnableControls(false);
    ui->BdPowerButton->setEnabled(false);
    m_PowerButtonOnIcon.addFile ( ":/new/prefix1/images/Crystal_Clear_action_exit_green.png", QSize(128, 128));
    m_PowerButtonOffIcon.addFile ( ":/new/prefix1/images/Crystal_Clear_action_exit.png", QSize(128, 128));
    //connect((this),    SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(const QString&)));
    // player interface
    connect((&m_PlayerInterface), SIGNAL(Connected()), this, SLOT(CommConnected()));
    connect((&m_PlayerInterface), SIGNAL(Disconnected()), this, SLOT(CommDisconnected()));
    connect((&m_PlayerInterface), SIGNAL(CommError(QString)), this,  SLOT(CommError(QString)));
    connect((&m_PlayerInterface), SIGNAL(PlayerOffline(bool)), this,  SLOT(PlayerOffline(bool)));
    connect((&m_PlayerInterface), SIGNAL(PlayerType(QString)), this,  SLOT(PlayerType(QString)));
    CheckOnline();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(CheckOnlineInternal()));
    timer->start(3000);
    this->setWindowTitle(tr("Blu-Ray player"));
    //time specified in ms
    /*
    QStringList responseList;
    responseList << PowerResponse().getResponseID();
    responseList << ErrorResponse().getResponseID();
    MsgDistributor::AddResponseListener(this, responseList);
*/
}


BluRayDialog::~BluRayDialog()
{
    delete ui;
}

void BluRayDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("BlueRayWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}

void BluRayDialog::resizeEvent(QResizeEvent *event)
{
    m_Settings.setValue("BlueRayWindowGeometry", saveGeometry());
    QDialog::resizeEvent(event);
}

void BluRayDialog::ConnectPlayer()
{
    ui->pushButtonConnect->setEnabled(false);
    m_SettingsDialog->EnableIPInputBD(false);
    if (!m_PlayerInterface.IsConnected())
    {
        m_PlayerInterface.ConnectToPlayer(m_PlayerIpAddress, m_PlayerIpPort);
    }
}
void BluRayDialog::PlayerType (QString name) {
    if(m_Settings.value("ShowPlayerNameInTitle", true).toBool())
    {
        this->setWindowTitle(name);
    }
}
void BluRayDialog::PlayerOffline(bool offline) {
    //    qDebug()<<"Player offline "<<offline;
    ui->BdPowerButton->setIcon((offline) ? m_PowerButtonOffIcon : m_PowerButtonOnIcon);
    //ui->BdPowerButton->setText((offline) ? tr("ON") : tr("OFF"));
    if(offline) {
        this->setWindowTitle(tr("Blu-Ray player"));
    }

    EnableControls(!offline);
    m_offline = offline;
}

void BluRayDialog::ManualShowBluRayDialog()
{
    ShowBluRayDialog(false);
}

void BluRayDialog::ShowBluRayDialog(bool autoShow)
{   
    if ((!autoShow) || (m_Settings.value("AutoShowBlueRay", false).toBool() && !isVisible()))
    {
        if (!m_PositionSet || !m_Settings.value("SaveBlueRayWindowGeometry", false).toBool())
        {
            QWidget* Parent = dynamic_cast<QWidget*>(parent());
            if (Parent != NULL)
            {
                int x = Parent->pos().x() + Parent->width() + 20;
                QPoint pos;
                pos.setX(x);
                pos.setY(Parent->pos().y());
                move(pos);
            }
        }
        show();
    }
}

void BluRayDialog::CommConnected()
{
    qDebug()<<"player connected";
    Logger::Log("connected");
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonConnect->setText(tr("Disconnect"));
    ui->pushButtonConnect->setChecked(true);
    ui->BdPowerButton->setEnabled(true);
    m_PlayerOnline = true;
    EnableControls(true);
    CheckOnline();
}
void BluRayDialog::CommDisconnected()
{
    ui->pushButtonConnect->setText(tr("Connect to Player"));
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonConnect->setChecked(false);
    m_SettingsDialog->EnableIPInputBD(true);
    EnableControls(false);
    ui->BdPowerButton->setEnabled(false);
    m_PlayerOnline = false;
}
void BluRayDialog::CommError(QString/* socketError*/)
{
    Logger::Log("tcp error");
    m_SettingsDialog->EnableIPInputBD(true);
    ui->pushButtonConnect->setText(tr("Connect to Player"));
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonConnect->setChecked(false);
    m_SettingsDialog->EnableIPInputBD(true);
    EnableControls(false);
    m_PlayerOnline = false;
}
bool BluRayDialog::SendCmd(const QString& cmd)
{
    bool ret = m_PlayerInterface.SendCmd(cmd);
    // m_PlayerInterface.SendCmd("?P");
    // qDebug()<<cmd;
    return ret;
}

void BluRayDialog::EnableControls(bool enable)
{
    ui->BdContinuedButton->setEnabled(enable);
    ui->BdOpen_CloseButton->setEnabled(enable);
    ui->BdAudioButton->setEnabled(enable);
    ui->BdSubtitleButton->setEnabled(enable);
    ui->BdAngleButton->setEnabled(enable);
    ui->BdFlDimmerButton->setEnabled(enable);
    ui->BdCD_DVDButton->setEnabled(enable);
    ui->BdHDMIButton->setEnabled(enable);
    ui->BdTopMenuButton->setEnabled(enable);
    ui->BdFunctionButton->setEnabled(enable);
    ui->BdExitButton->setEnabled(enable);
    ui->BdMediaGalleryButton->setEnabled(enable);
    ui->BdPopUpMenuButton->setEnabled(enable);
    ui->CursorUpButton->setEnabled(enable);
    ui->CursorLeftButton->setEnabled(enable);
    ui->CursorEnterButton->setEnabled(enable);
    ui->CursorRightButton->setEnabled(enable);
    ui->CursorDownButton->setEnabled(enable);
    ui->BdHomeMenuButton->setEnabled(enable);
    ui->BdReturnButton->setEnabled(enable);
    ui->BdProgramButton->setEnabled(enable);
    ui->BdBookmarkButton->setEnabled(enable);
    ui->BdZoomButton->setEnabled(enable);
    ui->BdIndexButton->setEnabled(enable);
    ui->BdPrevButton->setEnabled(enable);
    ui->BdPlayButton->setEnabled(enable);
    ui->BdNextButton->setEnabled(enable);
    ui->BdRevButton->setEnabled(enable);
    ui->BdPauseButton->setEnabled(enable);
    ui->BdStopButton->setEnabled(enable);
    ui->BdFwdButton->setEnabled(enable);
    ui->Bd1Button->setEnabled(enable);
    ui->Bd2Button->setEnabled(enable);
    ui->Bd3Button->setEnabled(enable);
    ui->Bd2ndVideoButton->setEnabled(enable);
    ui->Bd4Button->setEnabled(enable);
    ui->Bd5Button->setEnabled(enable);
    ui->Bd6Button->setEnabled(enable);
    ui->Bd2AudioButton->setEnabled(enable);
    ui->Bd7Button->setEnabled(enable);
    ui->Bd8Button->setEnabled(enable);
    ui->Bd9Button->setEnabled(enable);
    ui->BdA_BButton->setEnabled(enable);
    ui->BdClearButton->setEnabled(enable);
    ui->Bd0Button->setEnabled(enable);
    ui->BdEnterButton->setEnabled(enable);
    ui->BdRepeatButton->setEnabled(enable);
    ui->BdDisplayButton->setEnabled(enable);
    ui->BdKeylockButton->setEnabled(enable);
    ui->BdReplayButton->setEnabled(enable);
    ui->BdSkipSearchButton->setEnabled(enable);
}

void BluRayDialog::onConnect()
{
    if (!m_PlayerInterface.IsConnected())
    {
        // connect
        // read settings from the line edits
        QString ip1, ip2, ip3, ip4, ip_port;
        // first the 4 ip address blocks
        m_SettingsDialog->GetIpAddressBD(ip1, ip2, ip3, ip4, ip_port);
        if (ip1 == "")
        {
            ip1 = "192"; // set default
        }
        if (ip2 == "")
        {
            ip2 = "168"; // set default
        }
        if (ip3 == "")
        {
            ip3 = "1"; // set default
        }
        if (ip4 == "")
        {
            ip4 = "192"; // set default
        }
        m_PlayerIpAddress = ip1 + "." + ip2 + "." + ip3 + "." + ip4;
        // and then th ip port
        if (ip_port == "")
        {
            ip_port = "8102"; // set default
            m_PlayerIpPort = 8102;
        }
        else
        {
            m_PlayerIpPort = ip_port.toInt();
        }
        // save the ip address and port permanently
        m_SettingsDialog->SetIpAddressBD(ip1, ip2, ip3, ip4, ip_port);
        m_Settings.setValue("Player_IP/1", ip1);
        m_Settings.setValue("Player_IP/2", ip2);
        m_Settings.setValue("Player_IP/3", ip3);
        m_Settings.setValue("Player_IP/4", ip4);
        m_Settings.setValue("Player_IP/PORT", ip_port);
        ConnectPlayer();
    }
    else
    {
        // disconnect
        EnableControls(false);
        //ui->BdPowerButton->setEnabled(false);
        m_PlayerInterface.Disconnect();
        m_PlayerOnline = false;
    }
    CheckOnline();
}
void BluRayDialog::CheckOnlineInternal() {
    if(m_PlayerOnline) {
        SendCmd("?P");
        SendCmd("?L");
//        SendCmd("?");
    }
}

void BluRayDialog::CheckOnline() {
    m_offline=true;
    PlayerOffline(true);
}


void BluRayDialog::on_pushButtonConnect_clicked()
{
    ui->pushButtonConnect->setChecked(!ui->pushButtonConnect->isChecked());
    onConnect();
}


void BluRayDialog::on_BdPowerButton_clicked()
{
    if (m_offline)
    {
        SendCmd("PN");
        CheckOnline();
    }
    else
    {
        SendCmd("PF");
        PlayerOffline(true);
    }
}
void BluRayDialog::on_BdContinuedButton_clicked()
{
    emit SendCmd("/A181AFAA/RU");
}

void BluRayDialog::on_BdOpen_CloseButton_clicked()
{
    emit SendCmd("/A181AFB6/RU");
}

void BluRayDialog::on_BdAudioButton_clicked()
{
    emit SendCmd("/A181AFBE/RU");
}

void BluRayDialog::on_BdSubtitleButton_clicked()
{
    emit SendCmd("/A181AF36/RU");
}

void BluRayDialog::on_BdAngleButton_clicked()
{
    emit SendCmd("/A181AFB5/RU");
}

void BluRayDialog::on_BdFlDimmerButton_clicked()
{
    emit SendCmd("/A181AFF9/RU");
}

void BluRayDialog::on_BdCD_DVDButton_clicked()
{
    emit SendCmd("/A181AF2A/RU");
}

void BluRayDialog::on_BdHDMIButton_clicked()
{
    emit SendCmd("/A181AFF8/RU");
}

void BluRayDialog::on_BdTopMenuButton_clicked()
{
    emit SendCmd("/A181AFB4/RU");
}

void BluRayDialog::on_BdFunctionButton_clicked()
{
    emit SendCmd("/A181AFB3/RU");
}

void BluRayDialog::on_BdExitButton_clicked()
{
    emit SendCmd("/A181AF20/RU");
}

void BluRayDialog::on_BdMediaGalleryButton_clicked()
{
    emit SendCmd("/A181AFF7/RU");
}

void BluRayDialog::on_BdPopUpMenuButton_clicked()
{
    emit SendCmd("/A181AFB9/RU");
}

void BluRayDialog::on_CursorUpButton_clicked()
{
    emit SendCmd("/A184FFFF/RU");
}

void BluRayDialog::on_CursorLeftButton_clicked()
{
    emit SendCmd("/A187FFFF/RU");
}

void BluRayDialog::on_CursorEnterButton_clicked()
{
    emit SendCmd("/A181AFEF/RU");
}

void BluRayDialog::on_CursorRightButton_clicked()
{
    emit SendCmd("/A186FFFF/RU");
}

void BluRayDialog::on_CursorDownButton_clicked()
{
    emit SendCmd("/A185FFFF/RU");
}

void BluRayDialog::on_BdHomeMenuButton_clicked()
{
    emit SendCmd("/A181AFB0/RU");
}

void BluRayDialog::on_BdReturnButton_clicked()
{
    emit SendCmd("/A181AFF4/RU");
}

void BluRayDialog::on_BdProgramButton_clicked()
{
    emit SendCmd("/A181AF60/RU");
}

void BluRayDialog::on_BdBookmarkButton_clicked()
{
    emit SendCmd("/A181AF61/RU");
}

void BluRayDialog::on_BdZoomButton_clicked()
{
    emit SendCmd("/A181AF62/RU");
}

void BluRayDialog::on_BdIndexButton_clicked()
{
    emit SendCmd("/A181AF63/RU");
}

void BluRayDialog::on_BdPrevButton_clicked()
{
    emit SendCmd("/A181AFEA/RU");
}

void BluRayDialog::on_BdPlayButton_clicked()
{
    emit SendCmd("/A181AF39/RU");
}

void BluRayDialog::on_BdNextButton_clicked()
{
    emit SendCmd("/A181AFE9/RU");
}

void BluRayDialog::on_BdRevButton_clicked()
{
    emit SendCmd("/A181AF3E/RU");
}

void BluRayDialog::on_BdPauseButton_clicked()
{
    emit SendCmd("/A181AF3A/RU");
}

void BluRayDialog::on_BdStopButton_clicked()
{
    emit SendCmd("/A181AF38/RU");
}

void BluRayDialog::on_BdFwdButton_clicked()
{
    emit SendCmd("/A181AF3D/RU");
}

void BluRayDialog::on_Bd1Button_clicked()
{
    emit SendCmd("/A181AFA1/RU");
}

void BluRayDialog::on_Bd2Button_clicked()
{
    emit SendCmd("/A181AFA2/RU");
}

void BluRayDialog::on_Bd3Button_clicked()
{
    emit SendCmd("/A181AFA3/RU");
}

void BluRayDialog::on_Bd2ndVideoButton_clicked()
{
    emit SendCmd("/A181AFBF/RU");
}

void BluRayDialog::on_Bd4Button_clicked()
{
    emit SendCmd("/A181AFA4/RU");
}

void BluRayDialog::on_Bd5Button_clicked()
{
    emit SendCmd("/A181AFA5/RU");
}

void BluRayDialog::on_Bd6Button_clicked()
{
    emit SendCmd("/A181AFA6/RU");
}

void BluRayDialog::on_Bd2AudioButton_clicked()
{
    emit SendCmd("/A181AFBD/RU");
}

void BluRayDialog::on_Bd7Button_clicked()
{
    emit SendCmd("/A181AFA7/RU");
}

void BluRayDialog::on_Bd8Button_clicked()
{
    emit SendCmd("/A181AFA8/RU");
}

void BluRayDialog::on_Bd9Button_clicked()
{
    emit SendCmd("/A181AFA9/RU");
}

void BluRayDialog::on_BdA_BButton_clicked()
{
    emit SendCmd("/A181AFE4/RU");
}

void BluRayDialog::on_BdClearButton_clicked()
{
    emit SendCmd("/A181AFE5/RU");
}

void BluRayDialog::on_Bd0Button_clicked()
{
    emit SendCmd("/A181AFA0/RU");
}

void BluRayDialog::on_BdEnterButton_clicked()
{
    emit SendCmd("/A181AFEF/RU");
}

void BluRayDialog::on_BdRepeatButton_clicked()
{
    emit SendCmd("/A181AFE8/RU");
}

void BluRayDialog::on_BdDisplayButton_clicked()
{
    emit SendCmd("/A181AFE3/RU");
}

void BluRayDialog::on_BdKeylockButton_clicked()
{
    emit SendCmd("/A181AF22/RU");
}

void BluRayDialog::on_BdReplayButton_clicked()
{
    emit SendCmd("/A181AF24/RU");
}

void BluRayDialog::on_BdSkipSearchButton_clicked()
{
    emit SendCmd("/A181AF25/RU");
}
