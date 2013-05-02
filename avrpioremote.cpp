#include "avrpioremote.h"
#include "ui_avrpioremote.h"
#include <QDebug>
//#include <QLayout>
#include <qtextcodec.h>
#include "actionwithparameter.h"

AVRPioRemote::AVRPioRemote(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AVRPioRemote),
    m_IpValidator(0, 255, this),
    m_IpPortValidator(0, 35535, this),
    m_Settings("AMCore", "AVRPioRemote"),
    m_FavoritesCompatibilityMode(false)
{
    m_IpPort = 8102;

    m_ReceiverOnline = false;

    ui->setupUi(this);
    //setWindowState(Qt::WindowMinimized);
    //Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint;
    Qt::WindowFlags flags = windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowContextHelpButtonHint;
    this->setWindowFlags(flags);
    //layout()->setSizeConstraint( QLayout::SetFixedSize );
    this->setFixedSize(this->size());
    EnableControls(false);
    ui->PowerButton->setEnabled(false);

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
    //connect((&m_ReceiverInterface), SIGNAL(NetData(QString)), this,  SLOT(NetData(QString)));
    connect((&m_ReceiverInterface), SIGNAL(DFiltData(bool)), this,  SLOT(DFiltData(bool)));
    //connect((&m_ReceiverInterface), SIGNAL(NetData(QString)), this, SLOT(ShowNetDialog()));

    // ip adress
    ui->lineEditIP1->setValidator(&m_IpValidator);
    ui->lineEditIP2->setValidator(&m_IpValidator);
    ui->lineEditIP3->setValidator(&m_IpValidator);
    ui->lineEditIP4->setValidator(&m_IpValidator);
    ui->lineEditIPPort->setValidator(&m_IpPortValidator);
    ui->lineEditIP1->setText(m_Settings.value("IP/1", "192").toString());
    ui->lineEditIP2->setText(m_Settings.value("IP/2", "168").toString());
    ui->lineEditIP3->setText(m_Settings.value("IP/3", "1").toString());
    ui->lineEditIP4->setText(m_Settings.value("IP/4", "1").toString());
    ui->lineEditIPPort->setText(m_Settings.value("IP/PORT", "8102").toString());

//    m_TCPThread = new QThread();
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

    m_NetRadioDialog = new NetRadioDialog(this, m_Settings);
    connect((&m_ReceiverInterface), SIGNAL(NetData(QString)), m_NetRadioDialog, SLOT(NetData(QString)));
    connect((m_NetRadioDialog),    SIGNAL(SendCmd(QString)), this, SLOT(SendCmd(QString)));

    m_LoudspeakerSettingsDialog = new LoudspeakerSettingsDialog(this, m_Settings);

    m_TunerDialog = new TunerDialog(this, m_ReceiverInterface, m_Settings);


    m_TestDialog = new TestDialog(this);
    connect((&m_ReceiverInterface), SIGNAL(DisplayData(int, QString)), m_TestDialog,  SLOT(DisplayData(int, QString)));
    connect((m_TestDialog),    SIGNAL(SendCmd(QString)), this, SLOT(SendCmd(QString)));
    connect((&m_ReceiverInterface), SIGNAL(InputFunctionData(int, QString)), m_TestDialog,  SLOT(InputFunctionData(int, QString)));

    m_OldFavoritesDialog = new OldFavoritesDialog(this, &m_ReceiverInterface);

    m_SettingsDialog = new SettingsDialog(this, m_Settings);
    m_SettingsDialog->setModal(true);;
}

AVRPioRemote::~AVRPioRemote()
{
    delete ui;
}

void AVRPioRemote::closeEvent(QCloseEvent *event)
{
    m_ReceiverInterface.Disconnect();
    QDialog::closeEvent(event);
}

void AVRPioRemote::SelectInputButton(int idx)
{
    int i = 0;
    QPushButton* found = NULL;
    while (m_InputButtons[i] != (QPushButton*)-1)
    {
        if (m_InputButtons[i] != NULL)
        {
            if (i == idx)
            {
                m_InputButtons[i]->setChecked(true);
                found = m_InputButtons[i];
            }
            else
            {
                if(m_InputButtons[i] != found)
                    m_InputButtons[i]->setChecked(false);
            }
        }
        i++;
    }
    if (found == ui->InputNetButton)
    {
        if (!m_FavoritesCompatibilityMode)
        {
            ShowNetDialog();
        }
        else
        {
            ShowOldFavoritesDialog();
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
    if (found == ui->InputTunerButton)
    {
        m_TunerDialog->ShowTunerDialog();
    }
    else
    {
        if (m_TunerDialog->isVisible())
            m_TunerDialog->hide();
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

    ui->StausLineEdit->setText("Connecting...");

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


void AVRPioRemote::DisplayData(int DispNo, QString data)
{
    ui->ScreenLineEdit1->setText(data);
}


void AVRPioRemote::PowerData(bool powerOn)
{
    ui->PowerButton->setChecked(powerOn);
    ui->PowerButton->setText((!powerOn)?"ON":"OFF");
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
        ui->StausLineEdit->setText("This doesn't work now");
        break;
    case 3:
        Logger::Log("This does'nt work with this receiver");
        ui->StausLineEdit->setText("This doesn't work with this receiver");
        break;
    case 4:
        Logger::Log("Command error");
        ui->StausLineEdit->setText("Command error");
        break;
    case 6:
        Logger::Log("Parameter error");
        ui->StausLineEdit->setText("Parameter error");
        break;
    case -1:
        Logger::Log("Receiver busy");
        ui->StausLineEdit->setText("Receiver busy");
        break;
    default:
        Logger::Log("Unknown error");
        ui->StausLineEdit->setText("Unknown error");
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
    SendCmd(str.toAscii());
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
    ui->pushButtonConnect->setText("Connect");
    ClearScreen();
    ui->StausLineEdit->setText(socketError);
}

void AVRPioRemote::CommConnected()
{
    Logger::Log("connected");
    ui->PowerButton->setEnabled(true);
    ui->StausLineEdit->setText("Connected");
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonConnect->setText("Disconnect");
    m_ReceiverOnline = true;
    RequestStatus();
}

void AVRPioRemote::CommDisconnected()
{
    Logger::Log("disconnected");
    EnableIPInput(true);
    ui->pushButtonConnect->setText("Connect");
    ui->pushButtonConnect->setEnabled(true);
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
        QString ip1, ip2, ip3, ip4, ip_port;
        ip1 = ui->lineEditIP1->text().trimmed();
        if (ip1 == "")
        {
            ip1 = "192";
            ui->lineEditIP1->setText(ip1);
        }
        ip2 = ui->lineEditIP2->text().trimmed();
        if (ip2 == "")
        {
            ip2 = "168";
            ui->lineEditIP2->setText(ip2);
        }
        ip3 = ui->lineEditIP3->text().trimmed();
        if (ip3 == "")
        {
            ip3 = "1";
            ui->lineEditIP3->setText(ip3);
        }
        ip4 = ui->lineEditIP4->text().trimmed();
        if (ip4 == "")
        {
            ip4 = "192";
            ui->lineEditIP4->setText(ip4);
        }
        m_IpAddress = ip1 + "." + ip2 + "." + ip3 + "." + ip4;
        ip_port = ui->lineEditIPPort->text().trimmed();
        if (ip_port == "")
        {
            ip_port = "8102";
            m_IpPort = ip_port.toInt();
            ui->lineEditIPPort->setText(ip_port);
        }
        m_Settings.setValue("IP/1", ip1);
        m_Settings.setValue("IP/2", ip2);
        m_Settings.setValue("IP/3", ip3);
        m_Settings.setValue("IP/4", ip4);
        m_Settings.setValue("IP/PORT", ip_port);
        ConnectReceiver();
    }
    else
    {
        EnableControls(false);
        ui->PowerButton->setEnabled(false);
        m_ReceiverInterface.Disconnect();
        m_ReceiverOnline = false;
    }
}

void AVRPioRemote::ShowNetDialog()
{
    if (!m_NetRadioDialog->isVisible())
    {
        SendCmd("?GAH");
        int x = this->pos().x() + this->width() + 20;
        QPoint pos;
        pos.setX(x);
        pos.setY(this->pos().y());
        m_NetRadioDialog->move(pos);
        m_NetRadioDialog->show();
    }
}


void AVRPioRemote::ShowSettingsDialog()
{
    if (!m_SettingsDialog->isVisible())
    {
//        int x = this->pos().x() + this->width() + 20;
//        QPoint pos;
//        pos.setX(x);
//        pos.setY(this->pos().y());
//        m_SettingsDialog->move(pos);
        m_SettingsDialog->show();
    }
}


void AVRPioRemote::ShowAboutDialog()
{
    AboutDialog* about = new AboutDialog(this);
    about->show();
}

void AVRPioRemote::ShowTestDialog()
{
    if (!m_TestDialog->isVisible())
    {
        int x = this->pos().x() + this->width() + 20;
        QPoint pos;
        pos.setX(x);
        pos.setY(this->pos().y());
        m_TestDialog->move(pos);
        m_TestDialog->show();
    }
}

void AVRPioRemote::ShowOldFavoritesDialog()
{
    if (!m_OldFavoritesDialog ->isVisible())
    {
        int x = this->pos().x() + this->width() + 20;
        QPoint pos;
        pos.setX(x);
        pos.setY(this->pos().y());
        m_OldFavoritesDialog->move(pos);
        m_OldFavoritesDialog->show();
    }
}

void AVRPioRemote::ShowLoudspeakerSettingsDialog()
{
    if (!m_LoudspeakerSettingsDialog->isVisible())
    {
        //SendCmd("?GAH");
        int x = this->pos().x() + this->width() + 20;
        QPoint pos;
        pos.setX(x);
        pos.setY(this->pos().y());
        m_LoudspeakerSettingsDialog->move(pos);
        m_LoudspeakerSettingsDialog->show();
    }
}

void AVRPioRemote::on_MoreButton_clicked()
{
    QAction* pAction;
    QMenu MyMenu(this);
    //MyMenu.addActions(this->actions());
    if (m_ReceiverOnline == true)
    {
        pAction = new QAction("Refresh status", this);
        MyMenu.addAction(pAction);
        connect(pAction, SIGNAL(triggered()), this, SLOT(RequestStatus()));

        pAction = new QAction("Internet Radio", this);
        MyMenu.addAction(pAction);
        connect(pAction, SIGNAL(triggered()), this, SLOT(ShowNetDialog()));

        pAction = new QAction("Compatible Favorites", this);
        MyMenu.addAction(pAction);
        connect(pAction, SIGNAL(triggered()), this, SLOT(ShowOldFavoritesDialog()));

        pAction = new QAction("Tuner", this);
        MyMenu.addAction(pAction);
        connect(pAction, SIGNAL(triggered()), m_TunerDialog, SLOT(ShowTunerDialog()));

//        pAction = new QAction("Loudspeaker Settings", this);
//        MyMenu.addAction(pAction);
//        connect(pAction, SIGNAL(triggered()), this, SLOT(ShowLoudspeakerSettingsDialog()));

//        pAction = new QAction("Test", this);
//        MyMenu.addAction(pAction);
//        connect(pAction, SIGNAL(triggered()), this, SLOT(ShowTestDialog()));

//        pAction = new QAction("More Information", this);
//        pAction = new QAction("Equalizer", this);
//        pAction = new QAction("LS Settings", this);
//        pAction = new QAction("Show Zone 2", this);
//        pAction = new QAction("Input Wizard", this);
    }
    pAction = new QAction("Settings", this);
    MyMenu.addAction(pAction);
    connect(pAction, SIGNAL(triggered()), this, SLOT(ShowSettingsDialog()));

    pAction = new QAction("About", this);
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
    ActionWithParameter* pAction;
    QMenu MyMenu(this);
    //MyMenu.addActions(this->actions());
    if (m_ReceiverOnline == true)
    {
        int i = 0;
        while (strlen(LISTENING_MODE[i].key) != 0)
        {
            pAction = new ActionWithParameter(this, LISTENING_MODE[i].text, LISTENING_MODE[i].key);
            connect(pAction, SIGNAL(ActionTriggered(QString)), this, SLOT(LMSelectedAction(QString)));
            MyMenu.addAction(pAction);
            i++;
        }
    }

    QPoint pos = QCursor::pos();
    MyMenu.exec(pos);

}

void AVRPioRemote::on_InputVideoButton_clicked()
{
    ui->InputVideoButton->setChecked(!ui->InputVideoButton->isChecked());
    SendCmd("10FN");
}
