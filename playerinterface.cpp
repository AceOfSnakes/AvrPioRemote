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
#include "playerinterface.h"
#include "Defs.h"
#include <QTextCodec>
#include <QStringList>
#include <QRegExp>


/*
string trim(const string &t, const string &ws)
{
    string str = t;
    size_t found;
    found = str.find_last_not_of(ws);
    if (found != string::npos)
        str.erase(found+1);
    else
        str.clear();            // str is all whitespace

    return str;
}
*/

PlayerInterface::PlayerInterface()
{
    m_Connected = false;

    // socket
    connect((&m_Socket), SIGNAL(connected()), this, SLOT(TcpConnected()));
    connect((&m_Socket), SIGNAL(disconnected()), this, SLOT(TcpDisconnected()));
    //connect((&m_Socket), SIGNAL(readyRead()), this, SLOT(ReadString()));
    //connect((&m_Socket), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SocketStateChanged(QAbstractSocket::SocketState)));
    connect((&m_Socket), SIGNAL(error(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));
}


void PlayerInterface::ConnectToPlayer(const QString& PlayerIpAddress, const int PlayerIpPort)
{
    m_Socket.connectToHost(PlayerIpAddress, PlayerIpPort);
}


void PlayerInterface::Disconnect()
{
    m_Connected = false;
    m_Socket.disconnectFromHost();
    m_Socket.close();
}


void PlayerInterface::TcpConnected()
{
      qDebug()<<"PlayerInterface::TcpConnected()";
    m_Connected = true;
    emit Connected();
}


void PlayerInterface::TcpDisconnected()
{
    m_Connected = false;
    emit Disconnected();
}


bool PlayerInterface::IsConnected()
{
    return m_Connected;
}

/*
void PlayerInterface::ReadString()
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
        if (data[i] != '\n' && data[i] != '\r')
        {
            continue;
        }
        lineLength = i - lineStartPos;
        if (lineLength > 0)
        {
            m_ReceivedString.append((const char*)&data[lineStartPos], 0, lineLength);
            m_ReceivedString = trim(m_ReceivedString, "\r");
            m_ReceivedString = trim(m_ReceivedString, "\n");
            if (m_ReceivedString != "")
            {
                QString str;
                //QTextCodec::setCodecForCStrings();
                str = str.fromUtf8(m_ReceivedString.c_str());

//                QByteArray encodedString = m_ReceivedString.c_str();
//                QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
//                str = codec->toUnicode(encodedString);
                //str = str.fromStdString(m_ReceivedString);
//                str = str.trimmed();
//                str.remove(QChar('\r'));
//                str.remove(QChar('\n'));
                InterpretString(str);
                emit DataReceived(str);
            }
            m_ReceivedString = "";
        }
        lineStartPos = i + 1;
    }
    if (lineStartPos < count)
        m_ReceivedString.append((const char*)&data[lineStartPos]);

//    tmp[count] = 0;
//    QString info = tmp.c_str();
//    if (tmp.length() > 1 && tmp[tmp.length() - 2] == '\r')
//        tmp[tmp.length() - 2] = 0;
}
*/

void PlayerInterface::TcpError(QAbstractSocket::SocketError socketError)
{
    //             QMessageBox::information(this, tr("Pioneer Remote"),
    //                                      tr("The host was not found. Please check the "
    //                                         "host name and port settings."));
    QString str;
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        str = QString("Host closed connection: %1").arg(m_Socket.errorString());
        break;
    case QAbstractSocket::HostNotFoundError:
        str = QString("Host not found: %1").arg(m_Socket.errorString());
        //Log(tr("The host was not found. Please check the host name and port settings."), QColor(255, 0, 0));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        str = QString("Host refused connection: %1").arg(m_Socket.errorString());
//        Log(tr("The connection was refused by the peer. "
//               "Make sure the Player is on "
//               "and check ip address and port."), QColor(255, 0, 0));
        break;
    default:
        str = QString("The following error occurred: %1.").arg(m_Socket.errorString());
        //Log(tr("The following error occurred: %1.").arg(m_Socket.errorString()), QColor(255, 0, 0));
    }
    emit CommError(str);
//    if (m_StartConnection)
//    {
//        Xsleep::msleep(10000);
//        ConnectTCP();
//    }
}


/*
QString PlayerInterface::DecodeHexString(const QString& hex)
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
*/

bool PlayerInterface::SendCmd(const QString& cmd)
{
//    Log("--> " + cmd, QColor(0, 200, 0));
    CmdToBeSend(cmd);
    Logger::Log("--> " + cmd);
    QString tmp = cmd + "\r";
    return m_Socket.write(tmp.toLatin1(), tmp.length()) == tmp.length();
}


/*
void PlayerInterface::InterpretString(const QString& data)
{
    // DISPLAY
    if (data.startsWith("SS") || data.startsWith("CLV") || data.startsWith("MC"))
    {
        QString name = data;
        emit SpeakerData(name);
    }
    else if (data.startsWith("AST"))
    {
        //AST030100000000000000001010000010000
        QString ast = data.mid(3);
        if (ast.length() >= 33)
        {
            QString codec, samplingRate;
            int n = 0;

            // codec
            n = ast.mid(0, 2).toInt();
            codec = (n >= 0 && n <= 27)?(AST1[n]):"unknown";

            // sampling rate
            n = ast.mid(2, 2).toInt();
            samplingRate = (n >= 0 && n <= 6)?(AST3[n]):"unknown";

            // channel format info data5-data20
            // output channel data21-33
            emit AudioStatusData(codec, samplingRate);
        }
    }
    else if (data.startsWith("VST"))
    {
        QString vst = data.mid(3);
        if (vst.length() >= 28)
        {
            int n = 0;

            // Terminal
            n = vst[0].toLatin1() - '0';
            //ui->lineEditVideoInputSignal->setText((n >= 0 && n <= 5)?(VIDEO_INPUT_TERMINAL[n]):"---");

            // Input Resolution
            n = vst.mid(1, 2).toInt();
            //ui->lineEditVideoInputResolution->setText((n >= 0 && n <= 11)?(VIDEO_INPUT_RESOLUTION[n]):"---");

            // Aspect
            n = vst[3].toLatin1() - '0';
            //ui->lineEditVideoInputAspect->setText((n >= 0 && n <= 3)?(VIDEO_INPUT_ASPECT[n]):"---");

            // Color Format (HDMI only)
            n = vst[4].toLatin1() - '0';
            //ui->lineEditVideoInputColorFormat->setText((n >= 0 && n <= 4)?(VIDEO_INPUT_COLOR_FORMAT[n]):"---");

            // Bits (HDMI only)
            n = vst[5].toLatin1() - '0';
            //ui->lineEditVideoInputBits->setText((n >= 0 && n <= 4)?(VIDEO_INPUT_BITS[n]):"---");

            // Color Space (HDMI only)
            n = vst[6].toLatin1() - '0';
            //ui->lineEditVideoInputColorSpace->setText((n >= 0 && n <= 6)?(VIDEO_INPUT_COLOR_SPACE[n]):"---");

            // Output Resolution
            n = vst.mid(7, 2).toInt();
            //ui->lineEditVideoOutputResolution->setText((n >= 0 && n <= 11)?(VIDEO_INPUT_RESOLUTION[n]):"---");

            // Output Aspect
            n = vst[9].toLatin1() - '0';
            //ui->lineEditVideoOutputAspect->setText((n >= 0 && n <= 3)?(VIDEO_INPUT_ASPECT[n]):"---");

            // Output Color Format (HDMI only)
            n = vst[10].toLatin1() - '0';
            //ui->lineEditVideoOutputColorFormat->setText((n >= 0 && n <= 4)?(VIDEO_INPUT_COLOR_FORMAT[n]):"---");

            // Output Bits (HDMI only)
            n = vst[11].toLatin1() - '0';
            //ui->lineEditVideoOutputBits->setText((n >= 0 && n <= 4)?(VIDEO_INPUT_BITS[n]):"---");

            // Output Color Space (HDMI only)
            n = vst[12].toLatin1() - '0';
            //ui->lineEditVideoOutputColorSpace->setText((n >= 0 && n <= 6)?(VIDEO_INPUT_COLOR_SPACE[n]):"---");

            // HDMI out 1 Recommended Resolution
            n = vst.mid(13, 2).toInt();
            //ui->lineEditVideoOut1Resolution->setText((n >= 0 && n <= 11)?(VIDEO_INPUT_RESOLUTION[n]):"---");

            // HDMI out 1 Deep Color
            n = vst[15].toLatin1() - '0';
            //ui->lineEditVideoOut1ColorDepth->setText((n >= 0 && n <= 4)?(VIDEO_INPUT_BITS[n]):"---");

            // HDMI out 1 Color Space data17-21

            // HDMI out 2 Recommended Resolution
            n = vst.mid(21, 2).toInt();
            //ui->lineEditVideoOut2Resolution->setText((n >= 0 && n <= 11)?(VIDEO_INPUT_RESOLUTION[n]):"---");

            // HDMI out 2 Deep Color
            n = vst[22].toLatin1() - '0';
            //ui->lineEditVideoOut2ColorDepth->setText((n >= 0 && n <= 4)?(VIDEO_INPUT_BITS[n]):"---");

            // HDMI out 1 Color Space data25-29
        }
    }
    else if (data.startsWith("FN"))
    {
        int n = 0;
        sscanf(data.toLatin1(), "FN%d", &n);
        QString str = (n >= 0 && n <= 48)?(VIDEO_INPUT[n]):"unknown";
        if (str == "")
            str = "unknown";
        emit InputChanged(n, str);
    }
    else if (data.startsWith("SR"))
    {
        QString text = FindString(LISTENING_MODE, data.mid(2, 4));
        if (text == "")
            text = "---";
        //ui->lineEditListeningMode->setText(text);
        emit Listenextended(data.mid(2, 4), text);
    }
    else if (data.startsWith("LM"))
    {
        QString text = FindString(PLAYING_LISTENING_MODE, data.mid(2, 4));
        if (text == "")
            text = "---";
        emit ListeningModeData(text);
    }
    else if (data.startsWith("TO0"))
    {
        //ui->pushButtonToneOn->setText("BYPASS");
        //ui->pushButtonToneOn->setChecked(false);
        //ui->pushButtonBassDown->setEnabled(false);
        //ui->pushButtonBassUp->setEnabled(false);
        //ui->pushButtonTrebleDown->setEnabled(false);
        //ui->pushButtonTrebleUp->setEnabled(false);
    }
    else if (data.startsWith("TO1"))
    {
        //ui->pushButtonToneOn->setText("ON");
        //ui->pushButtonToneOn->setChecked(true);
        //ui->pushButtonBassDown->setEnabled(true);
        //ui->pushButtonBassUp->setEnabled(true);
        //ui->pushButtonTrebleDown->setEnabled(true);
        //ui->pushButtonTrebleUp->setEnabled(true);
    }
    else if (data.startsWith("BA"))
    {
        double vol = data.mid(2, 2).toDouble();
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
    else if (data.startsWith("TR"))
    {
        double vol = data.mid(2, 2).toDouble();
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
    else if (data.startsWith("GBH") ||
             data.startsWith("GCH") ||
             data.startsWith("GDH") ||
             data.startsWith("GEH") ||
             data.startsWith("GHH"))
    {
        emit NetData(data);
    }
    else if (data.startsWith("GBI") ||
             data.startsWith("GCI") ||
             data.startsWith("GDI") ||
             data.startsWith("GEI"))
    {
        emit usbData(data);
    }
    else if (data.startsWith("RGD"))
    {
        QStringList l = data.mid(3).split(QRegExp("[<>]"), QString::SkipEmptyParts);
        QString str1, str2;
        if (l.count() > 0)
            str1 = l[0];
        if (l.count() > 1)
            str2 = l[1];
        emit PlayerType(str1, str2);
    }
    else if (data.startsWith("SSO"))
    {
        QString name = data.mid(3);
        name = name.trimmed();
        while (name.startsWith("\""))
            name.remove(0, 1);
        while (name.endsWith("\""))
            name.chop(1);
        emit PlayerNetworkName(name);
    }
}
*/
