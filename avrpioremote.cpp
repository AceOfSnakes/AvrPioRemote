#include "avrpioremote.h"
#include "ui_avrpioremote.h"
#include <QDebug>
//#include <QLayout>
#include <qtextcodec.h>

AVRPioRemote::AVRPioRemote(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AVRPioRemote),
    m_IpValidator(0, 255, this),
    m_IpPortValidator(0, 35535, this),
    m_Settings("AMCore", "AVRPioRemote")
{
    m_Connected = false;
    m_Port = 8102;

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

    // socket
    connect((&m_Socket), SIGNAL(connected()), this, SLOT(TcpConnected()));
    connect((&m_Socket), SIGNAL(disconnected()), this, SLOT(TcpDisconnected()));
    connect((&m_Socket), SIGNAL(readyRead()), this, SLOT(ReadString()));
    //connect((&m_Socket), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SocketStateChanged(QAbstractSocket::SocketState)));
    connect((&m_Socket), SIGNAL(error(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));
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

    ui->Num1Button->setText("AUTO/ALC/\nDIRECT");

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
    /* 10 */ m_InputButtons.append(NULL); //"VIDEO 1(VIDEO)",
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

    m_NetRadioDialog = new NetRadioDialog(this);
    connect(this, SIGNAL(NetData(QString)), this, SLOT(ShowNetDialog()));

    m_LoudspeakerSettingsDialog = new LoudspeakerSettingsDialog(this);
    connect(m_LoudspeakerSettingsDialog, SIGNAL(SendCmd(QString)), this, SLOT(SendCmd(QString)));
}

AVRPioRemote::~AVRPioRemote()
{
    delete ui;
}

void AVRPioRemote::closeEvent(QCloseEvent *event)
{
    m_Socket.close();
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
        ShowNetDialog();
    }
    else
    {
        if (m_NetRadioDialog->isVisible())
            m_NetRadioDialog->hide();
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

LMAction::LMAction(const QString& Text, const QString& Number, QObject* Parent)
    : QAction(Text, Parent)
{
    m_Number = Number;
    connect(this, SIGNAL(triggered()), this, SLOT(ActionTriggered()));
    connect(this, SIGNAL(SendCmd(QString)), Parent, SLOT(SendCmd(QString)));
}

LMAction::~LMAction()
{

}

void LMAction::ActionTriggered()
{
    QString cmd = QString("%1SR").arg(m_Number);

    emit SendCmd(cmd);
}

void AVRPioRemote::ConnectTCP()
{
    ui->pushButtonConnect->setEnabled(false);
    EnableIPInput(false);

    ui->StausLineEdit->setText("Connecting...");

//    ConnectWorker* worker = new ConnectWorker(m_Socket, m_Ip, m_Port, this->thread());
//    worker->moveToThread(m_TCPThread);
//    m_Socket.moveToThread(m_TCPThread);
//    connect(worker, SIGNAL(error(QString)), this, SLOT(ConnectWorkerErrorString(QString)));
//    connect(m_TCPThread, SIGNAL(started()), worker, SLOT(process()));
//    connect(worker, SIGNAL(finished()), m_TCPThread, SLOT(quit()));
//    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
//    connect(m_TCPThread, SIGNAL(finished()), m_TCPThread, SLOT(deleteLater()));
//    connect(worker, SIGNAL(finished()), this, SLOT(onConnectWorkerFinished()));
//    m_TCPThread->start();

    //if (m_Socket.state() == QAbstractSocket::HostLookupState && m_Socket.state() != QAbstractSocket::ConnectingState)
    m_Socket.connectToHost(m_Ip, m_Port);

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

void AVRPioRemote::ReadString()
{
    // read all available data
    int count = m_Socket.bytesAvailable();
    std::vector<char> data;
    data.resize(count + 1);
    m_Socket.read(&data[0], count);
    data[count] = '\0';

    // split lines
    int lineLength = 0;
    int lineStartPos = 0;
    for(int i = 0; i < count; i++)
    {
        if (data[i] != '\n')
        {
            continue;
        }
        lineLength = i - lineStartPos;
        if (lineLength > 0)
        {
            data[i] = '\0';
            m_RestString.append((const char*)&data[lineStartPos]);
            //qDebug(str.toAscii());
            if (m_RestString.trimmed() != "")
            {
                Log("<-- " + m_RestString, QColor(0, 200, 0));
                InterpretString(m_RestString);
            }
            m_RestString = "";
        }
        lineStartPos = i + 1;
    }
    if (lineStartPos < count)
        m_RestString.append((const char*)&data[lineStartPos]);

//    tmp[count] = 0;
//    QString info = tmp.c_str();
//    if (tmp.length() > 1 && tmp[tmp.length() - 2] == '\r')
//        tmp[tmp.length() - 2] = 0;
}

void AVRPioRemote::InterpretString(const QString& str)
{
    //log(str.toAscii());
    // DISPLAY
    if (str.startsWith("FL02"))
    {
        QString displayData = DecodeHexString(str.mid(4));
        //qDebug() << displayData;
        ui->ScreenLineEdit1->setText(displayData);
    }
    else if (str.startsWith("FL00"))
    {
        QString displayData = DecodeHexString(str.mid(4));
        //qDebug() << displayData;
        ui->ScreenLineEdit1->setText(displayData);
    }
    else if (str.startsWith("FL03"))
    {
        QString displayData = DecodeHexString(str.mid(4));
        //qDebug() << displayData;
        ui->ScreenLineEdit1->setText(displayData);
    }
    else if (str.startsWith("PWR0"))
    {
        //qDebug("PWR ON");
        ui->PowerButton->setChecked(true);
        ui->PowerButton->setText("ON");
        EnableControls(true);
        m_ReceiverOnline = true;
        //RequestStatus(true);
    }
    else if (str.startsWith("PWR1"))
    {
        ui->PowerButton->setChecked(false);
        ui->PowerButton->setText("OFF");
        EnableControls(false);
        m_ReceiverOnline = false;
    }
    else if (str.startsWith("VOL"))
    {
        double vol = str.mid(3, 3).toDouble();
        QString str;
        if (vol <= 0)
            str = "---.---";
        else
        {
            double dB = -80.5 + vol * 0.5;
            if (dB <= 0.0)
                str = QString("%1dB").arg(dB, 4, 'f', 1);
            else
                str = QString("+%1dB").arg(dB, 4, 'f', 1);
        }
        ui->VolumeLineEdit->setText(str);
        //ui->lineEditVolume->setText(info.mid(3, 3));
    }
    else if (str.startsWith("MUT0"))
    {
        ui->VolumeMuteButton->setChecked(true);
    }
    else if (str.startsWith("MUT1"))
    {
        ui->VolumeMuteButton->setChecked(false);
    }
    else if (str.startsWith("E02"))
    {
        Log("This doesn't work now");
        ui->StausLineEdit->setText("This doesn't work now");
    }
    else if (str.startsWith("E03"))
    {
        Log("This does'nt work with this receiver");
        ui->StausLineEdit->setText("This doesn't work with this receiver");
    }
    else if (str.startsWith("E04"))
    {
        Log("Command error");
        ui->StausLineEdit->setText("Command error");
    }
    else if (str.startsWith("E06"))
    {
        Log("Parameter error");
        ui->StausLineEdit->setText("Parameter error");
    }
    else if (str.startsWith("B00"))
    {
        Log("Receiver busy");
        ui->StausLineEdit->setText("Receiver busy");
    }
    else if (str.startsWith("AST"))
    {
        //AST030100000000000000001010000010000
        QString ast = str.mid(3);
        if (ast.length() >= 33)
        {
            int n = 0;

            // signal
            n = ast.mid(0, 2).toInt();
            ui->AudioCodecLineEdit->setText((n >= 0 && n <= 27)?(AST1[n]):"unknown");
            //qDebug() << n;
            qDebug() << "AST1: " << ((n >= 0 && n <= 27)?(AST1[n]):"unknown");

            //frequency
            n = ast.mid(2, 2).toInt();
            ui->AudioSampleRateLineEdit->setText((n >= 0 && n <= 6)?(AST3[n]):"unknown");
            //qDebug() << n;
            qDebug() << "AST3: " << ((n >= 0 && n <= 6)?(AST3[n]):"unknown");
            // channel format info data5-data20
            // output channel data21-33
        }
    }
    else if (str.startsWith("VST"))
    {
        QString vst = str.mid(3);
        if (vst.length() >= 28)
        {
            int n = 0;

            // Terminal
            n = vst[0].toAscii() - '0';
            //ui->lineEditVideoInputSignal->setText((n >= 0 && n <= 5)?(VIDEO_INPUT_TERMINAL[n]):"---");

            // Input Resolution
            n = vst.mid(1, 2).toInt();
            //ui->lineEditVideoInputResolution->setText((n >= 0 && n <= 11)?(VIDEO_INPUT_RESOLUTION[n]):"---");

            // Aspect
            n = vst[3].toAscii() - '0';
            //ui->lineEditVideoInputAspect->setText((n >= 0 && n <= 3)?(VIDEO_INPUT_ASPECT[n]):"---");

            // Color Format (HDMI only)
            n = vst[4].toAscii() - '0';
            //ui->lineEditVideoInputColorFormat->setText((n >= 0 && n <= 4)?(VIDEO_INPUT_COLOR_FORMAT[n]):"---");

            // Bits (HDMI only)
            n = vst[5].toAscii() - '0';
            //ui->lineEditVideoInputBits->setText((n >= 0 && n <= 4)?(VIDEO_INPUT_BITS[n]):"---");

            // Color Space (HDMI only)
            n = vst[6].toAscii() - '0';
            //ui->lineEditVideoInputColorSpace->setText((n >= 0 && n <= 6)?(VIDEO_INPUT_COLOR_SPACE[n]):"---");

            // Output Resolution
            n = vst.mid(7, 2).toInt();
            //ui->lineEditVideoOutputResolution->setText((n >= 0 && n <= 11)?(VIDEO_INPUT_RESOLUTION[n]):"---");

            // Output Aspect
            n = vst[9].toAscii() - '0';
            //ui->lineEditVideoOutputAspect->setText((n >= 0 && n <= 3)?(VIDEO_INPUT_ASPECT[n]):"---");

            // Output Color Format (HDMI only)
            n = vst[10].toAscii() - '0';
            //ui->lineEditVideoOutputColorFormat->setText((n >= 0 && n <= 4)?(VIDEO_INPUT_COLOR_FORMAT[n]):"---");

            // Output Bits (HDMI only)
            n = vst[11].toAscii() - '0';
            //ui->lineEditVideoOutputBits->setText((n >= 0 && n <= 4)?(VIDEO_INPUT_BITS[n]):"---");

            // Output Color Space (HDMI only)
            n = vst[12].toAscii() - '0';
            //ui->lineEditVideoOutputColorSpace->setText((n >= 0 && n <= 6)?(VIDEO_INPUT_COLOR_SPACE[n]):"---");

            // HDMI out 1 Recommended Resolution
            n = vst.mid(13, 2).toInt();
            //ui->lineEditVideoOut1Resolution->setText((n >= 0 && n <= 11)?(VIDEO_INPUT_RESOLUTION[n]):"---");

            // HDMI out 1 Deep Color
            n = vst[15].toAscii() - '0';
            //ui->lineEditVideoOut1ColorDepth->setText((n >= 0 && n <= 4)?(VIDEO_INPUT_BITS[n]):"---");

            // HDMI out 1 Color Space data17-21

            // HDMI out 2 Recommended Resolution
            n = vst.mid(21, 2).toInt();
            //ui->lineEditVideoOut2Resolution->setText((n >= 0 && n <= 11)?(VIDEO_INPUT_RESOLUTION[n]):"---");

            // HDMI out 2 Deep Color
            n = vst[22].toAscii() - '0';
            //ui->lineEditVideoOut2ColorDepth->setText((n >= 0 && n <= 4)?(VIDEO_INPUT_BITS[n]):"---");

            // HDMI out 1 Color Space data25-29
        }
    }
    else if (str.startsWith("FN"))
    {
        int n = str.mid(2, 2).toInt();
        QString str = (n >= 0 && n <= 48)?(VIDEO_INPUT[n]):"unknown";
        if (str == "")
            str = "unknown";
        ui->InputLineEdit->setText(str);
        SelectInputButton(n);
        str = QString("?RGB%1").arg(n, 2, 10, QLatin1Char('0'));
        SendCmd(str.toAscii());
        RequestStatus(false);
        //SendCmd("?AST"); // request audio information
        //SendCmd("?VST"); // request video information
    }
    else if (str.startsWith("RGB"))
    {
        //bool renamed = (tmp[5] == '1');
        QString name = str.mid(6);
        ui->InputNameLineEdit->setText(name);
    }
    else if (str.startsWith("SR"))
    {
        QString text = FindString(LISTENING_MODE, str.mid(2, 4));
        if (text == "")
            text = "---";
        //ui->lineEditListeningMode->setText(text);
    }
    else if (str.startsWith("LM"))
    {
        QString text = FindString(PLAYING_LISTENING_MODE, str.mid(2, 4));
        if (text == "")
            text = "---";
        ui->ListeningModeLineEdit->setText(text);
    }
    else if (str.startsWith("TO0"))
    {
        //ui->pushButtonToneOn->setText("BYPASS");
        //ui->pushButtonToneOn->setChecked(false);
        //ui->pushButtonBassDown->setEnabled(false);
        //ui->pushButtonBassUp->setEnabled(false);
        //ui->pushButtonTrebleDown->setEnabled(false);
        //ui->pushButtonTrebleUp->setEnabled(false);
    }
    else if (str.startsWith("TO1"))
    {
        //ui->pushButtonToneOn->setText("ON");
        //ui->pushButtonToneOn->setChecked(true);
        //ui->pushButtonBassDown->setEnabled(true);
        //ui->pushButtonBassUp->setEnabled(true);
        //ui->pushButtonTrebleDown->setEnabled(true);
        //ui->pushButtonTrebleUp->setEnabled(true);
    }
    else if (str.startsWith("BA"))
    {
        double vol = str.mid(2, 2).toDouble();
        QString str;
        if (vol < 0)
            str = "---.---";
        else
        {
            double dB = 6.0 - vol * 1.0;
            if (dB <= 0.0)
                str = QString("%1dB").arg(dB, 4, 'f', 1);
            else
                str = QString("+%1dB").arg(dB, 4, 'f', 1);
        }
        //ui->lineEditBass->setText(str);
    }
    else if (str.startsWith("TR"))
    {
        double vol = str.mid(2, 2).toDouble();
        QString str;
        if (vol < 0)
            str = "---.---";
        else
        {
            double dB = 6.0 - vol * 1.0;
            if (dB <= 0.0)
                str = QString("%1dB").arg(dB, 4, 'f', 1);
            else
                str = QString("+%1dB").arg(dB, 4, 'f', 1);
        }
        //ui->lineEditTreble->setText(str);
    }
    else if (str.startsWith("IS0"))
    {
        ui->PhaseButton->setChecked(false);
    }
    else if (str.startsWith("IS1") ||
             str.startsWith("IS2") ||
             str.startsWith("IS9"))
    {
        ui->PhaseButton->setChecked(true);
    }
    else if (str.startsWith("ATI0"))
    {
        ui->HiBitButton->setChecked(false);
    }
    else if (str.startsWith("ATI1"))
    {
        ui->HiBitButton->setChecked(true);
    }
    else if (str.startsWith("PQ0"))
    {
        ui->PqlsButton->setChecked(false);
    }
    else if (str.startsWith("PQ1"))
    {
        ui->PqlsButton->setChecked(true);
    }
    else if (str.startsWith("GBH") ||
             str.startsWith("GCH") ||
             str.startsWith("GDH") ||
             str.startsWith("GEH") ||
             str.startsWith("GHH"))
    {
        emit NetData(str);
    }
    else if (str.startsWith("ATA0"))
    {
        ui->DFiltButton->setChecked(false);
    }
    else if (str.startsWith("ATA1"))
    {
        ui->DFiltButton->setChecked(true);
    }
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

void AVRPioRemote::TcpError(QAbstractSocket::SocketError socketError)
{
    //             QMessageBox::information(this, tr("Pioneer Remote"),
    //                                      tr("The host was not found. Please check the "
    //                                         "host name and port settings."));
    Log("tcp error");
    ui->pushButtonConnect->setEnabled(true);
    EnableIPInput(true);
    EnableControls(false);
    m_Connected = false;
    ui->pushButtonConnect->setText("Connect");
    ClearScreen();
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        ui->StausLineEdit->setText("Host closed connection");
        break;
    case QAbstractSocket::HostNotFoundError:
        ui->StausLineEdit->setText("Host not found");
        Log(tr("The host was not found. Please check the host name and port settings."), QColor(255, 0, 0));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        ui->StausLineEdit->setText("Host refused connection");
        Log(tr("The connection was refused by the peer. "
               "Make sure the fortune server is running, "
               "and check that the host name and port "
               "settings are correct."), QColor(255, 0, 0));
        break;
    default:
        ui->StausLineEdit->setText(m_Socket.errorString());
        Log(tr("The following error occurred: %1.").arg(m_Socket.errorString()), QColor(255, 0, 0));
    }
//    if (m_StartConnection)
//    {
//        Xsleep::msleep(10000);
//        ConnectTCP();
//    }
}

void AVRPioRemote::TcpConnected()
{
    Log("connected");
    ui->PowerButton->setEnabled(true);
    ui->StausLineEdit->setText("Connected");
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonConnect->setText("Disconnect");
    m_Connected = true;
    m_ReceiverOnline = true;
    RequestStatus();
}

void AVRPioRemote::TcpDisconnected()
{
    Log("disconnected");
    EnableIPInput(true);
    m_Connected = false;
    ui->pushButtonConnect->setText("Connect");
    ui->pushButtonConnect->setEnabled(true);
    EnableControls(false);
    ClearScreen();
}

QString AVRPioRemote::DecodeHexString(const QString& hex)
{
    QString str = "";
    for (int i = 0; i < (int)hex.length(); i+=2)
    {
        int c = hex.mid(i, 2).toInt(NULL, 16);
        if (c == 5)
            str += "[)";
        else if (c == 6)
            str += "(]";
        else if (c == 9)
            str += "<|";
        else if (c == 10)
            str += "|>";
        else
            str += (QChar)c;
    }
    return str;
}

void AVRPioRemote::Log(const QString& text)
{
    Log(text, QColor(0, 0, 0));
}

void AVRPioRemote::Log(const QString& text, const QColor& color)
{
//    ui->listWidgetLog->addItem(text);
//    if (ui->listWidgetLog->count() > 100)
//        ui->listWidgetLog->removeItemWidget(ui->listWidgetLog->item(0));
//    ui->listWidgetLog->setCurrentItem(ui->listWidgetLog->item(ui->listWidgetLog->count() - 1));
//    QBrush brush(color);
//    int index = ui->listWidgetLog->currentIndex().row();
//    ui->listWidgetLog->item(index)->setForeground(brush);
    qDebug() << text;
}

bool AVRPioRemote::SendCmd(const QString& cmd)
{
    Log("--> " + cmd, QColor(0, 200, 0));
    QString tmp = cmd + "\r";
    return m_Socket.write(tmp.toAscii(), tmp.length()) == tmp.length();
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
    if (!m_Connected)
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
        m_Ip = ip1 + "." + ip2 + "." + ip3 + "." + ip4;
        ip_port = ui->lineEditIPPort->text().trimmed();
        if (ip_port == "")
        {
            ip_port = "8102";
            m_Port = ip_port.toInt();
            ui->lineEditIPPort->setText(ip_port);
        }
        m_Settings.setValue("IP/1", ip1);
        m_Settings.setValue("IP/2", ip2);
        m_Settings.setValue("IP/3", ip3);
        m_Settings.setValue("IP/4", ip4);
        m_Settings.setValue("IP/PORT", ip_port);
//        ui->lineEditIP1->setEnabled(false);
//        ui->lineEditIP2->setEnabled(false);
//        ui->lineEditIP3->setEnabled(false);
//        ui->lineEditIP4->setEnabled(false);
//        ui->lineEditIPPort->setEnabled(false);
        ConnectTCP();
    }
    else
    {
        EnableControls(false);
        ui->PowerButton->setEnabled(false);

//        ui->lineEditIP1->setEnabled(true);
//        ui->lineEditIP2->setEnabled(true);
//        ui->lineEditIP3->setEnabled(true);
//        ui->lineEditIP4->setEnabled(true);
//        ui->lineEditIPPort->setEnabled(true);
        //m_Socket.abort();
        m_Socket.disconnectFromHost();
        m_Socket.close();
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

void AVRPioRemote::ShowAboutDialog()
{
    AboutDialog* about = new AboutDialog(this);
    about->show();
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

        pAction = new QAction("Loudspeaker Settings", this);
        MyMenu.addAction(pAction);
        connect(pAction, SIGNAL(triggered()), this, SLOT(ShowLoudspeakerSettingsDialog()));

//        pAction = new QAction("More Information", this);
//        pAction = new QAction("Equalizer", this);
//        pAction = new QAction("LS Settings", this);
//        pAction = new QAction("Show Zone 2", this);
//        pAction = new QAction("Input Wizard", this);
    }
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
    LMAction* pAction;
    QMenu MyMenu(this);
    //MyMenu.addActions(this->actions());
    if (m_ReceiverOnline == true)
    {
        int i = 0;
        while (strlen(LISTENING_MODE[i].key) != 0)
        {
            pAction = new LMAction(LISTENING_MODE[i].text, LISTENING_MODE[i].key, this);
            MyMenu.addAction(pAction);
            i++;
        }
    }

    QPoint pos = QCursor::pos();
    MyMenu.exec(pos);

}
