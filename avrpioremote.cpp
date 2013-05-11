#include "avrpioremote.h"
#include "ui_avrpioremote.h"
#include <QDebug>
//#include <QLayout>
#include <qtextcodec.h>
#include "actionwithparameter.h"
#include <QWidget>

AVRPioRemote::AVRPioRemote(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AVRPioRemote),
    m_IpValidator(0, 255, this),
    m_IpPortValidator(0, 35535, this),
    m_Settings(QSettings::IniFormat, QSettings::UserScope, "AVRPIO", "AVRPioRemote"),
    m_FavoritesCompatibilityMode(false),
    m_StatusLineTimer(this)
{
    m_IpPort = 8102;
    m_ReceiverOnline = false;

    QString lang = m_Settings.value("Language", "auto").toString();
    if (lang == "auto")
    {
        lang = QLocale::system().name();
    }
    if (lang.startsWith("de"))
    {
        m_Translater.load("avrpioremote_de");
    }
    else
    {
        m_Translater.load("avrpioremote_en");
    }
    QCoreApplication::installTranslator(&m_Translater);
    ui->setupUi(this);
    // add minimize button to to title
    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
            | Qt::WindowMinimizeButtonHint
            | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    // set not resizeable
    this->setFixedSize(this->size());

    // restore the position of the window
    if (m_Settings.value("SaveMainWindowGeometry", true).toBool())
    {
        restoreGeometry(m_Settings.value("MainWindowGeometry").toByteArray());
    }

    // disable controls
    EnableControls(false);
    ui->PowerButton->setEnabled(false);

    // get compatibility setting for favorites list for LX-83
    m_FavoritesCompatibilityMode = m_Settings.value("FavoritesCompatibilityMode", false).toBool();

    // receiver interface
    connect((&m_ReceiverInterface), SIGNAL(Connected()), this, SLOT(CommConnected()));
    connect((&m_ReceiverInterface), SIGNAL(Disconnected()), this, SLOT(CommDisconnected()));
    connect((&m_ReceiverInterface), SIGNAL(CommError(QString)), this,  SLOT(CommError(QString)));
    connect((&m_ReceiverInterface), SIGNAL(DataReceived(QString)), this,  SLOT(NewDataReceived(QString)));
    connect((&m_ReceiverInterface), SIGNAL(DisplayData(int, QString)), this,  SLOT(DisplayData(int, QString)));
    connect((&m_ReceiverInterface), SIGNAL(PowerData(bool)), this,  SLOT(PowerData(bool)));
    connect((&m_ReceiverInterface), SIGNAL(VolumeData(double)), this,  SLOT(VolumeData(double)));
    connect((&m_ReceiverInterface), SIGNAL(MuteData(bool)), this,  SLOT(MuteData(bool)));
    connect((&m_ReceiverInterface), SIGNAL(ErrorData(int)), this,  SLOT(ErrorData(int)));
    connect((&m_ReceiverInterface), SIGNAL(AudioStatusData(QString, QString)), this,  SLOT(AudioStatusData(QString, QString)));
    connect((&m_ReceiverInterface), SIGNAL(InputFunctionData(int, QString)), this,  SLOT(InputFunctionData(int, QString)));
    connect((&m_ReceiverInterface), SIGNAL(PhaseData(int)), this,  SLOT(PhaseData(int)));
    connect((&m_ReceiverInterface), SIGNAL(InputNameData(QString)), this,  SLOT(InputNameData(QString)));
    connect((&m_ReceiverInterface), SIGNAL(ListeningModeData(QString)), this,  SLOT(ListeningModeData(QString)));
    connect((&m_ReceiverInterface), SIGNAL(HiBitData(bool)), this,  SLOT(HiBitData(bool)));
    connect((&m_ReceiverInterface), SIGNAL(PqlsData(bool)), this,  SLOT(PqlsData(bool)));
    connect((&m_ReceiverInterface), SIGNAL(DFiltData(bool)), this,  SLOT(DFiltData(bool)));
    connect((&m_ReceiverInterface), SIGNAL(ReceiverType(QString,QString)), this,  SLOT(ReceiverType(QString,QString)));
    connect((&m_ReceiverInterface), SIGNAL(ReceiverNetworkName(QString)), this,  SLOT(ReceiverNetworkName(QString)));

    // configure ip adress edit iput
    ui->lineEditIP1->setValidator(&m_IpValidator);
    ui->lineEditIP2->setValidator(&m_IpValidator);
    ui->lineEditIP3->setValidator(&m_IpValidator);
    ui->lineEditIP4->setValidator(&m_IpValidator);
    ui->lineEditIPPort->setValidator(&m_IpPortValidator);
    // get the saved ip address data
    ui->lineEditIP1->setText(m_Settings.value("IP/1", "192").toString());
    ui->lineEditIP2->setText(m_Settings.value("IP/2", "168").toString());
    ui->lineEditIP3->setText(m_Settings.value("IP/3", "1").toString());
    ui->lineEditIP4->setText(m_Settings.value("IP/4", "1").toString());
    ui->lineEditIPPort->setText(m_Settings.value("IP/PORT", "8102").toString());

    // make a list with buttons that correspond to a input type
    /* 00 */ m_InputButtons.append(NULL); //"PHONO"
    /* 01 */ m_InputButtons.append(ui->InputCdButton); //"CD",
    /* 02 */ m_InputButtons.append(ui->InputTunerButton); //"TUNER",
    /* 03 */ m_InputButtons.append(NULL); //"CD-R/TAPE",
    /* 04 */ m_InputButtons.append(ui->InputDvdButton); //"DVD",
    /* 05 */ m_InputButtons.append(ui->InputSatButton); //"TV/SAT",
    /* 06 */ m_InputButtons.append(ui->InputSatButton); //"SAT/CBL",
    /* 07 */ m_InputButtons.append(NULL); //"",
    /* 08 */ m_InputButtons.append(NULL); //"",
    /* 09 */ m_InputButtons.append(NULL); //"",
    /* 10 */ m_InputButtons.append(ui->InputVideoButton); //"VIDEO 1(VIDEO)",
    /* 11 */ m_InputButtons.append(NULL); //"",
    /* 12 */ m_InputButtons.append(NULL); //"MULTI CH IN",
    /* 13 */ m_InputButtons.append(NULL); //"",
    /* 14 */ m_InputButtons.append(NULL); //"VIDEO 2",
    /* 15 */ m_InputButtons.append(ui->InputDvrButton); //"DVR/BDR",
    /* 16 */ m_InputButtons.append(NULL); //"",
    /* 17 */ m_InputButtons.append(ui->InputIpodButton); //"iPod/USB",
    /* 18 */ m_InputButtons.append(ui->InputNetButton); //"XM RADIO",
    /* 19 */ m_InputButtons.append(ui->InputHdmiButton); //"HDMI 1",
    /* 20 */ m_InputButtons.append(ui->InputHdmiButton); //"HDMI 2",
    /* 21 */ m_InputButtons.append(ui->InputHdmiButton); //"HDMI 3",
    /* 22 */ m_InputButtons.append(ui->InputHdmiButton); //"HDMI 4",
    /* 23 */ m_InputButtons.append(ui->InputHdmiButton); //"HDMI 5",
    /* 24 */ m_InputButtons.append(ui->InputHdmiButton); //"HDMI 6",
    /* 25 */ m_InputButtons.append(ui->InputBdButton); //"BD",
    /* 26 */ m_InputButtons.append(ui->InputNetButton); //"HOME MEDIA GALLERY(Internet Radio)",
    /* 27 */ m_InputButtons.append(ui->InputNetButton); //"SIRIUS",
    /* 28 */ m_InputButtons.append(NULL); //"",
    /* 29 */ m_InputButtons.append(NULL); //"",
    /* 30 */ m_InputButtons.append(NULL); //"",
    /* 31 */ m_InputButtons.append(ui->InputHdmiButton); //"HDMI (cyclic)",
    /* 32 */ m_InputButtons.append(NULL); //"",
    /* 33 */ m_InputButtons.append(ui->InputAdptButton); //"ADAPTER PORT",
    /* 34 */ m_InputButtons.append(ui->InputHdmiButton); //"HDMI 7",
    /* 35 */ m_InputButtons.append(NULL); //"",
    /* 36 */ m_InputButtons.append(NULL); //"",
    /* 37 */ m_InputButtons.append(NULL); //"",
    /* 38 */ m_InputButtons.append(ui->InputNetButton); //"INTERNET RADIO",
    /* 39 */ m_InputButtons.append(NULL); //"",
    /* 40 */ m_InputButtons.append(ui->InputNetButton); //"SiriusXM",
    /* 41 */ m_InputButtons.append(ui->InputNetButton); //"PANDORA",
    /* 42 */ m_InputButtons.append(NULL); //"",
    /* 43 */ m_InputButtons.append(NULL); //"",
    /* 44 */ m_InputButtons.append(ui->InputNetButton); //"MEDIA SERVER",
    /* 45 */ m_InputButtons.append(ui->InputNetButton); //"FAVORITES",
    /* 46 */ m_InputButtons.append(NULL); //"",
    /* 47 */ m_InputButtons.append(NULL); //"",
    /* 48 */ m_InputButtons.append(ui->InputHdmiButton); //"MHL",
    /* END */ m_InputButtons.append((QPushButton*)-1);

    // configure the timer
    connect((&m_StatusLineTimer), SIGNAL(timeout()), this, SLOT(StatusLineTimeout()));
    m_StatusLineTimer.setSingleShot(true);
    m_StatusLineTimer.setInterval(10000);

    // create NetRadio dialog
    m_NetRadioDialog = new NetRadioDialog(this, m_Settings, m_ReceiverInterface);

    // create usb dialog
    m_usbDialog = new usbDialog(this, m_Settings, m_ReceiverInterface);

    // create loudspeaker dialog
    m_LoudspeakerSettingsDialog = new LoudspeakerSettingsDialog(this, m_Settings, m_ReceiverInterface );

    // create Tuner dialog
    m_TunerDialog = new TunerDialog(this, m_ReceiverInterface, m_Settings);

    // create Test dialog
    m_TestDialog = new TestDialog(this, m_ReceiverInterface, m_Settings);

    // create compatible favorites dialog
    m_OldFavoritesDialog = new OldFavoritesDialog(this, &m_ReceiverInterface);

    // create Settings dialog
    m_SettingsDialog = new SettingsDialog(this, m_Settings);

    // create EQ dialog
    m_EQDialog = new EQDialog(this, m_ReceiverInterface,m_Settings);

    m_Listendiag = new  ListeningModeDialog(this, m_Settings, m_ReceiverInterface );

}


AVRPioRemote::~AVRPioRemote()
{
    delete m_NetRadioDialog;
    delete m_usbDialog;
    delete m_LoudspeakerSettingsDialog;
    delete m_TunerDialog;
    delete m_TestDialog;
    delete m_OldFavoritesDialog;
    delete m_SettingsDialog;
    delete m_EQDialog;
    delete ui;
    delete m_Listendiag;
}


void AVRPioRemote::closeEvent(QCloseEvent *event)
{
    m_ReceiverInterface.Disconnect();
    // save the window position
    m_Settings.setValue("MainWindowGeometry", saveGeometry());
    QDialog::closeEvent(event);
}


void AVRPioRemote::StatusLineTimeout()
{
    ui->StatusLineEdit->clear();
}


void AVRPioRemote::SelectInputButton(int idx)
{
    int i = 0;
    QPushButton* found = NULL;
    // find a button associated with the input
    while (m_InputButtons[i] != (QPushButton*)-1)
    {
        if (m_InputButtons[i] != NULL)
        {
            if (i == idx)
            {
                // select the found input button
                m_InputButtons[i]->setChecked(true);
                found = m_InputButtons[i];
            }
            else
            {
                // unselect all other input buttons
                if(m_InputButtons[i] != found)
                    m_InputButtons[i]->setChecked(false);
            }
        }
        i++;
    }
    // if it is a net input, open NetRadio window, otherwise close it
    if (found == ui->InputNetButton)
    {
        if (!m_FavoritesCompatibilityMode)
        {
            m_NetRadioDialog->ShowNetDialog();
        }
        else
        {
            m_OldFavoritesDialog->ShowOldFavoritesDialog();
        }
    }
    else
    {
        if (!m_FavoritesCompatibilityMode)
        {
            if (m_NetRadioDialog->isVisible())
                m_NetRadioDialog->hide();
        }
        else
        {
            if (m_OldFavoritesDialog->isVisible())
                m_OldFavoritesDialog->hide();
        }
    }
    // if it is the tuner input, open Tuner window, otherwise close it

    if (found == ui->InputTunerButton)
    {
        m_TunerDialog->ShowTunerDialog();
    }
    else
    {
        if (m_TunerDialog->isVisible())
            m_TunerDialog->hide();
    }
    if (found == ui->InputIpodButton)
    {
        m_usbDialog->ShowusbDialog();
    }
    else
    {
        if (m_usbDialog->isVisible())
            m_usbDialog->hide();
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
    QString cmd = QString("%1SR").arg(Param);
    SendCmd(cmd);
}

void AVRPioRemote::ConnectReceiver()
{
    ui->pushButtonConnect->setEnabled(false);
    EnableIPInput(false);

    ui->StatusLineEdit->setText(tr("Connecting..."));
    m_StatusLineTimer.start();

    if (!m_ReceiverInterface.IsConnected())
    {
        m_ReceiverInterface.ConnectToReceiver(m_IpAddress, m_IpPort);
    }

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


void AVRPioRemote::NewDataReceived(QString data)
{
    Logger::Log("<-- " + data);
}


void AVRPioRemote::DisplayData(int, QString data)
{
    ui->ScreenLineEdit1->setText(data);
}


void AVRPioRemote::PowerData(bool powerOn)
{
    ui->PowerButton->setChecked(powerOn);
    ui->PowerButton->setText((!powerOn)?tr("ON"):tr("OFF"));
    EnableControls(powerOn);
    m_ReceiverOnline = powerOn;
}


void AVRPioRemote::VolumeData(double dB)
{
    QString str;
    if (dB <= -80.5)
        str = "---.---";
    else
    {
        if (dB <= 0.0)
            str = QString("%1dB").arg(dB, 4, 'f', 1);
        else
            str = QString("+%1dB").arg(dB, 4, 'f', 1);
    }
    ui->VolumeLineEdit->setText(str);
}

void AVRPioRemote::MuteData(bool muted)
{
    ui->VolumeMuteButton->setChecked(muted);
}


void AVRPioRemote::ErrorData(int type)
{
    switch(type)
    {
    case 2:
        Logger::Log("This doesn't work now");
        ui->StatusLineEdit->setText(tr("This doesn't work now"));
        m_StatusLineTimer.start();
        break;
    case 3:
        Logger::Log("This does'nt work with this receiver");
        ui->StatusLineEdit->setText(tr("This doesn't work with this receiver"));
        m_StatusLineTimer.start();
        break;
    case 4:
        Logger::Log("Command error");
        ui->StatusLineEdit->setText(tr("Command error"));
        m_StatusLineTimer.start();
        break;
    case 6:
        Logger::Log("Parameter error");
        ui->StatusLineEdit->setText(tr("Parameter error"));
        m_StatusLineTimer.start();
        break;
    case -1:
        Logger::Log("Receiver busy");
        ui->StatusLineEdit->setText(tr("Receiver busy"));
        m_StatusLineTimer.start();
        break;
    default:
        Logger::Log("Unknown error");
        ui->StatusLineEdit->setText(tr("Unknown error"));
        m_StatusLineTimer.start();
        break;
    }
}


void AVRPioRemote::AudioStatusData(QString codec, QString samplingRate)
{
    ui->AudioCodecLineEdit->setText(codec);
    ui->AudioSampleRateLineEdit->setText(samplingRate);
}


void AVRPioRemote::InputFunctionData(int no, QString name)
{
    ui->InputLineEdit->setText(name);
    SelectInputButton(no);
    QString str = QString("?RGB%1").arg(no, 2, 10, QLatin1Char('0'));
    SendCmd(str.toLocal8Bit());
    RequestStatus(false);
}


void AVRPioRemote::PhaseData(int phase)
{
    ui->PhaseButton->setChecked(phase != 0);
}


void AVRPioRemote::InputNameData(QString name)
{
    ui->InputNameLineEdit->setText(name);
}


void AVRPioRemote::ListeningModeData(QString name)
{
    ui->ListeningModeLineEdit->setText(name);
}


void AVRPioRemote::HiBitData(bool set)
{
    ui->HiBitButton->setChecked(set);
}


void AVRPioRemote::PqlsData(bool set)
{
    ui->PqlsButton->setChecked(set);
}


void AVRPioRemote::DFiltData(bool set)
{
    ui->DFiltButton->setChecked(set);
}


void AVRPioRemote::RequestStatus(bool input)
{
    SendCmd("?P"); // power
    if (m_ReceiverOnline)
    {
        SendCmd("?V"); // volume
        SendCmd("?M"); // mute
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
        SendCmd("?IS"); // phase control
        SendCmd("?ATI"); // Hi-Bit
        SendCmd("?ATA"); // Hi-Bit
        SendCmd("?PQ"); // PQLS
    }
    //sendCmd("?RGB**"); // request input name information
}

void AVRPioRemote::CommError(QString socketError)
{
    Logger::Log("tcp error");
    ui->pushButtonConnect->setEnabled(true);
    EnableIPInput(true);
    EnableControls(false);
    ui->pushButtonConnect->setText(tr("Connect"));
    ClearScreen();
    ui->StatusLineEdit->setText(socketError);
    m_StatusLineTimer.start();
}

void AVRPioRemote::CommConnected()
{
    Logger::Log("connected");
    ui->PowerButton->setEnabled(true);
    ui->StatusLineEdit->setText(tr("Connected"));
    m_StatusLineTimer.start();
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonConnect->setText(tr("Disconnect"));
    ui->pushButtonConnect->setChecked(true);
    m_ReceiverOnline = true;
    SendCmd("?RGD"); // Receiver-Kennung
    SendCmd("?SSO"); // Receiver friendly name (network)
    RequestStatus();
}

void AVRPioRemote::CommDisconnected()
{
    Logger::Log("disconnected");
    EnableIPInput(true);
    ui->pushButtonConnect->setText(tr("Connect"));
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonConnect->setChecked(false);
    EnableControls(false);
    ClearScreen();
}

//void AVRPioRemote::Log(const QString& text)
//{
//    Log(text, QColor(0, 0, 0));
//}

//void AVRPioRemote::Log(const QString& text, const QColor& color)
//{
////    ui->listWidgetLog->addItem(text);
////    if (ui->listWidgetLog->count() > 100)
////        ui->listWidgetLog->removeItemWidget(ui->listWidgetLog->item(0));
////    ui->listWidgetLog->setCurrentItem(ui->listWidgetLog->item(ui->listWidgetLog->count() - 1));
////    QBrush brush(color);
////    int index = ui->listWidgetLog->currentIndex().row();
////    ui->listWidgetLog->item(index)->setForeground(brush);
//    qDebug() << text;
//}

bool AVRPioRemote::SendCmd(const QString& cmd)
{
    m_ReceiverInterface.SendCmd(cmd);
    return true;
}

void AVRPioRemote::EnableControls(bool enable)
{
    ui->DFiltButton->setEnabled(enable);
    ui->HiBitButton->setEnabled(enable);
    ui->InputAdptButton->setEnabled(enable);
    ui->InputBdButton->setEnabled(enable);
    ui->InputCdButton->setEnabled(enable);
    ui->InputDvdButton->setEnabled(enable);
    ui->InputDvrButton->setEnabled(enable);
    ui->InputHdmiButton->setEnabled(enable);
    ui->InputIpodButton->setEnabled(enable);
    ui->InputLeftButton->setEnabled(enable);
    ui->InputNetButton->setEnabled(enable);
    ui->InputRightButton->setEnabled(enable);
    ui->InputSatButton->setEnabled(enable);
    ui->InputTunerButton->setEnabled(enable);
    ui->InputTvButton->setEnabled(enable);
    ui->InputVideoButton->setEnabled(enable);
    ui->Num1Button->setEnabled(enable);
    ui->Num2Button->setEnabled(enable);
    ui->Num3Button->setEnabled(enable);
    ui->PhaseButton->setEnabled(enable);
    ui->PqlsButton->setEnabled(enable);
    ui->VolumeDownButton->setEnabled(enable);
    ui->VolumeMuteButton->setEnabled(enable);
    ui->VolumeUpButton->setEnabled(enable);
    ui->ShowAllListeningModesButton->setEnabled(enable);
    ui->ATBEQModesButton->setEnabled(enable);
}

void AVRPioRemote::EnableIPInput(bool enable)
{
    ui->lineEditIP1->setEnabled(enable);
    ui->lineEditIP2->setEnabled(enable);
    ui->lineEditIP3->setEnabled(enable);
    ui->lineEditIP4->setEnabled(enable);
    ui->lineEditIPPort->setEnabled(enable);
}

void AVRPioRemote::ClearScreen()
{
    ui->ScreenLineEdit1->setText("");
    ui->ListeningModeLineEdit->setText("");
    ui->AudioCodecLineEdit->setText("");
    ui->AudioSampleRateLineEdit->setText("");
    ui->VolumeLineEdit->setText("");
    ui->InputLineEdit->setText("");
    ui->InputNameLineEdit->setText("");
}

void AVRPioRemote::onConnect()
{
    if (!m_ReceiverInterface.IsConnected())
    {
        // connect
        // read settings from the line edits
        QString ip1, ip2, ip3, ip4, ip_port;
        // first the 4 ip address blocks
        ip1 = ui->lineEditIP1->text().trimmed();
        if (ip1 == "")
        {
            ip1 = "192"; // set default
            ui->lineEditIP1->setText(ip1);
        }
        ip2 = ui->lineEditIP2->text().trimmed();
        if (ip2 == "")
        {
            ip2 = "168"; // set default
            ui->lineEditIP2->setText(ip2);
        }
        ip3 = ui->lineEditIP3->text().trimmed();
        if (ip3 == "")
        {
            ip3 = "1"; // set default
            ui->lineEditIP3->setText(ip3);
        }
        ip4 = ui->lineEditIP4->text().trimmed();
        if (ip4 == "")
        {
            ip4 = "192"; // set default
            ui->lineEditIP4->setText(ip4);
        }
        m_IpAddress = ip1 + "." + ip2 + "." + ip3 + "." + ip4;
        // and then th ip port
        ip_port = ui->lineEditIPPort->text().trimmed();
        if (ip_port == "")
        {
            ip_port = "8102"; // set default
            ui->lineEditIPPort->setText(ip_port);
        }
        else
        {
            m_IpPort = ip_port.toInt();
        }
        // save the ip address and port permanently
        m_Settings.setValue("IP/1", ip1);
        m_Settings.setValue("IP/2", ip2);
        m_Settings.setValue("IP/3", ip3);
        m_Settings.setValue("IP/4", ip4);
        m_Settings.setValue("IP/PORT", ip_port);
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
        pAction = new QAction(tr("Internet Radio"), this);
        MyMenu.addAction(pAction);
        connect(pAction, SIGNAL(triggered()), m_NetRadioDialog, SLOT(ShowNetDialog()));

        pAction = new QAction(tr("Tuner"), this);
        MyMenu.addAction(pAction);
        connect(pAction, SIGNAL(triggered()), m_TunerDialog, SLOT(ShowTunerDialog()));

        pAction = new QAction(tr("IPod/USB"), this);
        MyMenu.addAction(pAction);
        connect(pAction, SIGNAL(triggered()), m_usbDialog, SLOT(ShowusbDialog()));

        pAction = new QAction(tr("Equalizer"), this);
        MyMenu.addAction(pAction);
        connect(pAction, SIGNAL(triggered()), m_EQDialog, SLOT(ShowEQDialog()));

        pAction = new QAction(tr("Speaker Settings"), this);
        MyMenu.addAction(pAction);
        connect(pAction, SIGNAL(triggered()), m_LoudspeakerSettingsDialog, SLOT(ShowLoudspeakerSettingsDialog()));

        pAction = new QAction(tr("ListenMode Settings"), this);
        MyMenu.addAction(pAction);
        connect(pAction, SIGNAL(triggered()), m_Listendiag, SLOT(ShowListeningDialog()));


//        pAction = new QAction(tr("Compatible Favorites"), this);
//        MyMenu.addAction(pAction);
//        connect(pAction, SIGNAL(triggered()), m_OldFavoritesDialog, SLOT(ShowOldFavoritesDialog()));

        pAction = new QAction(tr("Refresh status"), this);
        MyMenu.addAction(pAction);
        connect(pAction, SIGNAL(triggered()), this, SLOT(RequestStatus()));

//        pAction = new QAction("More Information", this);
//        pAction = new QAction("Equalizer", this);
//        pAction = new QAction("LS Settings", this);
//        pAction = new QAction("Show Zone 2", this);
//        pAction = new QAction("Input Wizard", this);
    }
    pAction = new QAction(tr("-"), this);
    pAction->setSeparator(true);
    MyMenu.addAction(pAction);

    pAction = new QAction(tr("Test"), this);
    MyMenu.addAction(pAction);
    connect(pAction, SIGNAL(triggered()), m_TestDialog, SLOT(ShowTestDialog()));

    pAction = new QAction(tr("Settings"), this);
    MyMenu.addAction(pAction);
    connect(pAction, SIGNAL(triggered()), m_SettingsDialog, SLOT(ShowSettingsDialog()));

    pAction = new QAction(tr("About AVRPioRemote"), this);
    MyMenu.addAction(pAction);
    connect(pAction, SIGNAL(triggered()), this, SLOT(ShowAboutDialog()));


    QPoint pos = QCursor::pos();
    MyMenu.exec(pos);
}

void AVRPioRemote::on_VolumeUpButton_clicked()
{
    SendCmd("VU");
}

void AVRPioRemote::on_VolumeDownButton_clicked()
{
    SendCmd("VD");
}

void AVRPioRemote::on_VolumeMuteButton_clicked()
{
    if (ui->VolumeMuteButton->isChecked())
        SendCmd("MO");
    else
        SendCmd("MF");
}

void AVRPioRemote::on_pushButtonConnect_clicked()
{
    ui->pushButtonConnect->setChecked(!ui->pushButtonConnect->isChecked());
    onConnect();
}


void AVRPioRemote::on_PowerButton_clicked()
{
    ui->PowerButton->setChecked(!ui->PowerButton->isChecked());
    if (!ui->PowerButton->isChecked())
    {
        SendCmd("PO");
        SendCmd("PO");
    }
    else
    {
        SendCmd("PF");
        SendCmd("PF");
        m_ReceiverOnline = false;
    }
}


void AVRPioRemote::on_InputLeftButton_clicked()
{
    SendCmd("FD");
}

void AVRPioRemote::on_InputRightButton_clicked()
{
    SendCmd("FU");
}

void AVRPioRemote::on_PhaseButton_clicked()
{
    ui->PhaseButton->setChecked(!ui->PhaseButton->isChecked());
    //SendCmd("9IS");
    if (ui->PhaseButton->isChecked())
        SendCmd("0IS");
    else
        SendCmd("1IS");
}

void AVRPioRemote::on_PqlsButton_clicked()
{
    ui->PqlsButton->setChecked(!ui->PqlsButton->isChecked());
    //SendCmd("9PQ");
    if (ui->PqlsButton->isChecked())
        SendCmd("0PQ");
    else
        SendCmd("1PQ");
}

void AVRPioRemote::on_DFiltButton_clicked()
{
    ui->DFiltButton->setChecked(!ui->DFiltButton->isChecked());
    //SendCmd("9ATV");
    if (ui->DFiltButton->isChecked())
        SendCmd("0ATA");
    else
        SendCmd("1ATA");
}

void AVRPioRemote::on_HiBitButton_clicked()
{
    ui->HiBitButton->setChecked(!ui->HiBitButton->isChecked());
    //SendCmd("9ATI");
    if (ui->HiBitButton->isChecked())
        SendCmd("0ATI");
    else
        SendCmd("1ATI");
}

void AVRPioRemote::on_InputBdButton_clicked()
{
    ui->InputBdButton->setChecked(!ui->InputBdButton->isChecked());
    SendCmd("25FN");
}

void AVRPioRemote::on_InputDvdButton_clicked()
{
    ui->InputDvdButton->setChecked(!ui->InputDvdButton->isChecked());
    SendCmd("04FN");
}

void AVRPioRemote::on_InputDvrButton_clicked()
{
    ui->InputDvrButton->setChecked(!ui->InputDvrButton->isChecked());
    SendCmd("15FN");
}

void AVRPioRemote::on_InputTvButton_clicked()
{
    ui->InputTvButton->setChecked(!ui->InputTvButton->isChecked());
    SendCmd("05FN");
}

void AVRPioRemote::on_InputCdButton_clicked()
{
    ui->InputCdButton->setChecked(!ui->InputCdButton->isChecked());
    SendCmd("01FN");
}

void AVRPioRemote::on_InputIpodButton_clicked()
{
    m_usbDialog->ShowusbDialog();
    ui->InputIpodButton->setChecked(!ui->InputIpodButton->isChecked());
    SendCmd("17FN");
}

void AVRPioRemote::on_InputSatButton_clicked()
{
    ui->InputSatButton->setChecked(!ui->InputSatButton->isChecked());
    SendCmd("06FN");
}

void AVRPioRemote::on_InputAdptButton_clicked()
{
    ui->InputAdptButton->setChecked(!ui->InputAdptButton->isChecked());
    SendCmd("33FN");
}

void AVRPioRemote::on_InputHdmiButton_clicked()
{
    ui->InputHdmiButton->setChecked(!ui->InputHdmiButton->isChecked());
    SendCmd("31FN");
}

void AVRPioRemote::on_InputNetButton_clicked()
{
    ui->InputNetButton->setChecked(!ui->InputNetButton->isChecked());
    SendCmd("26FN");
}

void AVRPioRemote::on_InputTunerButton_clicked()
{
    ui->InputTunerButton->setChecked(!ui->InputTunerButton->isChecked());
    m_TunerDialog->ShowTunerDialog();
    SendCmd("02FN");
}

void AVRPioRemote::on_Num1Button_clicked()
{
    //ui->InputTunerButton->setChecked(!ui->InputTunerButton->isChecked());
    SendCmd("0005SR");
}

void AVRPioRemote::on_Num2Button_clicked()
{
    //ui->InputTunerButton->setChecked(!ui->InputTunerButton->isChecked());
    SendCmd("0010SR");
}

void AVRPioRemote::on_Num3Button_clicked()
{
    //ui->InputTunerButton->setChecked(!ui->InputTunerButton->isChecked());
    SendCmd("0100SR");
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
    SendCmd("10FN");
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

