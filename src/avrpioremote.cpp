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
#include "avrpioremote.h"
#include "ui_avrpioremote.h"
#include <QDebug>
#include <QNetworkProxy>
#include "actionwithparameter.h"
#include <QWidget>
#include <QSizePolicy>
#include <QStyleFactory>
#include <QIcon>
#include "themereader.h"
//#include <QtSvg>
#ifdef Q_OS_LINUX
#include <QtX11Extras/qx11info_x11.h>
#endif

AVRPioRemote::AVRPioRemote(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AVRPioRemote),
    m_Settings(QSettings::IniFormat, QSettings::UserScope, "AVRPIO", "AVRPioRemote"),
    m_StatusLineTimer(this),
    m_ReceiverInterface(),
    m_PlayerInterface(),
    m_RefreshTimer(this)
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    this->setWindowFlags(Qt::Dialog);
    m_IpPort = 8102;
    m_IsPioneer = true;
    m_ReceiverOnline = false;
    m_tray_icon = NULL;
    m_SelectedInput   = NULL;
    m_SelectedInputZ2 = NULL;
    m_SelectedInputZ3 = NULL;
    m_Zone2PowerOn    = false;
    m_Zone3PowerOn    = false;

    m_PowerOn         = false;
    m_Connected       = false;
    m_PassThroughLast = false;

    padding_left = 0;
    padding_right = 0;
    padding_top = 0;
    padding_bottom = 0;

   //
    //connect( QApplication::closeAllWindows(), SIGNAL(destroyed(QObject*)), this, SLOT(exitNormally()) );
    qDebug()<<"aaaaa aaa aaa";
    QString lang = m_Settings.value("Language", "auto").toString();
    if (lang == "auto")
    {
        lang = QLocale::system().name();
    }
    if (lang.startsWith("de"))
    {
        m_Translater.load(QString::fromUtf8(":/new/prefix1/avrpioremote_de"));
    }
    else if (lang.startsWith("ru"))
    {
        m_Translater.load(QString::fromUtf8(":/new/prefix1/avrpioremote_ru"));
    }
    else
    {
        m_Translater.load(QString::fromUtf8(":/new/prefix1/avrpioremote_en"));
    }
    QCoreApplication::installTranslator(&m_Translater);
    ui->setupUi(this);
    width = size().width();
    height = size().height();
    // add minimize button to to title
    //    Qt::WindowFlags flags =
    //    Qt::CustomizeWindowHint |
    //            Qt::WindowMinimizeButtonHint
    //            |Qt::WindowMaximizeButtonHint
    //            | Qt::WindowCloseButtonHint
    //            | Qt::MSWindowsFixedSizeDialogHint;
    //    this->setWindowFlags(flags );

    // restore the position of the window
    if (m_Settings.value("SaveMainWindowGeometry", true).toBool())
    {
        restoreGeometry(m_Settings.value("MainWindowGeometry").toByteArray());
    }

    if (m_Settings.value("UseBlackTheme", true).toBool()) {
        SetTheme("theme/main-window.xml");
    }
    QNetworkProxy proxy;
    QNetworkProxy::setApplicationProxy(proxy);

    m_PowerButtonOffIcon.addFile ( ":/new/prefix1/images/Crystal_Clear_action_exit_green.png", QSize(128, 128));
    m_PowerButtonOnIcon.addFile ( ":/new/prefix1/images/Crystal_Clear_action_exit.png", QSize(128, 128));

    // receiver interface
    connect((&m_ReceiverInterface), SIGNAL(Connected()), this, SLOT(CommConnected()));
    connect((&m_ReceiverInterface), SIGNAL(Disconnected()), this, SLOT(CommDisconnected()));
    connect((&m_ReceiverInterface), SIGNAL(CommError(QString)), this,  SLOT(CommError(QString)));
    connect((&m_ReceiverInterface), SIGNAL(DataReceived(const QString&, bool)), this,  SLOT(NewDataReceived(const QString&, bool)));
    connect((&m_ReceiverInterface), SIGNAL(ListeningModeData(QString)), this,  SLOT(ListeningModeData(QString)));
    connect((&m_ReceiverInterface), SIGNAL(ReceiverType(QString,QString)), this,  SLOT(ReceiverType(QString,QString)));
    connect((&m_ReceiverInterface), SIGNAL(ReceiverNetworkName(QString)), this,  SLOT(ReceiverNetworkName(QString)));
    connect((&m_ReceiverInterface), SIGNAL(ZoneInput(int, int)),this,SLOT(ZoneInput(int, int)));

    // PIONEER BUTTONS

    // make a list with buttons that correspond to a input type
    /* 00 */ m_PioneerInputButtons.append(NULL); //"PHONO"
    /* 01 */ m_PioneerInputButtons.append(ui->InputCdButton); //"CD",
    /* 02 */ m_PioneerInputButtons.append(ui->InputTunerButton); //"TUNER",
    /* 03 */ m_PioneerInputButtons.append(NULL); //"CD-R/TAPE",
    /* 04 */ m_PioneerInputButtons.append(ui->InputDvdButton); //"DVD",
    /* 05 */ m_PioneerInputButtons.append(ui->InputTvButton); //"TV/SAT",
    /* 06 */ m_PioneerInputButtons.append(ui->InputSatButton); //"SAT/CBL",
    /* 07 */ m_PioneerInputButtons.append(NULL); //"",
    /* 08 */ m_PioneerInputButtons.append(NULL); //"",
    /* 09 */ m_PioneerInputButtons.append(NULL); //"",
    /* 10 */ m_PioneerInputButtons.append(ui->InputVideoButton); //"VIDEO 1(VIDEO)",
    /* 11 */ m_PioneerInputButtons.append(NULL); //"",
    /* 12 */ m_PioneerInputButtons.append(NULL); //"MULTI CH IN",
    /* 13 */ m_PioneerInputButtons.append(NULL); //"",
    /* 14 */ m_PioneerInputButtons.append(NULL); //"VIDEO 2",
    /* 15 */ m_PioneerInputButtons.append(ui->InputDvrButton); //"DVR/BDR",
    /* 16 */ m_PioneerInputButtons.append(NULL); //"",
    /* 17 */ m_PioneerInputButtons.append(ui->InputIpodButton); //"iPod/USB",
    /* 18 */ m_PioneerInputButtons.append(ui->InputNetButton); //"XM RADIO",
    /* 19 */ m_PioneerInputButtons.append(ui->InputHdmiButton); //"HDMI 1",
    /* 20 */ m_PioneerInputButtons.append(ui->InputHdmiButton); //"HDMI 2",
    /* 21 */ m_PioneerInputButtons.append(ui->InputHdmiButton); //"HDMI 3",
    /* 22 */ m_PioneerInputButtons.append(ui->InputHdmiButton); //"HDMI 4",
    /* 23 */ m_PioneerInputButtons.append(ui->InputHdmiButton); //"HDMI 5",
    /* 24 */ m_PioneerInputButtons.append(ui->InputHdmiButton); //"HDMI 6",
    /* 25 */ m_PioneerInputButtons.append(ui->InputBdButton); //"BD",
    /* 26 */ m_PioneerInputButtons.append(ui->InputNetButton); //"HOME MEDIA GALLERY(Internet Radio)",
    /* 27 */ m_PioneerInputButtons.append(ui->InputNetButton); //"SIRIUS",
    /* 28 */ m_PioneerInputButtons.append(NULL); //"",
    /* 29 */ m_PioneerInputButtons.append(NULL); //"",
    /* 30 */ m_PioneerInputButtons.append(NULL); //"",
    /* 31 */ m_PioneerInputButtons.append(ui->InputHdmiButton); //"HDMI (cyclic)",
    /* 32 */ m_PioneerInputButtons.append(NULL); //"",
    /* 33 */ m_PioneerInputButtons.append(ui->InputAdptButton); //"ADAPTER PORT",
    /* 34 */ m_PioneerInputButtons.append(ui->InputHdmiButton); //"HDMI 7",
    /* 35 */ m_PioneerInputButtons.append(NULL); //"",
    /* 36 */ m_PioneerInputButtons.append(NULL); //"",
    /* 37 */ m_PioneerInputButtons.append(NULL); //"",
    /* 38 */ m_PioneerInputButtons.append(ui->InputNetButton); //"INTERNET RADIO",
    /* 39 */ m_PioneerInputButtons.append(NULL); //"",
    /* 40 */ m_PioneerInputButtons.append(ui->InputNetButton); //"SiriusXM",
    /* 41 */ m_PioneerInputButtons.append(ui->InputNetButton); //"PANDORA",
    /* 42 */ m_PioneerInputButtons.append(NULL); //"",
    /* 43 */ m_PioneerInputButtons.append(NULL); //"",
    /* 44 */ m_PioneerInputButtons.append(ui->InputNetButton); //"MEDIA SERVER",
    /* 45 */ m_PioneerInputButtons.append(ui->InputNetButton); //"FAVORITES",
    /* 46 */ m_PioneerInputButtons.append(NULL); //"",
    /* 47 */ m_PioneerInputButtons.append(NULL); //"",
    /* 48 */ m_PioneerInputButtons.append(ui->InputHdmiButton); //"MHL",
    /* END */ m_PioneerInputButtons.append((QPushButton*)-1);

    // ONKYO BUTTONS

    m_OnkyoInputButtons[0x01] = ui->InputSatButton; //"CBL/SAT";
    m_OnkyoInputButtons[0x02] = ui->InputDvdButton; //"GAME";
    m_OnkyoInputButtons[0x03] = NULL; //"AUX";
    m_OnkyoInputButtons[0x10] = ui->InputBdButton; //"BD/DVD";
    m_OnkyoInputButtons[0x11] = ui->InputNetButton; //"STRM BOX";
    m_OnkyoInputButtons[0x12] = ui->InputTvButton; //"TV";1
    m_OnkyoInputButtons[0x22] = NULL; //"PHONO";
    m_OnkyoInputButtons[0x23] = ui->InputCdButton; //"CD";
    m_OnkyoInputButtons[0x24] = ui->InputTunerButton; //"FM";
    m_OnkyoInputButtons[0x25] = ui->InputTunerButton; //"AM";
    m_OnkyoInputButtons[0x26] = ui->InputTunerButton; //"TUNER";
    m_OnkyoInputButtons[0x29] = ui->InputIpodButton; //"USB(Front)";
    m_OnkyoInputButtons[0x2B] = ui->InputNetButton; //"NET";
    m_OnkyoInputButtons[0x2C] = NULL; //"USB(toggle)";
    m_OnkyoInputButtons[0x2E] = NULL; //"BT AUDIO";
    m_OnkyoInputButtons[0x55] = ui->InputHdmiButton; //"HDMI 5";
    m_OnkyoInputButtons[0x56] = ui->InputHdmiButton; //"HDMI 6";
    m_OnkyoInputButtons[0x57] = ui->InputHdmiButton; //"HDMI 7";


    // configure the timer
    connect((&m_StatusLineTimer), SIGNAL(timeout()), this, SLOT(StatusLineTimeout()));
    m_StatusLineTimer.setSingleShot(true);
    m_StatusLineTimer.setInterval(10000);

    connect((&m_RefreshTimer), SIGNAL(timeout()), this, SLOT(RefreshTimeout()));
    m_RefreshTimer.setSingleShot(false);
    m_RefreshTimer.setInterval(10000);

    // create NetRadio dialog
    m_NetRadioDialog = new NetRadioDialog(this, m_Settings, m_ReceiverInterface);
    m_NetOnkyoDialog = new NetOnkyoDialog(this, m_Settings, m_ReceiverInterface);


    // create usb dialog
    m_usbDialog = new usbDialog(this, m_Settings, m_ReceiverInterface);

    // create loudspeaker dialog
    m_LoudspeakerSettingsDialog = new LoudspeakerSettingsDialog(this, m_Settings, m_ReceiverInterface );
    m_LoudspeakerSettingsOnkyoDialog = new LoudspeakerSettingsOnkyoDialog(this, m_Settings, m_ReceiverInterface );

    // create Tuner dialog
    m_TunerDialog = new TunerDialog(this, m_ReceiverInterface, m_Settings);

    // create Test dialog
    m_TestDialog = new TestDialog(this, m_ReceiverInterface, m_Settings, tr("Receiver"));

    // create Settings dialog
    m_SettingsDialog = new SettingsDialog(this, m_Settings, m_ReceiverInterface, m_PlayerInterface);
    // create BluRay dialog
    m_BluRayDialog = new BluRayDialog(this, m_Settings, m_PlayerInterface, m_SettingsDialog);

    // create Test dialog
    m_PlayerTestDialog = new TestDialog(this, m_PlayerInterface, m_Settings, tr("Player"));

    // create EQ dialog
    m_EQDialog = new EQDialog(this, m_ReceiverInterface, m_Settings);

    m_Listendiag = new  ListeningModeDialog(this, m_Settings, m_ReceiverInterface);

    m_ZoneControlDialog = new ZoneControlDialog(this, m_Settings, m_ReceiverInterface);

    m_AVSettingsDialog = new AVSettingsDialog(this, m_Settings, m_ReceiverInterface);

    m_MCACCEQDialog = new MCACCEQDialog(this, m_Settings, m_ReceiverInterface);

    m_AutoSearchDialog = NULL;

    m_WiringDialog = new WiringDialog(this, m_Settings, m_ReceiverInterface);

    m_HdmiControlDialog = new HdmiControlDialog(this, m_Settings, m_ReceiverInterface);

    m_InfoDialog = new InfoDialog(this, m_Settings, m_ReceiverInterface);

    m_MCACCProgressDialog = new MCACCProgressDialog(this, m_Settings, m_ReceiverInterface);

    m_InputLSConfiguration = new GraphicLSWidget(this, true);
    m_InputLSConfiguration->makeSmall();
    m_InputLSConfiguration->move(11 + padding_left, 50 + padding_top);
    m_OutputLSConfiguration = new GraphicLSWidget(this, false);
    m_OutputLSConfiguration->makeSmall();
    m_OutputLSConfiguration->move(164 + padding_left, 50 + padding_top);

    // disable controls
    EnableControls(false);
    ui->PowerButton->setEnabled(false);
    ui->ZoneControlButton->setEnabled(false);

    QStringList responseList;
    responseList << InputFunctionResponse_FN().getResponseID();
    responseList << PowerResponse_PWR_APR_BPR_ZEP().getResponseID();
    responseList << DisplayDataResponse_FL().getResponseID();
    responseList << HDMIPassThroughResponse_STU().getResponseID();
    responseList << VolumeResponse_VOL_ZV_YV().getResponseID();
    responseList << MuteResponse_MUT_Z2MUT_Z3MUT().getResponseID();
    responseList << InputNameResponse_RGB().getResponseID();
    responseList << ErrorResponse_B_E().getResponseID();
    responseList << PhaseControlResponse_IS().getResponseID();
    responseList << HiBitResponse_ATI().getResponseID();
    responseList << PQLSControlResponse_PQ().getResponseID();
    responseList << SoundRetrieverResponse_ATA().getResponseID();
    responseList << AudioStatusDataResponse_AST().getResponseID();
    responseList << Response_AUB().getResponseID();
    MsgDistributor::AddResponseListener(this, responseList);

    if(m_tray_icon == NULL) {
        m_tray_icon = new QSystemTrayIcon(QIcon(":/new/prefix1/images/AVRPioRemote.png"));

        connect( m_tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_show_hide(QSystemTrayIcon::ActivationReason)) );
        QAction *quit_action = new QAction(QIcon(":/new/prefix1/images/close.png"), "Exit", m_tray_icon );
        connect( quit_action, SIGNAL(triggered()), this, SLOT(exitNormally()) );

        QAction *hide_action = new QAction(QIcon(":/new/prefix1/images/showHide.png"), "Show/Hide", m_tray_icon );
        connect( hide_action, SIGNAL(triggered()), this, SLOT(on_show_hide()) );

        QMenu *tray_icon_menu = new QMenu;
        tray_icon_menu->addAction( hide_action );
        tray_icon_menu->addAction( quit_action );

        m_tray_icon->setContextMenu( tray_icon_menu );
        if(m_Settings.value("MinimizeToTrayCheckBox", false).toBool()) {
            m_tray_icon->show();
        }
        connect(m_SettingsDialog,SIGNAL(minimizeToTrayChanged()), this, SLOT(on_MinimizeToTrayChanged()));
    }
    qApp->installEventFilter(this);
}

void AVRPioRemote::on_MinimizeToTrayChanged() {
    if(m_Settings.value("MinimizeToTrayCheckBox", false).toBool()) {
        m_tray_icon->show();
    }
    else {
        m_tray_icon->hide();
    }
}

AVRPioRemote::~AVRPioRemote()
{
    qApp->removeEventFilter(this);
    delete m_NetRadioDialog;
    delete m_NetOnkyoDialog;
    delete m_BluRayDialog;
    delete m_usbDialog;
    delete m_LoudspeakerSettingsDialog;
    delete m_LoudspeakerSettingsOnkyoDialog;
    delete m_TunerDialog;
    delete m_TestDialog;
    delete m_PlayerTestDialog;
    delete m_SettingsDialog;
    delete m_EQDialog;
    delete m_Listendiag;
    delete m_ZoneControlDialog;
    delete m_AVSettingsDialog;
    delete m_MCACCEQDialog;
    delete m_WiringDialog;
    delete m_HdmiControlDialog;
    delete m_InfoDialog;
    delete m_MCACCProgressDialog;
    if(m_tray_icon != NULL) {
        delete m_tray_icon;
    }
    delete ui;
}


void AVRPioRemote::closeEvent(QCloseEvent *event)
{
    qDebug()<< event;
    m_ReceiverInterface.Disconnect();
    // save the window position
    m_Settings.setValue("MainWindowGeometry", saveGeometry());
    m_ReceiverInterface.Stop();
    QMainWindow::closeEvent(event);
}

void AVRPioRemote::changeEvent(QEvent *e)
{

    qDebug()<<e;
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::WindowStateChange: {
        QWindowStateChangeEvent* event =
                static_cast< QWindowStateChangeEvent* >( e );
        if(!isMinimized() && isVisible()) {
            showNormal();
            break;
        }
        //qDebug()<<event<<isMinimized()<<event->oldState();
        if(((event->oldState() == Qt::WindowNoState || event->oldState() & Qt::WindowMaximized)&& isMinimized()) || ((event->oldState() &Qt::WindowMinimized ) && !isMinimized() )) {
            on_show_hide();
        }
    }
        break;
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool AVRPioRemote::eventFilter(QObject *obj, QEvent *event)
{
    static bool mouse_down = false;
    static int x_rel_pos = 0;
    static int y_rel_pos = 0;

    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if (obj->objectName() == ui->centralWidget->objectName() && event->type() == QEvent::MouseButtonPress) {
        mouse_down = true;
        x_rel_pos = mouseEvent->globalPos().x() - x();
        y_rel_pos = mouseEvent->globalPos().y() - y();
    } else if (event->type() == QEvent::MouseButtonRelease) {
        mouse_down = false;
    } else if (event->type() == QEvent::MouseMove) {
        if (mouse_down)
            this->move(x() - (x() - mouseEvent->globalPos().x()) - x_rel_pos, y() - (y() - mouseEvent->globalPos().y()) - y_rel_pos);
    }
    return false;
}

void AVRPioRemote::SetTheme(QString theme_name) {
    //    QtSvg library
    //    plugin iconengines/libqsvgicon.dll
    //    plugin imageformats/libqsvg.dll
    //    the dependencies: QtCore, QtGui, QtXml library

    //    QStringList libPaths = QCoreApplication::libraryPaths();
    //    libPaths << QCoreApplication::applicationDirPath()
    //                 + "/plugins";
    //    QCoreApplication::setLibraryPaths(libPaths);

    //this->statusBar()->setSizeGripEnabled(false);


    //setWindowFlags(Qt::FramelessWindowHint);
    //QIcon pixmap("J:\\Projekte\\blue-bkgnd.svg");
    ThemeReader theme;
    if (!theme.readTheme(theme_name)) {
        return;
    }
    ThemeReader::ThemeElement window_theme = theme.getElement("window");
    if (window_theme.name == "") {
        return;
    }

    padding_left = theme.padding_left;
    padding_right =theme.padding_right;
    padding_top = theme.padding_top;
    padding_bottom = theme.padding_bottom;
    if (window_theme.width != 0) {
        width = window_theme.width;
    }
    if (window_theme.height != 0) {
        height = window_theme.height;
    }

    //QString path = QCoreApplication::applicationDirPath() + "/theme/bkgnd.png";
    if (theme.background_image != "") {
        QPixmap pixmap(theme.background_image);
        if(pixmap.height() > 0) {
            setWindowFlags(Qt::FramelessWindowHint);
#ifdef Q_OS_LINUX
            if(QX11Info::isPlatformX11() ){
                if(QX11Info::isCompositingManagerRunning()) {
                    setAttribute(Qt::WA_TranslucentBackground);
                }
            }
#else
            setAttribute(Qt::WA_TranslucentBackground);
#endif
            pixmap = pixmap.scaled(width + padding_left + padding_right, height + padding_top + padding_bottom);
            ui->centralWidget->setPixmap(pixmap);

            ThemeReader::ThemeElement exit_button_theme = theme.getElement("exit-button");
            if (exit_button_theme.name != "") {
                QPushButton *exitButton = new QPushButton(ui->centralWidget);
                exitButton->setFixedSize(exit_button_theme.width, exit_button_theme.height);
                exitButton->move(exit_button_theme.posX, exit_button_theme.posY);
                if (exit_button_theme.style != "") {
                    exitButton->setStyleSheet(exit_button_theme.style);
                }
                connect(exitButton, SIGNAL(clicked()), this, SLOT(exitNormally()));
            }
            ThemeReader::ThemeElement minimize_button_theme = theme.getElement("minimize-button");
            if (minimize_button_theme.name != "") {
                QPushButton *minimizeButton = new QPushButton(ui->centralWidget);
                minimizeButton->setFixedSize(minimize_button_theme.width, minimize_button_theme.height);
                minimizeButton->move(minimize_button_theme.posX, minimize_button_theme.posY);
                if (minimize_button_theme.style != "") {
                    minimizeButton->setStyleSheet(minimize_button_theme.style);
                }
                connect(minimizeButton, SIGNAL(clicked()), this, SLOT(minimize()));
            }
            //        showMinimized();

        }
    }

    //QPixmap pixmap("J:\\Projekte\\blue-bkgnd2.png");
    //qDebug() << pixmap;
    //ui->centralWidget->setPixmap(pixmap.pixmap(QSize()));
    //qDebug() << QImageReader::supportedImageFormats();
    //qDebug() << pixmap.availableSizes();

    //    qDebug()<<"dedede"<<flags;
    // set not resizeable
    this->setFixedSize(width + padding_left + padding_right, height + padding_top + padding_bottom);
    ui->mainWidget->move(padding_left, padding_top);
    theme.applyThemeToWidgets(ui->centralWidget);
    //exitButton->setText("Exit");
    //        exitButton->setStyleSheet(
    //                    "QPushButton {"
    //                    "  border-image: url(" + QCoreApplication::applicationDirPath() + "/theme/sys_exit_normal.png) 0 0 0 0 stretch stretch;"
    //                    //"  border-image: url(" + QCoreApplication::applicationDirPath() + "/theme/sys_exit_normal.png)  no-repeat center center fixed;"
    //                    "  background-color:rgba(0, 0, 0, 0.0);"
    //                    "}"
    //                    "QPushButton:hover:!pressed {"
    //                    "  border-image: url(" + QCoreApplication::applicationDirPath() + "/theme/sys_exit_highlight.png) 0 0 0 0 stretch stretch;"
    //                    "}"
    //                    "QPushButton:hover {"
    //                    "  border-image: url(" + QCoreApplication::applicationDirPath() + "/theme/sys_exit_pressed.png) 0 0 0 0 stretch stretch;"
    //                    "}"
    //        );
}
void AVRPioRemote::on_show_hide( ) {

    if( isVisible() )  {
        hide();

        foreach(QDialog* x,this->findChildren<QDialog *>()) {
            //qDebug()<<x->objectName()<<x->isVisible()<<x->isActiveWindow()<<x->isMinimized();
            if(x->isVisible()) {
                x->showMinimized();
                x->setVisible(false);
            }
        }
    }
    else {
        //qDebug()<<"\n\nRestore\n";
        showNormal();
        raise();
        setFocus();
        foreach(QDialog* x,this->findChildren<QDialog *>()) {
            //qDebug()<<x->objectName()<<x->isHidden()<<x->isVisible()<<x->isMinimized();
            if(x->isVisible() || x->isMinimized()) {
                x->showNormal();
                x->raise();
            }
        }

        qApp->setActiveWindow(this);
    }
}

void AVRPioRemote::on_show_hide( QSystemTrayIcon::ActivationReason reason )
{
    if( reason )     {
        if( reason != QSystemTrayIcon::Trigger ) {
            return;
        }
    }

    on_show_hide();

}
void AVRPioRemote::minimize() {
    if(m_Settings.value("MinimizeToTrayCheckBox", false).toBool()) {
        on_show_hide();
    } else {
        emit showMinimized();
    }
}

void AVRPioRemote::exitNormally()
{

    qDebug() <<"exit";
    emit qApp->exit(0);
    exit(0);
}

void AVRPioRemote::StatusLineTimeout()
{
    ui->StatusLineEdit->clear();
}

void AVRPioRemote::RefreshTimeout()
{
    if (m_IsPioneer)
    {
        SendCmd("?AST");
        SendCmd("?VST");
    }
    else
    {
        SendCmd("IFAQSTN");
        SendCmd("IFVQSTN");
    }
}


QPushButton* AVRPioRemote::FindInputButton(int idx)
{
    QPushButton* found = NULL;
    int i = 0;
    // find a button associated with the input
    if (m_IsPioneer)
    {
        while (m_PioneerInputButtons[i] != (QPushButton*)-1)
        {
            if (m_PioneerInputButtons[i] != NULL)
            {
                if (i == idx)
                {
                    found = m_PioneerInputButtons[i];
                    break;
                }
            }
            i++;
        }
    }
    else
    {
        found = m_OnkyoInputButtons.value(idx, NULL);
    }
    return found;
}


void AVRPioRemote::SelectInputButton(int idx, int zone)
{
    int i = 0;
    QPushButton* found = NULL;
    // find a button associated with the input
    found = FindInputButton(idx);

    if (zone == 1)
        m_SelectedInput = found;
    else if (zone == 2)
        m_SelectedInputZ2 = found;
    else if (zone == 3)
        m_SelectedInputZ3 = found;

    if (zone == 1 && found != NULL)
    {
        m_SelectedInput = found;
        if (m_IsPioneer)
        {
            while (m_PioneerInputButtons[i] != (QPushButton*)-1)
            {
                if (m_PioneerInputButtons[i] != NULL)
                {
                    if (i == idx)
                    {
                        // select the found input button
                        m_PioneerInputButtons[i]->setChecked(true);
                    }
                    else
                    {
                        // unselect all other input buttons
                        if(m_PioneerInputButtons[i] != found)
                            m_PioneerInputButtons[i]->setChecked(false);
                    }
                }
                i++;
            }
        }
        else
        {
            foreach (QPushButton* button, m_OnkyoInputButtons)
            {
                if (button != NULL)
                {
                    int key = m_OnkyoInputButtons.key(button);
                    button->setChecked(key == idx);
                }
            }
        }
    }
    // if it is a net input, open NetRadio window, otherwise close it
    if ((m_SelectedInput == ui->InputNetButton && m_PowerOn)
            || (m_Zone2PowerOn && m_SelectedInputZ2 == ui->InputNetButton)
            || (m_Zone3PowerOn && m_SelectedInputZ3 == ui->InputNetButton))
    {
        if (m_IsPioneer)
        {
            m_NetRadioDialog->ShowNetDialog(true);
        }
        else
        {
            m_NetOnkyoDialog->ShowNetDialog(true);
        }
    }
    else
    {
        if (m_NetRadioDialog->isVisible())
        {
            m_NetRadioDialog->hide();
        }
        if (m_NetOnkyoDialog->isVisible())
        {
            m_NetOnkyoDialog->hide();
        }
    }

    // if it is the tuner input, open Tuner window, otherwise close it
    if (m_SelectedInput == ui->InputTunerButton || (m_Zone2PowerOn && m_SelectedInputZ2 == ui->InputTunerButton) || (m_Zone3PowerOn && m_SelectedInputZ3 == ui->InputTunerButton))
    {
        m_TunerDialog->ShowTunerDialog(true);
    }
    else
    {
        if (m_TunerDialog->isVisible())
            m_TunerDialog->hide();
    }

    // if it is the tuner input, open Tuner window, otherwise close it
    if (m_SelectedInput == ui->InputIpodButton || (m_Zone2PowerOn && m_SelectedInputZ2 == ui->InputIpodButton) || (m_Zone3PowerOn && m_SelectedInputZ3 == ui->InputIpodButton))
    {
        m_usbDialog->ShowusbDialog(true);
    }
    else
    {
        if (m_usbDialog->isVisible())
            m_usbDialog->hide();
    }

    if (m_SelectedInput == ui->InputBdButton || (m_Zone2PowerOn && m_SelectedInputZ2 == ui->InputBdButton) || (m_Zone3PowerOn && m_SelectedInputZ3 == ui->InputBdButton))
    {
        m_BluRayDialog->ShowBluRayDialog(true);
    }
    else
    {
        if (m_BluRayDialog->isVisible())
            m_BluRayDialog->hide();
    }

}

//void AVRPioRemote::SocketStateChanged(QAbstractSocket::SocketState State)
//{
//    Log(QString("SocketState $1").arg(State));
//}

//ConnectWorker::ConnectWorker(QTcpSocket& socket, QString &address, int port, QThread* mainThread):
//    Socket(socket),
//    Address(address),
//    Port(port),
//    MainThread(*mainThread)
//{
//}

//ConnectWorker::~ConnectWorker() {
//}

//void ConnectWorker::process() {
//    // allocate resources using new here
//    //qDebug("Hello World!");
//    if (Socket.state() != QAbstractSocket::HostLookupState && Socket.state() != QAbstractSocket::ConnectingState)
//        Socket.connectToHost(Address, Port);
//    Socket.moveToThread(&MainThread);
//    emit finished();
//}

void AVRPioRemote::LMSelectedAction(QString Param)
{
    if (m_IsPioneer)
    {
        QString cmd = QString("%1SR").arg(Param);
        SendCmd(cmd);
    }
}

void AVRPioRemote::ConnectReceiver()
{
    ui->pushButtonConnect->setEnabled(false);
    m_SettingsDialog->EnableIPInput(false);

    ui->StatusLineEdit->setText(tr("Connecting..."));
    m_StatusLineTimer.start();

    if (!m_ReceiverInterface.IsConnected())
    {
        m_ReceiverInterface.ConnectToReceiver(m_IpAddress, m_IpPort, m_IsPioneer);
    }

}
void AVRPioRemote::ConnectPlayer()
{
    m_BluRayDialog->show();
    ui->StatusLineEdit->setText(tr("Connecting player..."));
    m_StatusLineTimer.start();
    m_BluRayDialog->onConnect();
}

//void AVRPioRemote::onConnectWorkerFinished()
//{
//    Log("Worker finished");
//    ui->pushButtonConnect->setEnabled(true);
//}

//void AVRPioRemote::ConnectWorkerErrorString(QString err)
//{
//    ui->pushButtonConnect->setEnabled(true);
//    Log(err);
//}


void AVRPioRemote::NewDataReceived(const QString& data, bool is_pioneer)
{
    Logger::Log("<-- " + data);
}

void AVRPioRemote::ResponseReceived(ReceivedObjectBase *response)
{
    DisplayDataResponse_FL* display = dynamic_cast<DisplayDataResponse_FL*>(response);
    if (display != NULL)
    {
        ui->ScreenLineEdit1->setText(display->getDisplayLine());
        return;
    }
    InputFunctionResponse_FN* inputFunction = dynamic_cast<InputFunctionResponse_FN*>(response);
    if (inputFunction != NULL)
    {
        InputChanged(inputFunction->getNumber(), inputFunction->getName());
        return;
    }
    PowerResponse_PWR_APR_BPR_ZEP* power = dynamic_cast<PowerResponse_PWR_APR_BPR_ZEP*>(response);
    if (power != NULL)
    {
        if (power->GetZone() == PowerResponse_PWR_APR_BPR_ZEP::ZoneMain)
        {
            //RequestStatus(true);
            if (m_IsPioneer)
            {
                SendCmd("?F");
            }
            m_PowerOn = power->IsPoweredOn();
            ui->PowerButton->setIcon((m_PowerOn) ? m_PowerButtonOffIcon : m_PowerButtonOnIcon);
            //ui->PowerButton->setText((!m_PowerOn) ? tr("ON") : tr("OFF"));
            EnableControls(m_PowerOn);
            m_ReceiverOnline = m_PowerOn;
            if (m_PowerOn) {
                m_RefreshTimer.start();
            }
            else {
                m_RefreshTimer.stop();
                m_InfoDialog->Reset();
                m_InputLSConfiguration->reset();
                m_OutputLSConfiguration->reset();
            }
        }
        else if (power->GetZone() == PowerResponse_PWR_APR_BPR_ZEP::Zone2)
        {
            RequestStatus(false);
            m_Zone2PowerOn = power->IsPoweredOn();
            if (!m_Zone2PowerOn)
                m_SelectedInputZ2 = NULL;
        }
        else if (power->GetZone() == PowerResponse_PWR_APR_BPR_ZEP::Zone3)
        {
            RequestStatus(false);
            m_Zone3PowerOn = power->IsPoweredOn();
            if (!m_Zone3PowerOn)
                m_SelectedInputZ3 = NULL;
        }
        return;
    }
    HDMIPassThroughResponse_STU* pass_through = dynamic_cast<HDMIPassThroughResponse_STU*>(response);
    if (pass_through != NULL)
    {
        m_PassThroughLast = (pass_through->GetPassThroughFunction() == HDMIPassThroughResponse_STU::PassThroughLast);
        EnableControls(m_PowerOn);
        return;
    }
    VolumeResponse_VOL_ZV_YV* volume = dynamic_cast<VolumeResponse_VOL_ZV_YV*>(response);
    if (volume != NULL)
    {
        if (volume->GetZone() == VolumeResponse_VOL_ZV_YV::ZoneMain)
        {
            ui->VolumeLineEdit->setText(volume->GetAsString());
        }
        return;
    }
    MuteResponse_MUT_Z2MUT_Z3MUT* mute = dynamic_cast<MuteResponse_MUT_Z2MUT_Z3MUT*>(response);
    if (mute != NULL)
    {
        if (mute->GetZone() == MuteResponse_MUT_Z2MUT_Z3MUT::ZoneMain)
            ui->VolumeMuteButton->setChecked(mute->IsMuted());
        return;
    }
    InputNameResponse_RGB* inputname = dynamic_cast<InputNameResponse_RGB*>(response);
    if (inputname != NULL)
    {
        if (!m_Settings.value("ShowDefaultInputName", false).toBool())
            ui->InputNameLineEdit->setText(inputname->GetInputName());
        else
            ui->InputNameLineEdit->setToolTip(inputname->GetInputName());
    }
    ErrorResponse_B_E* error = dynamic_cast<ErrorResponse_B_E*>(response);
    if (error != NULL)
    {
        switch(error->GetError())
        {
        case ErrorResponse_B_E::ErrorDoesntWorkNow:
            Logger::Log("This doesn't work now");
            ui->StatusLineEdit->setText(tr("This doesn't work now"));
            break;
        case ErrorResponse_B_E::ErrorNotSupported:
            Logger::Log("This doesn't work with this receiver");
            ui->StatusLineEdit->setText(tr("This doesn't work with this receiver"));
            break;
        case ErrorResponse_B_E::ErrorCommand:
            Logger::Log("Command error");
            ui->StatusLineEdit->setText(tr("Command error"));
            break;
        case ErrorResponse_B_E::ErrorParameter:
            Logger::Log("Parameter error");
            ui->StatusLineEdit->setText(tr("Parameter error"));
            break;
        case ErrorResponse_B_E::ErrorBusy:
            Logger::Log("Receiver busy");
            ui->StatusLineEdit->setText(tr("Receiver busy"));
            break;
        default:
            Logger::Log("Unknown error");
            ui->StatusLineEdit->setText(tr("Unknown error"));
            break;
        }
        m_StatusLineTimer.start();
        return;
    }
    PhaseControlResponse_IS* phase = dynamic_cast<PhaseControlResponse_IS*>(response);
    if (phase != NULL)
    {
        ui->PhaseButton->setChecked(phase->IsPhaseControlOn());
        return;
    }
    HiBitResponse_ATI* hibit = dynamic_cast<HiBitResponse_ATI*>(response);
    if (hibit != NULL)
    {
        ui->HiBitButton->setChecked(hibit->IsHiBitOn());
        return;
    }
    PQLSControlResponse_PQ* pqls = dynamic_cast<PQLSControlResponse_PQ*>(response);
    if (pqls != NULL)
    {
        ui->PqlsButton->setChecked(pqls->IsPQLSControlOn());
        return;
    }
    SoundRetrieverResponse_ATA* sretr = dynamic_cast<SoundRetrieverResponse_ATA*>(response);
    if (sretr != NULL)
    {
        ui->SRetrButton->setChecked(sretr->IsSoundRetrieverOn());
        return;
    }
    // AST
    AudioStatusDataResponse_AST* ast = dynamic_cast<AudioStatusDataResponse_AST*>(response);
    if (ast != NULL)
    {
        ui->AudioCodecLineEdit->setText(ast->codec);
        ui->AudioSampleRateLineEdit->setText(ast->samplingRate);

        m_InputLSConfiguration->NewData(ast->iChFormat);
        m_OutputLSConfiguration->NewData(ast->oChFormat);
        return;
    }
    // AUB
    Response_AUB* aub = dynamic_cast<Response_AUB*>(response);
    if (aub != NULL)
    {
        ui->HiBitButton->setChecked(aub->GetHiBit());
        return;
    }

}

void AVRPioRemote::InputChanged(int no, QString name)
{
    if (m_Settings.value("ShowDefaultInputName", false).toBool())
        ui->InputNameLineEdit->setText(name);
    else
        ui->InputNameLineEdit->setToolTip(name);
    SelectInputButton(no);
    if (m_IsPioneer)
    {
        QString str = QString("?RGB%1").arg(no, 2, 10, QLatin1Char('0'));
        SendCmd(str.toLocal8Bit());
    }
    RequestStatus(false);
}


void AVRPioRemote::ZoneInput (int zone, int input)
{
    SelectInputButton(input, zone);
}


void AVRPioRemote::ListeningModeData(QString name)
{
    ui->ListeningModeLineEdit->setText(name);
}


void AVRPioRemote::RequestStatus(bool input)
{
    if (m_ReceiverOnline)
    {
        if (m_IsPioneer)
        {
            bool isVSX5XX = m_Settings.value("VSX5xxCompatibilityMode", false).toBool();
            SendCmd("?V"); // volume
            SendCmd("?M"); // mute
            if (!isVSX5XX)
                SendCmd("?FL"); // request display
            SendCmd("?AST"); // request audio information
            SendCmd("?VST"); // request video information
            SendCmd("?S"); // request listening mode
            SendCmd("?L"); // request playing listening mode
            SendCmd("?TO"); // request Tone
            SendCmd("?BA"); // request Bass
            SendCmd("?TR"); // request Treble
            if (input)
                SendCmd("?F"); // request input
            if (!isVSX5XX) {
                SendCmd("?IS"); // phase control
                SendCmd("?ATI"); // Hi-Bit
                SendCmd("?ATA"); // S.Retriever
                SendCmd("?PQ"); // PQLS
            }
            SendCmd("?AUB"); // new data (only HiBit flag known)
        }
        else
        {
            SendCmd("IFAQSTN"); // request audio information
            //SendCmd("IFVQSTN"); // request video information
        }
    }
    //sendCmd("?RGB**"); // request input name information
}

void AVRPioRemote::CommError(QString socketError)
{
    Logger::Log("tcp error");
    m_Connected = false;
    ui->pushButtonConnect->setEnabled(true);
    m_SettingsDialog->EnableIPInput(true);
    EnableControls(false);
    ui->pushButtonConnect->setText(tr("Connect"));
    ClearScreen();
    ui->StatusLineEdit->setText(socketError);
    m_StatusLineTimer.start();
    m_RefreshTimer.stop();
    ui->ZoneControlButton->setEnabled(false);
    m_InfoDialog->Reset();
    m_InputLSConfiguration->reset();
    m_OutputLSConfiguration->reset();
}

void AVRPioRemote::CommConnected()
{
    Logger::Log("connected");
    m_Connected = true;
    ui->PowerButton->setEnabled(true);
    ui->StatusLineEdit->setText(tr("Connected"));
    m_StatusLineTimer.start();
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonConnect->setText(tr("Disconnect"));
    ui->pushButtonConnect->setChecked(true);
    ui->ZoneControlButton->setEnabled(true);
    m_ReceiverOnline = true;
    if (m_IsPioneer)
    {
        SendCmd("?RGD"); // Receiver-Kennung
        SendCmd("?SSO"); // Receiver friendly name (network)
        //SendCmd("?STU"); // PassThrough
        SendCmd("?P"); // power
        SendCmd("?F"); // input
    }
    else
    {
        SendCmd("PWRQSTN"); // power
        SendCmd("AMTQSTN"); // mute
        SendCmd("MVLQSTN"); // volume
        SendCmd("SLIQSTN"); // input
        SendCmd("FLDQSTN"); // display
        SendCmd("PCTQSTN"); // phase control
        SendCmd("PCPQSTN"); // phase control plus
        SendCmd("HBTQSTN"); // hi-bit
        SendCmd("PQLQSTN"); // pqls
        SendCmd("LMDQSTN"); // listening mode
        SendCmd("IFAQSTN"); // request audio information
    }
}

void AVRPioRemote::CommDisconnected()
{
    Logger::Log("disconnected");
    m_Connected = false;
    m_SettingsDialog->EnableIPInput(true);
    ui->pushButtonConnect->setText(tr("Connect"));
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonConnect->setChecked(false);
    EnableControls(false);
    ClearScreen();
    ui->ZoneControlButton->setEnabled(false);
    m_RefreshTimer.stop();
    m_InfoDialog->Reset();
    m_InputLSConfiguration->reset();
    m_OutputLSConfiguration->reset();
}

bool AVRPioRemote::SendCmd(const QString& cmd)
{
    m_ReceiverInterface.SendCmd(cmd);
    return true;
}

void AVRPioRemote::EnableControls(bool enable)
{
    if (m_IsPioneer)
    {
        ui->SRetrButton->setEnabled(enable);
        ui->HiBitButton->setEnabled(enable);

        bool enableInputs = false;
        if ((m_Connected && m_PassThroughLast) || enable)
            enableInputs = true;

        ui->InputAdptButton->setEnabled(enableInputs);
        ui->InputBdButton->setEnabled(enableInputs);
        ui->InputCdButton->setEnabled(enableInputs);
        ui->InputDvdButton->setEnabled(enableInputs);
        ui->InputDvrButton->setEnabled(enableInputs);
        ui->InputHdmiButton->setEnabled(enableInputs);
        ui->InputIpodButton->setEnabled(enableInputs);
        ui->InputNetButton->setEnabled(enableInputs);
        ui->InputSatButton->setEnabled(enableInputs);
        ui->InputTunerButton->setEnabled(enableInputs);
        ui->InputTvButton->setEnabled(enableInputs);
        ui->InputVideoButton->setEnabled(enableInputs);

        ui->InputLeftButton->setEnabled(enable);
        ui->InputRightButton->setEnabled(enable);
        ui->AutoAlcDirectButton->setEnabled(enable);
        ui->StandardButton->setEnabled(enable);

        ui->AdvSurrButton->setEnabled(enable);
        ui->PhaseButton->setEnabled(enable);
        ui->PqlsButton->setEnabled(enable);
        ui->VolumeDownButton->setEnabled(enable);
        ui->VolumeMuteButton->setEnabled(enable);
        ui->VolumeUpButton->setEnabled(enable);
        ui->ShowAllListeningModesButton->setEnabled(enable);
        ui->ATBEQModesButton->setEnabled(enable);
        ui->InfoButton->setEnabled(enable);
    }
    else
    {
        ui->SRetrButton->setEnabled(enable);
        ui->HiBitButton->setEnabled(enable);

        bool enableInputs = false;
        if ((m_Connected && m_PassThroughLast) || enable)
            enableInputs = true;

//        ui->InputAdptButton->setEnabled(enableInputs);
        ui->InputBdButton->setEnabled(enableInputs);
        ui->InputCdButton->setEnabled(enableInputs);
        ui->InputDvdButton->setEnabled(enableInputs);
//        ui->InputDvrButton->setEnabled(enableInputs);
        ui->InputHdmiButton->setEnabled(enableInputs);
        ui->InputIpodButton->setEnabled(enableInputs);
        ui->InputNetButton->setEnabled(enableInputs);
        ui->InputSatButton->setEnabled(enableInputs);
        ui->InputTunerButton->setEnabled(enableInputs);
        ui->InputTvButton->setEnabled(enableInputs);
//        ui->InputVideoButton->setEnabled(enableInputs);

        ui->InputLeftButton->setEnabled(enable);
        ui->InputRightButton->setEnabled(enable);
        ui->AutoAlcDirectButton->setEnabled(enable);
        ui->StandardButton->setEnabled(enable);

        ui->AdvSurrButton->setEnabled(enable);
        ui->PhaseButton->setEnabled(enable);
        ui->PqlsButton->setEnabled(enable);
        ui->VolumeDownButton->setEnabled(enable);
        ui->VolumeMuteButton->setEnabled(enable);
        ui->VolumeUpButton->setEnabled(enable);
//        ui->ShowAllListeningModesButton->setEnabled(enable);
        ui->ATBEQModesButton->setEnabled(enable);
        ui->InfoButton->setEnabled(enable);
    }
}


void AVRPioRemote::ClearScreen()
{
    ui->ScreenLineEdit1->setText("");
    ui->ListeningModeLineEdit->setText("");
    ui->AudioCodecLineEdit->setText("");
    ui->AudioSampleRateLineEdit->setText("");
    ui->VolumeLineEdit->setText("");
    ui->InputNameLineEdit->setText("");
}

void AVRPioRemote::onConnectBD()
{
    ConnectPlayer();
}

void AVRPioRemote::onConnect()
{
    if (!m_ReceiverInterface.IsConnected())
    {
        // connect
        // read settings from the line edits
        QString ip1, ip2, ip3, ip4, ip_port;
        bool is_pioneer;
        // first the 4 ip address blocks
        m_SettingsDialog->GetIpAddress(ip1, ip2, ip3, ip4, ip_port, is_pioneer);
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
        m_IpAddress = ip1 + "." + ip2 + "." + ip3 + "." + ip4;
        // and then th ip port
        if (ip_port == "")
        {
            ip_port = "8102"; // set default
            m_IpPort = 8102;
            m_IsPioneer = true;
        }
        else
        {
            m_IpPort = ip_port.toInt();
            m_IsPioneer = is_pioneer;
        }
        // save the ip address and port permanently
        m_SettingsDialog->SetIpAddress(ip1, ip2, ip3, ip4, ip_port, m_IsPioneer);
        m_Settings.setValue("IP/1", ip1);
        m_Settings.setValue("IP/2", ip2);
        m_Settings.setValue("IP/3", ip3);
        m_Settings.setValue("IP/4", ip4);
        m_Settings.setValue("IP/PORT", ip_port);
        m_Settings.setValue("IP/IsPioneer", m_IsPioneer);
        ConnectReceiver();
    }
    else
    {
        // disconnect
        EnableControls(false);
        ui->PowerButton->setEnabled(false);
        m_ReceiverInterface.Disconnect();
        m_ReceiverOnline = false;
    }
}


void AVRPioRemote::ShowAboutDialog()
{
    AboutDialog* about = new AboutDialog(this);
    about->show();
}


void AVRPioRemote::on_MoreButton_clicked()
{
    QAction* pAction;
    QMenu MyMenu(this);
    //MyMenu.addActions(this->actions());

    if (m_ReceiverOnline == true)
    {
        if (m_IsPioneer)
        {
            pAction = new QAction(tr("Internet Radio"), this);
            MyMenu.addAction(pAction);
            connect(pAction, SIGNAL(triggered()), m_NetRadioDialog, SLOT(ManualShowNetDialog()));

            pAction = new QAction(tr("Tuner"), this);
            MyMenu.addAction(pAction);
            connect(pAction, SIGNAL(triggered()), m_TunerDialog, SLOT(ManualShowTunerDialog()));

            pAction = new QAction(tr("IPod / USB"), this);
            MyMenu.addAction(pAction);
            connect(pAction, SIGNAL(triggered()), m_usbDialog, SLOT(ManualShowusbDialog()));

            pAction = new QAction(tr("Equalizer / Tone"), this);
            MyMenu.addAction(pAction);
            connect(pAction, SIGNAL(triggered()), m_EQDialog, SLOT(ShowEQDialog()));

            pAction = new QAction(tr("Speaker Settings"), this);
            MyMenu.addAction(pAction);
            connect(pAction, SIGNAL(triggered()), m_LoudspeakerSettingsDialog, SLOT(ShowLoudspeakerSettingsDialog()));

            pAction = new QAction(tr("ListenMode Settings"), this);
            MyMenu.addAction(pAction);
            connect(pAction, SIGNAL(triggered()), m_Listendiag, SLOT(ShowListeningDialog()));

            pAction = new QAction(tr("Audio / Video Settings"), this);
            MyMenu.addAction(pAction);
            connect(pAction, SIGNAL(triggered()), m_AVSettingsDialog, SLOT(ShowAVSettingsDialog()));

            pAction = new QAction(tr("MCACC Equalizer"), this);
            MyMenu.addAction(pAction);
            connect(pAction, SIGNAL(triggered()), m_MCACCEQDialog, SLOT(ShowMCACCEQDialog()));

            pAction = new QAction(tr("HDMI Control"), this);
            MyMenu.addAction(pAction);
            connect(pAction, SIGNAL(triggered()), m_HdmiControlDialog, SLOT(ShowHdmiControlDialog()));

            pAction = new QAction(tr("MCACC Measuring Progress"), this);
            MyMenu.addAction(pAction);
            connect(pAction, SIGNAL(triggered()), m_MCACCProgressDialog, SLOT(ShowMCACCProgressDialog()));
        }
        else
        {
            pAction = new QAction(tr("Internet Radio"), this);
            MyMenu.addAction(pAction);
            connect(pAction, SIGNAL(triggered()), m_NetOnkyoDialog, SLOT(ManualShowNetDialog()));

//            pAction = new QAction(tr("Tuner"), this);
//            MyMenu.addAction(pAction);
//            connect(pAction, SIGNAL(triggered()), m_TunerDialog, SLOT(ManualShowTunerDialog()));

//            pAction = new QAction(tr("IPod / USB"), this);
//            MyMenu.addAction(pAction);
//            connect(pAction, SIGNAL(triggered()), m_usbDialog, SLOT(ManualShowusbDialog()));

            pAction = new QAction(tr("Equalizer / Tone"), this);
            MyMenu.addAction(pAction);
            connect(pAction, SIGNAL(triggered()), m_EQDialog, SLOT(ShowEQDialog()));

            pAction = new QAction(tr("Speaker Settings"), this);
            MyMenu.addAction(pAction);
            connect(pAction, SIGNAL(triggered()), m_LoudspeakerSettingsOnkyoDialog, SLOT(ShowLoudspeakerSettingsOnkyoDialog()));

//            pAction = new QAction(tr("ListenMode Settings"), this);
//            MyMenu.addAction(pAction);
//            connect(pAction, SIGNAL(triggered()), m_Listendiag, SLOT(ShowListeningDialog()));

//            pAction = new QAction(tr("Audio / Video Settings"), this);
//            MyMenu.addAction(pAction);
//            connect(pAction, SIGNAL(triggered()), m_AVSettingsDialog, SLOT(ShowAVSettingsDialog()));

//            pAction = new QAction(tr("MCACC Equalizer"), this);
//            MyMenu.addAction(pAction);
//            connect(pAction, SIGNAL(triggered()), m_MCACCEQDialog, SLOT(ShowMCACCEQDialog()));

//            pAction = new QAction(tr("HDMI Control"), this);
//            MyMenu.addAction(pAction);
//            connect(pAction, SIGNAL(triggered()), m_HdmiControlDialog, SLOT(ShowHdmiControlDialog()));

//            pAction = new QAction(tr("MCACC Measuring Progress"), this);
//            MyMenu.addAction(pAction);
//            connect(pAction, SIGNAL(triggered()), m_MCACCProgressDialog, SLOT(ShowMCACCProgressDialog()));
        }

        //        pAction = new QAction(tr("Wiring Wizard"), this);
        //        MyMenu.addAction(pAction);
        //        connect(pAction, SIGNAL(triggered()), m_WiringDialog, SLOT(ShowWiringDialog()));

        pAction = new QAction(tr("Refresh status"), this);
        MyMenu.addAction(pAction);
        connect(pAction, SIGNAL(triggered()), this, SLOT(RequestStatus()));

        //        pAction = new QAction("More Information", this);
        //        pAction = new QAction("Equalizer", this);
        //        pAction = new QAction("LS Settings", this);
        //        pAction = new QAction("Show Zone 2", this);
        //        pAction = new QAction("Input Wizard", this);
        MyMenu.addSeparator();
        //        pAction = new QAction(tr("-"), this);
        //        pAction->setSeparator(true);
        //        MyMenu.addAction(pAction);
    }

    pAction = new QAction(tr("Test"), this);
    MyMenu.addAction(pAction);
    connect(pAction, SIGNAL(triggered()), m_TestDialog, SLOT(ShowTestDialog()));


    pAction = new QAction(tr("Settings"), this);
    MyMenu.addAction(pAction);
    connect(pAction, SIGNAL(triggered()), m_SettingsDialog, SLOT(ShowSettingsDialog()));

    //    pAction = new QAction(tr("-"), this);
    //    pAction->setSeparator(true);
    //    MyMenu.addAction(pAction);
    MyMenu.addSeparator();

    pAction = new QAction(tr("Blu-Ray"), this);
    MyMenu.addAction(pAction);
    connect(pAction, SIGNAL(triggered()), m_BluRayDialog, SLOT(ManualShowBluRayDialog()));

    pAction = new QAction(tr("Blu-Ray Test"), this);
    MyMenu.addAction(pAction);
    connect(pAction, SIGNAL(triggered()), m_PlayerTestDialog, SLOT(ShowTestDialog()));
    /*
    pAction = new QAction(tr("Information"), this);
    pAction->setSeparator(true);
    MyMenu.addAction(pAction);*/
    MyMenu.addSeparator();
    pAction = new QAction(tr("About AVRPioRemote"), this);
    MyMenu.addAction(pAction);
    connect(pAction, SIGNAL(triggered()), this, SLOT(ShowAboutDialog()));


    QPoint pos = QCursor::pos();
    MyMenu.exec(pos);
}

void AVRPioRemote::on_VolumeUpButton_clicked()
{
    if (m_IsPioneer)
    {
        SendCmd("VU");
    }
    else
    {
        SendCmd("MVLUP");
    }
}

void AVRPioRemote::on_VolumeDownButton_clicked()
{
    if (m_IsPioneer)
    {
        SendCmd("VD");
    }
    else
    {
        SendCmd("MVLDOWN");
    }
}

void AVRPioRemote::on_VolumeMuteButton_clicked()
{
    if (m_IsPioneer)
    {
        if (ui->VolumeMuteButton->isChecked())
            SendCmd("MO");
        else
            SendCmd("MF");
    }
    else
    {
        if (ui->VolumeMuteButton->isChecked())
            SendCmd("AMT01");
        else
            SendCmd("AMT00");
    }
}

void AVRPioRemote::on_pushButtonConnect_clicked()
{
    ui->pushButtonConnect->setChecked(!ui->pushButtonConnect->isChecked());
    onConnect();
}


void AVRPioRemote::on_PowerButton_clicked()
{
    //ui->PowerButton->setChecked(!ui->PowerButton->isChecked());
    if (!m_PowerOn)
    {
        if (m_IsPioneer)
        {
            SendCmd("PO");
            SendCmd("PO");
        }
        else
        {
            SendCmd("PWR01");
        }
    }
    else
    {
        if (m_IsPioneer)
        {
            SendCmd("PF");
            SendCmd("PF");
        }
        else
        {
            SendCmd("PWR00");
        }
        m_ReceiverOnline = false;
    }
}


void AVRPioRemote::on_InputLeftButton_clicked()
{
    if (m_IsPioneer)
    {
        SendCmd("FD");
    }
    else
    {
        SendCmd("SLIDOWN");
    }
}

void AVRPioRemote::on_InputRightButton_clicked()
{
    if (m_IsPioneer)
    {
        SendCmd("FU");
    }
    else
    {
        SendCmd("SLIDUP");
    }
}

void AVRPioRemote::on_PhaseButton_clicked()
{
    ui->PhaseButton->setChecked(!ui->PhaseButton->isChecked());
    //SendCmd("9IS");
    if (ui->PhaseButton->isChecked())
    {
        if (m_IsPioneer)
        {
            SendCmd("0IS");
        }
        else
        {
            SendCmd("PCT00");
        }
    }
    else
    {
        if (m_IsPioneer)
        {
            SendCmd("1IS");
        }
        else
        {
            SendCmd("PCT01");
        }
    }
}

void AVRPioRemote::on_PqlsButton_clicked()
{
    ui->PqlsButton->setChecked(!ui->PqlsButton->isChecked());
    //SendCmd("9PQ");
    if (ui->PqlsButton->isChecked())
    {
        if (m_IsPioneer)
        {
            SendCmd("0PQ");
        }
        else
        {
            SendCmd("PQL00");
        }
    }
    else
    {
        if (m_IsPioneer)
        {
            SendCmd("1PQ");
        }
        else
        {
            SendCmd("PQL01");
        }
    }
}

void AVRPioRemote::on_SRetrButton_clicked()
{
    ui->SRetrButton->setChecked(!ui->SRetrButton->isChecked());
    //SendCmd("9ATV");
    if (ui->SRetrButton->isChecked())
    {
        if (m_IsPioneer)
        {
            SendCmd("0ATA");
        }
        else
        {
            SendCmd("MOT00");
        }
    }
    else
    {
        if (m_IsPioneer)
        {
            SendCmd("1ATA");
        }
        else
        {
            SendCmd("MOT01");
        }
    }
}

void AVRPioRemote::on_HiBitButton_clicked()
{
    ui->HiBitButton->setChecked(!ui->HiBitButton->isChecked());
    //SendCmd("9ATI");
    if (ui->HiBitButton->isChecked())
    {
        if (m_IsPioneer)
        {
            SendCmd("0ATI");
        }
        else
        {
            SendCmd("HBT00");
        }
    }
    else
    {
        if (m_IsPioneer)
        {
            SendCmd("1ATI");
        }
        else
        {
            SendCmd("HBT01");
        }
    }
}

void AVRPioRemote::on_InputBdButton_clicked()
{
    ui->InputBdButton->setChecked(!ui->InputBdButton->isChecked());
    if (m_IsPioneer)
    {
        SendCmd("25FN");
    }
    else
    {
        SendCmd("SLI10");
    }
}

void AVRPioRemote::on_InputDvdButton_clicked()
{
    ui->InputDvdButton->setChecked(!ui->InputDvdButton->isChecked());
    if (m_IsPioneer)
    {
        SendCmd("04FN");
    }
    else
    {
        SendCmd("SLI02");
    }
}

void AVRPioRemote::on_InputDvrButton_clicked()
{
    ui->InputDvrButton->setChecked(!ui->InputDvrButton->isChecked());
    if (m_IsPioneer)
    {
        SendCmd("15FN");
    }
}

void AVRPioRemote::on_InputTvButton_clicked()
{
    ui->InputTvButton->setChecked(!ui->InputTvButton->isChecked());
    if (m_IsPioneer)
    {
        SendCmd("05FN");
    }
    else
    {
        SendCmd("SLI12");
    }
}

void AVRPioRemote::on_InputCdButton_clicked()
{
    ui->InputCdButton->setChecked(!ui->InputCdButton->isChecked());
    if (m_IsPioneer)
    {
        SendCmd("01FN");
    }
    else
    {
        SendCmd("SLI23");
    }
}

void AVRPioRemote::on_InputIpodButton_clicked()
{
    m_usbDialog->ShowusbDialog(true);
    ui->InputIpodButton->setChecked(!ui->InputIpodButton->isChecked());
    if (m_IsPioneer)
    {
        SendCmd("17FN");
    }
    else
    {
        SendCmd("SLI29");
    }
}

void AVRPioRemote::on_InputSatButton_clicked()
{
    ui->InputSatButton->setChecked(!ui->InputSatButton->isChecked());
    if (m_IsPioneer)
    {
        SendCmd("06FN");
    }
}

void AVRPioRemote::on_InputAdptButton_clicked()
{
    ui->InputAdptButton->setChecked(!ui->InputAdptButton->isChecked());
    if (m_IsPioneer)
    {
        SendCmd("33FN");
    }
    else
    {
        SendCmd("SLI03");
    }
}

void AVRPioRemote::on_InputHdmiButton_clicked()
{
    ui->InputHdmiButton->setChecked(!ui->InputHdmiButton->isChecked());
    if (m_IsPioneer)
    {
        SendCmd("31FN");
    }
}

void AVRPioRemote::on_InputNetButton_clicked()
{
    ui->InputNetButton->setChecked(!ui->InputNetButton->isChecked());
    if (!m_Settings.value("VSX5xxCompatibilityMode", false).toBool())
    {
        if (m_IsPioneer)
        {
            SendCmd("26FN");
        }
        else
        {
            SendCmd("SLI2B");
        }
    }
    else
    {
        if (m_IsPioneer)
        {
            SendCmd("38FN");
        }
        else
        {
            SendCmd("SLI2B");
        }
    }
}

void AVRPioRemote::on_InputTunerButton_clicked()
{
    ui->InputTunerButton->setChecked(!ui->InputTunerButton->isChecked());
    if (m_IsPioneer)
    {
        m_TunerDialog->ShowTunerDialog(true);
        SendCmd("02FN");
    }
    else
    {
        SendCmd("SLI26");
    }
}

void AVRPioRemote::on_AutoAlcDirectButton_clicked()
{
    //ui->InputTunerButton->setChecked(!ui->InputTunerButton->isChecked());
    if (m_IsPioneer)
    {
        SendCmd("0005SR");
    }
    else
    {
        SendCmd("LMDAUTO");
    }
}

void AVRPioRemote::on_StandardButton_clicked()
{
    //ui->InputTunerButton->setChecked(!ui->InputTunerButton->isChecked());
    if (m_IsPioneer)
    {
        SendCmd("0010SR");
    }
    else
    {
        SendCmd("LMDSTEREO");
    }
}

void AVRPioRemote::on_AdvSurrButton_clicked()
{
    //ui->InputTunerButton->setChecked(!ui->InputTunerButton->isChecked());
    if (m_IsPioneer)
    {
        SendCmd("0100SR");
    }
    else
    {
        SendCmd("LMDSURR");
    }
}

void AVRPioRemote::on_ShowAllListeningModesButton_clicked()
{
    m_Listendiag->ShowListeningDialog();
    /*
    ActionWithParameter* pAction;
    QMenu MyMenu(this);
    //MyMenu.addActions(this->actions());
    if (m_ReceiverOnline == true)
    {
        // add ALL listening mode keys to the menu
        int i = 0;


        while (strlen(LISTENING_MODE[i].key) != 0)
        {
            pAction = new ActionWithParameter(this, LISTENING_MODE[i].text, LISTENING_MODE[i].key);
            connect(pAction, SIGNAL(ActionTriggered(QString)), this, SLOT(LMSelectedAction(QString)));
            MyMenu.addAction(pAction);
            i++;
            if (LISTENING_MODE[i].key=="0050" && m_Settings.value("TunerCompatibilityMode").toBool())
            {
                  //für 922 kompatible nur deren gültige werte anzeigen
                MyMenu.addSeparator();
                MyMenu.addAction("ESC=Beenden");
                break;
            }
        }
    }

    QPoint pos = QCursor::pos();
    if ( m_Settings.value("TunerCompatibilityMode").toBool())
    {
        int i=1;
        while (i!=0)
            i=int(MyMenu.exec(pos));
    }
    else
        MyMenu.exec(pos);
        */
}

void AVRPioRemote::on_InputVideoButton_clicked()
{
    ui->InputVideoButton->setChecked(!ui->InputVideoButton->isChecked());
    if (m_IsPioneer)
    {
        SendCmd("10FN");
    }
    else
    {
        SendCmd("SLI11"); //streamer box
    }
}

void AVRPioRemote::on_ATBEQModesButton_clicked()
{
    m_EQDialog->ShowEQDialog();
}

void AVRPioRemote::ReceiverType (QString/* no*/, QString name)
{
    if(m_Settings.value("ShowReceiverNameInTitle", true).toBool())
    {
        this->setWindowTitle(name);
    }
}


void AVRPioRemote::ReceiverNetworkName (QString/* name*/)
{
}


void AVRPioRemote::on_ZoneControlButton_clicked()
{
    m_ZoneControlDialog->ShowZoneControlDialog();
}

void AVRPioRemote::on_InfoButton_clicked()
{
    m_InfoDialog->ShowInfoDialog();
}

