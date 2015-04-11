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
#include "receiverinterface.h"

#include "Defs.h"
#include <QTextCodec>
#include <QStringList>
#include <QRegExp>

string trim(const string &t, const string &ws)
{
    string str = t;
    size_t found;
    found = str.find_last_not_of(ws);
    if (found != string::npos)
        str.erase(found+1);
    else
        return "";

    return str.erase(0, str.find_first_not_of(ws.c_str()));
}


ReceiverInterface::ReceiverInterface()
{
    m_Connected = false;

    // socket
    connect((&m_Socket), SIGNAL(connected()), this, SLOT(TcpConnected()));
    connect((&m_Socket), SIGNAL(disconnected()), this, SLOT(TcpDisconnected()));
    connect((&m_Socket), SIGNAL(readyRead()), this, SLOT(ReadString()));
    //connect((&m_Socket), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(SocketStateChanged(QAbstractSocket::SocketState)));
    connect((&m_Socket), SIGNAL(error(QAbstractSocket::SocketError)), this,  SLOT(TcpError(QAbstractSocket::SocketError)));
}


void ReceiverInterface::ConnectToReceiver(const QString& IpAddress, const int IpPort)
{
    m_Socket.connectToHost(IpAddress, IpPort);
}


void ReceiverInterface::Disconnect()
{
    m_Connected = false;
    m_Socket.disconnectFromHost();
    m_Socket.close();
}


void ReceiverInterface::TcpConnected()
{
    m_Connected = true;
    emit Connected();
}


void ReceiverInterface::TcpDisconnected()
{
    m_Connected = false;
    emit Disconnected();
}


bool ReceiverInterface::IsConnected()
{
    return m_Connected;
}


void ReceiverInterface::ReadString()
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
                MsgDistributor::NotifyListener(str);
                emit DataReceived(str);
            }
            m_ReceivedString = "";
        }
        lineStartPos = i + 1;
    }
    if (lineStartPos < count)
        m_ReceivedString.append((const char*)&data[lineStartPos]);
}


void ReceiverInterface::TcpError(QAbstractSocket::SocketError socketError)
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
//               "Make sure the receiver is on "
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


bool ReceiverInterface::SendCmd(const QString& cmd)
{
//    Log("--> " + cmd, QColor(0, 200, 0));
    CmdToBeSend(cmd);
    //Logger::Log("--> " + cmd);
    QString tmp = cmd + "\r";
    return m_Socket.write(tmp.toLatin1(), tmp.length()) == tmp.length();
}


void ReceiverInterface::InterpretString(const QString& data)
{
    if (data.startsWith("SS") || data.startsWith("CLV") || data.startsWith("SPK"))
    {
        emit SpeakerData(data);
    }
    else if (data.startsWith("AST"))
    {
        //AST030100000000000000001010000010000
        QString ast = data.mid(3);
        if (ast.length() >= 33)
        {
            QString codec, samplingRate;
            //int n = 0;

            // codec
            codec = FindValueByKey(AST1, ast.mid(0, 2));
            if (codec == "")
                codec = "???";

            // sampling rate
            samplingRate = FindValueByKey(AST3, ast.mid(2, 2));
            if (samplingRate == "")
                samplingRate = "???";

            QString iChFormat;
            QString oChFormat;
            QString oSamleRate;
            QString oBitDepth;
            QString pqls;
            QString phaseControl;
            QString phaseControlReversePhase;
            if (ast.length() >= 55) {
                // input channel format info
                iChFormat = ast.mid(4, 21);
                // output channel
                oChFormat = ast.mid(25, 18);
                // output sample rate
                oSamleRate = ast.mid(43, 2);
                // output bit depth
                oBitDepth = ast.mid(45, 2);
                // 47-50 reserved
                // working pqls
                pqls = ast.mid(51, 1);
                // auto phase control plus
                phaseControl = ast.mid(52, 2);
                // auto phase control plus reverse phase
                phaseControlReversePhase = ast.mid(54, 1);

                qDebug() << "Output sample rate" << oSamleRate;
                qDebug() << "Output bit depth" << oBitDepth;
                qDebug() << "PQLS" << pqls;
                qDebug() << "Phase Control Plus" << phaseControl;
                qDebug() << "Phase Control Plus (Reverse Phase)" << phaseControlReversePhase;
            } else if (ast.length() >= 43) {
                // input channel format info
                iChFormat = ast.mid(4, 21);
                // output channel
                oChFormat = ast.mid(25, 18);
            } else {
                // input channel format info
                iChFormat = ast.mid(4, 16);
                // output channel
                oChFormat = ast.mid(20, 13);
            }
            qDebug() << "Input  channel format" << iChFormat;
            qDebug() << "Output channel format" << oChFormat;
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
    else if (data.startsWith("SR"))
    {
        QString text = FindValueByKey(LISTENING_MODE, data.mid(2, 4));
        if (text == "")
            text = "---";
        //ui->lineEditListeningMode->setText(text);
        emit Listenextended(data.mid(2, 4), text);
    }
    else if (data.startsWith("LM"))
    {
        QString text = FindValueByKey(PLAYING_LISTENING_MODE, data.mid(2, 4));
        if (text == "")
            text = "---";
        emit ListeningModeData(text);
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
        emit ReceiverType(str1, str2);
    }
    else if (data.startsWith("SSO"))
    {
        QString name = data.mid(3);
        name = name.trimmed();
        while (name.startsWith("\""))
            name.remove(0, 1);
        while (name.endsWith("\""))
            name.chop(1);
        emit ReceiverNetworkName(name);
    }
    else if (data.startsWith("Z2F"))
    {
        int n = 0;
        sscanf(data.toLatin1(), "Z2F%d", &n);
        emit ZoneInput(2, n);
    }
    else if (data.startsWith("Z3F"))
    {
        int n = 0;
        sscanf(data.toLatin1(), "Z3F%d", &n);
        emit ZoneInput(3, n);
    }
    else if (data.startsWith("ZEA"))
    {
        int n = 0;
        sscanf(data.toLatin1(), "ZEA%d", &n);
        emit ZoneInput(4, n);
    }
    else if (data.startsWith("SUW"))
    {
        emit MCACCEQ(data.mid(3, 2).toInt(), data.mid(5, 3), data.mid(8, 2).toInt(), data.mid(10, 2).toInt());
    }
}

