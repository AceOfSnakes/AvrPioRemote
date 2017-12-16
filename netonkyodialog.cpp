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
#include "netonkyodialog.h"
#include "ui_netonkyodialog.h"
#include <QDebug>
#include <qtextcodec.h>
#include <QDateTime>
#include <QClipboard>
#include <QDomDocument>
#include <QDomNamedNodeMap>
#include <QMapIterator>

#define ROLE_LINENUMBER Qt::UserRole

NetOnkyoDialog::NetOnkyoDialog(QWidget *parent, QSettings &settings, ReceiverInterface &Comm) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::NetOnkyoDialog),
    m_Comm(Comm),
    m_PositionSet(false)
    //m_PlayTimeTimer(this)
{
    m_IndexOfLine1 = 0;
    m_IndexOfLastLine = 0;
    m_TotalNumberOfItems = 0;
    m_LayerNumber = -1;
    m_SelectedItemIndex = 0;
    m_VisibleListSize = 0;
    m_ScreenType = -1;
    m_ListUpdateFlag = 0;
    //m_PlayTime = 0;
    m_PlayFormat = "-";
    m_PlayBitrate = "-";
    m_NewDataFormat = false;

    ui->setupUi(this);
    ui->label_5->setVisible(false);
    ui->label_7->setVisible(false);
    ui->GenreLabel->setVisible(false);
    ui->FormatLabel->setVisible(false);

    ui->DisplayWidget->setCurrentIndex(0);

    // restore the position of the window
    if (m_Settings.value("SaveNetRadioWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("NetRadioWindowGeometry").toByteArray());
    }

    connect((&m_Comm), SIGNAL(NetData(QString)), this, SLOT(NetData(QString)));
    connect((this),    SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));

    MsgDistributor::AddResponseListener(this, QStringList() << InputFunctionResponse_FN().getResponseID());

    connect((&m_Timer), SIGNAL(timeout()), this, SLOT(Timeout()));
    //connect((&m_PlayTimeTimer), SIGNAL(timeout()), this, SLOT(RefreshPlayTime()));

    // directory
    QIcon icon1(QString::fromUtf8(":/new/prefix1/images/Gnome-folder-open.png"));
    m_Icons.insert(1, icon1);
    // audio
    QIcon icon2(QString::fromUtf8(":/new/prefix1/images/Gnome-audio-x-generic.png"));
    m_Icons.insert(2, icon2);
    // photo
    QIcon icon3(QString::fromUtf8(":/new/prefix1/images/Gnome-emblem-photos.png"));
    m_Icons.insert(3, icon3);
    // now playing
    QIcon icon4(QString::fromUtf8(":/new/prefix1/images/Gnome-media-playback-start.png"));
    m_Icons.insert(4, icon4);
    // video
    QIcon icon5(QString::fromUtf8(":/new/prefix1/images/Gnome-video-x-generic.png"));
    m_Icons.insert(5, icon5);

    m_DirectInputs.append(DirectInput(0x0E, "TuneIn Radio"));
    m_DirectInputs.append(DirectInput(0x04, "Pandora"));
    m_DirectInputs.append(DirectInput(0x0A, "Spotify"));
    m_DirectInputs.append(DirectInput(0x12, "Deezer"));
    m_DirectInputs.append(DirectInput(0x19, "TIDAL"));
    // fire connect
    // chromcast
    // play-fi dts
    //
    m_DirectInputs.append(DirectInput(0x18, "Airplay"));
    m_DirectInputs.append(DirectInput(0x00, "Music Server (DLNA)"));

    m_DirectInputs.append(DirectInput(0x02, "vTuner"));
    m_DirectInputs.append(DirectInput(0x03, "SiriusXM"));
    m_DirectInputs.append(DirectInput(0x05, "Rhapsody"));
    m_DirectInputs.append(DirectInput(0x06, "Last.fm"));
    m_DirectInputs.append(DirectInput(0x07, "Napster"));
    m_DirectInputs.append(DirectInput(0x08, "Slacker"));
    m_DirectInputs.append(DirectInput(0x09, "Mediafly"));
    m_DirectInputs.append(DirectInput(0x0B, "AUPEO!"));
    m_DirectInputs.append(DirectInput(0x0C, "Radiko"));
    m_DirectInputs.append(DirectInput(0x0D, "e-onkyo"));
    m_DirectInputs.append(DirectInput(0x0F, "mp3tunes"));
    m_DirectInputs.append(DirectInput(0x10, "Simfy"));
    m_DirectInputs.append(DirectInput(0x11, "Home Media"));
    m_DirectInputs.append(DirectInput(0x13, "iHeartRadio"));
    m_DirectInputs.append(DirectInput(0x1A, "onkyo music"));
    m_DirectInputs.append(DirectInput(0xF0, "USB/USB(Front)"));
    m_DirectInputs.append(DirectInput(0xF1, "USB(Rear)"));

    m_Timer.setSingleShot(false);
    m_Timer.setInterval(10000);
    //Timeout();

    //m_PlayTimeTimer.setInterval(200);
}


NetOnkyoDialog::~NetOnkyoDialog()
{
    delete ui;
}


void NetOnkyoDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("NetRadioWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}

void NetOnkyoDialog::ManualShowNetDialog()
{
    ShowNetDialog(false);
}

void NetOnkyoDialog::ShowNetDialog(bool autoShow)
{   
    if (!m_Comm.IsPioneer() && ((!autoShow) || (m_Settings.value("AutoShowNetRadio", true).toBool() && !isVisible())))
    {
        emit SendCmd("NLTQSTN");
        m_ScreenType = -1;
        if (!m_PositionSet || !m_Settings.value("SaveNetRadioWindowGeometry", false).toBool())
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
        m_Timer.start();
        ui->ArtistLabel->setText("");
        ui->AlbumLabel->setText("");
        ui->TitleLabel->setText("");
        show();
     }
}

void NetOnkyoDialog::ResponseReceived(ReceivedObjectBase *response)
{
//    InputFunctionResponse_FN* inputFunction = dynamic_cast<InputFunctionResponse_FN*>(response);
//    if (inputFunction != NULL)
//    {
//        int no = inputFunction->getNumber();
//        if (no == 26 || // NET cyclic
//            no == 38 || // internet radio
//            no == 40 || // sirius xm
//            no == 41 || // pandora
//            no == 44 || // media server
//            no == 45    // favorites
//                )
//        {
//            if (no == 45)
//            {
//                ui->NetAddFavButton->setEnabled(false);
//                ui->NetRemoveFavButton->setEnabled(true);
//                ui->NetSwitchToFavoritesButton->setChecked(true);
//                ui->NetSwitchToMediaServerButton->setChecked(false);
//                ui->NetSwitchToNetRadioButton->setChecked(false);
//            }
//            else
//            {
//                ui->NetAddFavButton->setEnabled(true);
//                ui->NetRemoveFavButton->setEnabled(false);
//                if (no == 38)
//                {
//                    ui->NetSwitchToFavoritesButton->setChecked(false);
//                    ui->NetSwitchToMediaServerButton->setChecked(false);
//                    ui->NetSwitchToNetRadioButton->setChecked(true);
//                }
//                else if (no == 44)
//                {
//                    ui->NetSwitchToFavoritesButton->setChecked(false);
//                    ui->NetSwitchToMediaServerButton->setChecked(true);
//                    ui->NetSwitchToNetRadioButton->setChecked(false);
//                }
//                else
//                {
//                    ui->NetSwitchToFavoritesButton->setChecked(false);
//                    ui->NetSwitchToMediaServerButton->setChecked(false);
//                    ui->NetSwitchToNetRadioButton->setChecked(false);
//                }
//            }
//            this->setWindowTitle(inputFunction->getName());
//        }
//        else
//        {
//            this->setWindowTitle(tr("NetRadio"));
//        }
//    }
}


void NetOnkyoDialog::Timeout()
{
    if(isVisible())
    {
        if (!m_Comm.IsPioneer() && m_ScreenType == 2) // playback
        {
            SendCmd("NATQSTN");
            SendCmd("NALQSTN");
            SendCmd("NTIQSTN");
        }
    }
}


//void NetOnkyoDialog::RefreshPlayTime()
//{
//    if (m_Comm.IsPioneer())
//    {
//        if (m_ScreenType >= 2 && m_ScreenType <= 5)
//        {
//            uint64_t tmp = QDateTime::currentMSecsSinceEpoch() / 1000ULL - m_PlayTime;
//            uint64_t hour = tmp / 3600ULL;
//            tmp = tmp - hour * 3600;
//            uint64_t min = tmp / 60;
//            uint64_t sec = tmp % 60;
//            QString timeString;
//            if (hour > 0)
//            {
//                timeString = QString("%1:%2:%3").arg(hour, 2, 10, QChar('0')).arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
//            }
//            else
//            {
//                timeString = QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
//            }
//            if (m_TotalTime != "" && m_TotalTime != "0:00")
//                timeString += " (" + m_TotalTime + ")";
//            ui->TimeLabel->setText(timeString);
//        }
//        else
//        {
//            m_PlayTimeTimer.stop();
//        }
//    }
//}


void NetOnkyoDialog::NetData(QString data)
{
    if (m_Comm.IsPioneer())
    {
        return;
    }

    // NMSMxxxxx20e net/usb menu status
    // "maabbstii"
    // NET/USB Menu Status (9 letters)
    // m -> Track Menu: "M": Menu is enable, "x": Menu is disable
    // aa /-> F1 button icon (Positive Feed or Mark/Unmark)
    // bb -> F2 button icon (Negative Feed)
    // aa or bb : "xx":disable, "01":Like, "02":don't like, "03":Love, "04":Ban,
    //                          "05":episode, "06":ratings, "07":Ban(black), "08":Ban(white),
    //                          "09":Favorite(black), "0A":Favorite(white), "0B":Favorite(yellow)
    // s -> Time Seek "S": Time Seek is enable "x": Time Seek is disable
    // t -> Time Display "1": Elapsed Time/Total Time, "2": Elapsed Time, "x": disable
    // ii-> Service icon
    // ii : "00":Music Server (DLNA), "01":My Favorite, "02":vTuner,
    //      "03":SiriusXM, "04":Pandora,
    //      "05":Rhapsody, "06":Last.fm, "08":Slacker, "0A":Spotify, "0B":AUPEO!,
    //      "0C":radiko, "0D":e-onkyo, "0E":TuneIn, "0F":MP3tunes, "10":Simfy,
    //      "11":Home Media, "12":Deezer, "13":iHeartRadio, "18":Airplay,
    //      "F0": USB/USB(Front), "F1: USB(Rear), "F2":Internet Radio
    //      "F3":NET, "F4":Bluetooth

    // NJA ...

    // IFANETWORK,,,,Extended Stereo,5.1.2 ch,,Off,---,Normal,No,
    // NTITechnoBase.FM - TechnoBase.FM - 24h Techno, Dance, HandsUp and M
    // NSTPxx



    // NLU00000001 NET/USB List Info (Update item, need processing XML data, for Network Control Only)
    // "xxxxyyyy"
    // xxxx -> index of update item (0000-FFFF : 1st to 65536th Item [4 HEX digits] )
    // yyyy : number of items (0000-FFFF : 1 to 65536 Items [4 HEX digits] )

    // NET/USB List Info
//    if (data.startsWith("NLS") && data.length() > 5)
//    {
//        QChar type = data[3];
//        if (type == 'C') // NLSC0P - Cursor Info
//        {
////            m_SelectedItemIndex = data[4].unicode() - '0';
////            QChar updateType = data[5]; // P = Page, C = Cursor position
////            if (updateType == 'P')
////            {
////                ui->listWidget->clear();
////            }
//////            else
//////            {
//////            }
////            if (m_SelectedItemIndex < ui->listWidget->count())
////            {
////                QColor color(255,255,255);
////                QColor selColor(200,200,255);
////                for (int i = 0; i < ui->listWidget->count(); i++)
////                {
////                    if (m_SelectedItemIndex == i)
////                    {
////                        ui->listWidget->item(i)->setBackgroundColor(selColor);
////                    }
////                    else
////                    {
////                        ui->listWidget->item(i)->setBackgroundColor(color);
////                    }
////                }
////            }
//            qDebug() << "Cursor info" << m_SelectedItemIndex << " of " << m_TotalNumberOfItems;
//        }
//        else if (type == 'A') // ascii
//        {
//            int pos = data[4].unicode() - '0';
//            QChar property = data[5];
//            // - : no
//            // 0 : Playing, A : Artist, B : Album, F : Folder, M : Music, P : Playlist, S : Search
//            // a : Account, b : Playlist-C, c : Starred, d : Unstarred, e : What's New
//            QString name = data.mid(6);
//            //qDebug() << "NLSA" << name << property;
//        }
//        else if (type == 'U') // unicode
//        {
//            int pos = data[4].unicode() - '0';
//            //int pos = data[4].unicode() - '0';
//            QChar property = data[5];
//            // - : no
//            // 0 : Playing, A : Artist, B : Album, F : Folder, M : Music, P : Playlist, S : Search
//            // a : Account, b : Playlist-C, c : Starred, d : Unstarred, e : What's New
//            QString name = data.mid(6);
////            //ui->listWidget->addItem(name);
////            ui->listWidget->insertItem(pos, name);
////            if (m_SelectedItemIndex == pos)
////            {
////                QColor selColor(200,200,255);
////                ui->listWidget->item(m_SelectedItemIndex)->setBackgroundColor(selColor);
////            }
//            //qDebug() << "NLSU" << pos << name << property;
//        }
//    }
//    else
    if (data.startsWith("NTM"))
    {
        handleTime(data.mid(3));
    }
    else if (data.startsWith("NLT") && data.length() > 24) // NET/USB List Title Info
    {
        // "xxuycccciiiillsraabbssnnn...nnn"
        // 3 xx     service type
        // 5 u      UI type
        // 6 y      layer info
        // 7 cccc   current cursor position
        // 11 iiii  number of list items
        // 15 ll    number of layer
        // 17 s     start flag
        // 18 r     reserved
        // 19 aa    icon on the left
        // 21 bb    icon on the right
        // 23 ss    status info
        // 25 nn    title bar text

        QString serviceType = data.mid(3, 2);
        // 00 : Music Server (DLNA), 01 : Favorite, 02 : vTuner, 03 : SiriusXM, 04 : Pandora, 05 : Rhapsody, 06 : Last.fm,
        // 07 : Napster, 08 : Slacker, 09 : Mediafly, 0A : Spotify, 0B : AUPEO!, 0C : radiko, 0D : e-onkyo,
        // 0E : TuneIn Radio, 0F : MP3tunes, 10 : Simfy, 11:Home Media, 12:Deezer, 13:iHeartRadio, 18:Airplay, 19:TIDAL, 1A:onkyo music,
        // F0 : USB/USB(Front) F1 : USB(Rear), F2 : Internet Radio, F3 : NET, FF : None

        int uiType = data[5].unicode() - '0';
        //0 : List, 1 : Menu, 2 : Playback, 3 : Popup, 4 : Keyboard, "5" : Menu List

        QChar layerInfo = data[6];
        // 0 : NET TOP, 1 : Service Top,DLNA/USB/iPod Top, 2 : under 2nd Layer

        int currentCursorPosition = data.mid(7, 4).toInt(0, 16);
        int numberOfListItems = data.mid(11, 4).toInt(0, 16);
        int numberOfLayer = data.mid(15, 2).toInt(0, 16);
        QChar startFlag = data[17]; // 0 : Not First, 1 : First

        // data[18] = reserved

        int iconLeft = data.mid(19, 2).toInt(0, 16); // Icon on Left of Title Bar
        // 00 : Internet Radio, 01 : Server, 02 : USB, 03 : iPod, 04 : DLNA, 05 : WiFi, 06 : Favorite
        // 10 : Account(Spotify), 11 : Album(Spotify), 12 : Playlist(Spotify), 13 : Playlist-C(Spotify)
        // 14 : Starred(Spotify), 15 : What's New(Spotify), 16 : Track(Spotify), 17 : Artist(Spotify)
        // 18 : Play(Spotify), 19 : Search(Spotify), 1A : Folder(Spotify)
        // FF : None

        int iconRight = data.mid(21, 2).toInt(0, 16); // Icon on Right of Title Bar
        // 00 : Music Server (DLNA), 01 : Favorite, 02 : vTuner, 03 : SiriusXM, 04 : Pandora, 05 : Rhapsody, 06 : Last.fm,
        // 07 : Napster, 08 : Slacker, 09 : Mediafly, 0A : Spotify, 0B : AUPEO!, 0C : radiko, 0D : e-onkyo,
        // 0E : TuneIn Radio, 0F : MP3tunes, 10 : Simfy, 11:Home Media, 12:Deezer, 13:iHeartRadio, 18:Airplay, 19:TIDAL, 1A:onkyo music,
        // F0:USB/USB(Front), F1:USB(Rear),
        // FF : None

        int statusInfo = data.mid(23, 2).toInt(0, 16); // Icon on Right of Title Bar
        // 00 : None, 01 : Connecting, 02 : Acquiring License, 03 : Buffering
        // 04 : Cannot Play, 05 : Searching, 06 : Profile update, 07 : Operation disabled
        // 08 : Server Start-up, 09 : Song rated as Favorite, 0A : Song banned from station,
        // 0B : Authentication Failed, 0C : Spotify Paused(max 1 device), 0D : Track Not Available, 0E : Cannot Skip

        QString titleBar = data.mid(25);
        ui->UnderListEdit->setText(titleBar);

        if (uiType == 0) // list
        {
            if (m_ScreenType != 0)
            {
                m_ScreenType = uiType;
                ui->DisplayWidget->setCurrentIndex(0);
                //ui->listWidget->clear();
                m_TotalNumberOfItems = numberOfListItems;
                //requestListData(0);
            }
        }
        else if (uiType == 2) // playback
        {
            if (m_ScreenType != 2)
            {
                m_ScreenType = uiType;
                ui->DisplayWidget->setCurrentIndex(1);
                SendCmd("NATQSTN");
                SendCmd("NALQSTN");
                SendCmd("NTIQSTN");
            }
        }

        m_SelectedItemIndex = currentCursorPosition;
        if (m_LayerNumber != numberOfLayer)
        {
            ui->listWidget->clear();
            m_LayerNumber = numberOfLayer;
            if (m_LayerNumber != 0)
            {
                requestListData(0);
            }
            else
            {
                // the first layer returns no xml data, so fill the list manually
                foreach (DirectInput di, m_DirectInputs) {
                    QListWidgetItem* listItem = new QListWidgetItem(ui->listWidget);
                    listItem->setText(di.title);
                    listItem->setIcon(m_Icons[1]);
                    listItem->setData(ROLE_LINENUMBER, di.number);
                }
            }
        }

        qDebug() << "type=" << serviceType << "ui=" << uiType << "pos=" << currentCursorPosition
                 << "count=" << numberOfListItems << "layer=" << numberOfLayer << "startFlag=" << startFlag << titleBar;
    }
    else if (data.startsWith("NAT")) // Artist
    {
        ui->ArtistLabel->setText(data.mid(3));
    }
    else if (data.startsWith("NAL")) // Album
    {
        ui->AlbumLabel->setText(data.mid(3));
        //ui->albumLabel->setText(tr("Album"));
    }
    else if (data.startsWith("NTI")) // Title
    {
        ui->TitleLabel->setText(data.mid(3));
        //ui->titleLabel->setText(tr("Title"));
    }
    else if (data.startsWith("NTR")) // Track info
    {
    }
    else if (data.startsWith("NST")) // Play status
    {
    }
    else if (data.startsWith("NLA")) // Net/Usb list info
    {
        // "tzzzzsurr<.....>"
        QChar responseType = data[3];
        // X -> XML
        // zzzz -> sequence number
        QChar status = data[8];
        // s -> status 'S' : success, 'E' : error
        int uiType = data[9].unicode() - '0';
        // u -> UI type '0' : List, '1' : Menu, '2' : Playback, '3' : Popup, '4' : Keyboard, "5" : Menu List
        // rr -> reserved
        QString xml = data.mid(12);
        // <.....> : XML data ( [CR] and [LF] are removed )
        qDebug() << "NLA: status = " << status << " uiType = " << uiType << " XML = " << xml;
        if (status == 'S')
        {
            QDomDocument doc;
            QString err;
            bool ok = doc.setContent(xml, false, &err);
            QDomNodeList roots = doc.elementsByTagName("item");
            for (int i = 0; i < roots.size(); i++) {
                QDomNode root = roots.item(i);
                QDomNamedNodeMap attributes = root.attributes();
                // title
                QDomNode tmp = attributes.namedItem("title");
                if (tmp.isNull())
                    continue;
                QString title = tmp.nodeValue().trimmed();
                tmp = attributes.namedItem("selectable");
                QString selectable = tmp.nodeValue().trimmed();
                tmp = attributes.namedItem("iconid");
                bool ok = false;
                int iconId = tmp.nodeValue().trimmed().toInt(&ok, 16);
                //icontype
                //iconid
                //selectable
                // '29' : Folder, '2A' : Folder X, '2B' : Server, '2C' : Server X, '2D' : Title, '2E' : Title X,
                // '2F' : Program, '31' : USB, '36' : Play, '37' : MultiAccount, for Spotify
                // '38' : Account, '39' : Album, '3A' : Playlist, '3B' : Playlist-C, '3C' : starred,
                // '3D' : What'sNew, '3E' : Artist, '3F' : Track, '40' : unstarred, '41' : Play, '43' : Search, '44' : Folder
                // for AUPEO!
                // '42' : Program
                if (selectable == "1")
                {
                    ui->listWidget->addItem(title);
                    QListWidgetItem* listItem = ui->listWidget->item(ui->listWidget->count() - 1);
                    listItem->setData(ROLE_LINENUMBER, i); // set line number for selection

                    switch(iconId)
                    {
                    case 0x29: // folder
                    case 0x2a: // folder x
                    case 0x44: // folder for AUPEO
                        listItem->setIcon(m_Icons[1]);
                        break;
                    case 0x2d: // title
                    case 0x2e: // title x
                    case 0x39: // album
                    case 0x3e: // artist
                    case 0x3f: // track
                        listItem->setIcon(m_Icons[2]);
                        break;
                    case 0x36: // play
                    case 0x41: // play
                        listItem->setIcon(m_Icons[4]);
                        break;
                    case 0x2b: // server
                    case 0x2c: // server x
                    case 0x2f: // program
                    case 0x31: // usb
                    case 0x37: // multiaccount for spotify
                    case 0x38: // account
                    case 0x3a: // playlist
                    case 0x3b: // playlist-c
                    case 0x3c: // starred
                    case 0x3d: // what's new
                    case 0x40: // unstarred
                    case 0x42: // program
                    case 0x43: // search
                    default:
                        break;
                    }
                }
                qDebug() << "Title: " << title << iconId;

            }
        }

    }


     //qDebug() << " >>> " << data;
    if (data.startsWith("GBH") || data.startsWith("GBP"))
    {
        m_VisibleListSize = data.mid(3, 2).toInt();
         //qDebug() << "visible list size " << m_VisibleListSize;
    }
    else if (data.startsWith("GCH") || data.startsWith("GCP"))
    {
        int ScreenType = data.mid(3, 2).toInt();
        if (ScreenType >= 2 && ScreenType <= 5)
        {
            // play window
            ui->DisplayWidget->setCurrentIndex(1);
            //m_PlayTimeTimer.start();
        }
        else
        {
            //m_PlayTimeTimer.stop();
            ui->TimeLabel->setText("--:--");
            ui->DisplayWidget->setCurrentIndex(0);
            ui->listWidget->clear();
        }
        m_ScreenType = ScreenType;
        m_ListUpdateFlag = data.mid(5, 1).toInt();
//        int topMenuKey = data.mid(6, 1).toInt();
//        int toolsKey = data.mid(7, 1).toInt();
//        int returnKey = data.mid(8, 1).toInt();
//        QString screenName = data.mid(10);
//        qDebug() << "ScreenType " << screenType << " ListUpdateFlag " << listUpdateFlag
//                 << " TopMenuKey " << topMenuKey << " ToolsKey " << toolsKey
//                 << " ReturnKey " << returnKey << " ScreenName <" << screenName << ">";
    }
    else if (data.startsWith("GDH") || data.startsWith("GDP"))
    {
        m_IndexOfLine1 = data.mid(3, 5).toInt();
        m_IndexOfLastLine = data.mid(8, 5).toInt();
        m_TotalNumberOfItems = data.mid(13, 5).toInt();
        //qDebug() << "IndexOfLine1 " << m_IndexOfLine1
        //         << " IndexOfLastLine " << m_IndexOfLastLine
        //         << " TotalNumberOfItems " << m_TotalNumberOfItems;
    }
    else if (data.startsWith("GEH") || data.startsWith("GEP"))
    {
        int LineNumber = data.mid(3, 2).toInt();
        int FocusInformation = data.mid(5, 1).toInt();
        int LineDataType = data.mid(6, 2).toInt();
        QString DisplayLine = data.mid(8);
        DisplayLine = DisplayLine.trimmed();
        while (DisplayLine.startsWith("\""))
            DisplayLine.remove(0, 1);
        while (DisplayLine.endsWith("\""))
            DisplayLine.chop(1);

//        m_StationList.append(DisplayLine);
//        if ((m_StationList.count() == m_IndexOfLastLine) && m_StationList.count() != m_TotalNumberOfItems)
//        {
//            QString cmd = QString("%1GGH").arg(m_IndexOfLastLine + 1, 5, 10, QChar('0'));
//            emit SendCmd(cmd);
//        }

//        QByteArray locallyEncoded = DisplayLine.toAscii(); // text to convert
//        QTextCodec *codec = QTextCodec::codecForName("KOI8-U"); // get the codec for KOI8-R
//        DisplayLine = codec->toUnicode( locallyEncoded );

        /*

Big5 -- Chinese
eucJP -- Japanese
eucKR -- Korean
GBK -- Chinese
JIS7 -- Japanese
Shift-JIS -- Japanese
TSCII -- Tamil
utf8 -- Unicode, 8-bit
utf16 -- Unicode
KOI8-R -- Russian
KOI8-U -- Ukrainian
ISO8859-1 -- Western
ISO8859-2 -- Central European
ISO8859-3 -- Central European
ISO8859-4 -- Baltic
ISO8859-5 -- Cyrillic
ISO8859-6 -- Arabic
ISO8859-7 -- Greek
ISO8859-8 -- Hebrew, visually ordered
ISO8859-8-i -- Hebrew, logically ordered
ISO8859-9 -- Turkish
ISO8859-10
ISO8859-13
ISO8859-14
ISO8859-15 -- Western
CP850
CP874
CP1250 -- Central European
CP1251 -- Cyrillic
CP1252 -- Western
CP1253 -- Greek
CP1254 -- Turkish
CP1255 -- Hebrew
CP1256 -- Arabic
CP1257 -- Baltic
CP1258
Apple Roman
TIS-620 -- Thai*/
        int index = LineNumber - 1;//ui->listWidget->count() - 1;
        if (m_ScreenType == 1)
        {
            ui->listWidget->addItem(DisplayLine);
            ui->listWidget->item(index)->setText(DisplayLine);
            ui->listWidget->item(index)->setIcon(m_Icons[LineDataType]);
            if (FocusInformation)
            {
                QBrush brush(QColor(0, 0, 255));
                //int index = ui->listWidgetLog->currentIndex().row();
                ui->listWidget->item(index)->setForeground(brush);
                m_SelectedItemIndex = index;
                QString text = QString("%1/%2").arg(m_IndexOfLine1 + m_SelectedItemIndex).arg(m_TotalNumberOfItems);
                ui->UnderListEdit->setText(text);
            }
        }
        else if (m_ScreenType >= 2 && m_ScreenType <= 5)
        {
            //ui->listWidget->addItem(DisplayLine);
            //ui->listWidget->item(index)->setText(DisplayLine);
            switch(LineDataType)
            {
            case 20: // Track
                ui->TitleLabel->setText(DisplayLine);
                ui->titleLabel->setText(tr("Title"));
                break;
            case 21: // Artist
                ui->ArtistLabel->setText(DisplayLine);
                break;
            case 22: // Album
                ui->AlbumLabel->setText(DisplayLine);
                ui->albumLabel->setText(tr("Album"));
                break;
            case 23: // Time
                handleTime(DisplayLine);
            case 24: // Genre
                ui->GenreLabel->setText(DisplayLine);
                break;
            case 26: // Format
                m_PlayFormat = DisplayLine;
                showAudioInfoLine();
                break;
            case 27: // Bitrate
                if (m_NewDataFormat) {
                    m_BitsPerSample = DisplayLine;
                } else {
                    int bitrate = DisplayLine.toInt() / 1000;
                    m_PlayBitrate = QString("%1kBit/s").arg(bitrate);
                }
                showAudioInfoLine();
                break;
            case 29: // new format Bitrate
                if (m_NewDataFormat) {
                    m_PlayBitrate = DisplayLine;
                    showAudioInfoLine();
                }
                break;
            case 32: // Channel
                ui->TitleLabel->setText(DisplayLine);
                ui->titleLabel->setText(tr("Channel"));
                break;
            case 33: // Station
                ui->AlbumLabel->setText(DisplayLine);
                ui->albumLabel->setText(tr("Station"));
                break;
            case 34: // total time
                m_NewDataFormat = true;
                //handleTime(DisplayLine);
                m_TotalTime = DisplayLine;
                break;
            }
        }
//        qDebug() << "LineNumber " << LineNumber << " FocusInformation " << FocusInformation
//                 << " LineDataType " << LineDataType << " DisplayLine <" << DisplayLine << ">";
    }
    else if (data.startsWith("GHH") || data.startsWith("GHP"))
    {
        if (m_ScreenType != 1)
        {
            ui->UnderListEdit->setText("");
        }
        //int SourceInformation = data.mid(3, 2).toInt();
        //qDebug() << "SourceInformation " << SourceInformation;
    }
}

/*
 * ISCP!1NLAX0003S000<?xml version="1.0" encoding="utf-8"?><response status="ok"><items offset="0" totalitems="12" ><item icontype="M" iconid="2d" title="105.7 |  FM  (90&apos;s Music)" selectable="1" /><item icontype="M" iconid="2d" title=" Premium (Electronic Music)" selectable="1" /><item icontype="M" iconid="2d" title="Hardcorepower Radio (Electronic Music)" selectable="1" /><item icontype="M" iconid="2d" title="Hard FM (Electronic Music)" selectable="1" /><item icontype="M" iconid="2d" title="Goanight (Electronic Music)" selectable="1" /><item icontype="M" iconid="2d" title="ENERGY Dance (Electronic Music)" selectable="1" /><item icontype="M" iconid="2d" title="bigFM Russia (Russian Music)" selectable="1" /><item icontype="M" iconid="2d" title="Radio Katek (Russian Music)" selectable="1" /><item icontype="M" iconid="2d" title="RauteMusik.FM Happy Hardcore (Techno)" selectable="1" /><item icontype="M" iconid="2d" title="HardBase.FM (Techno)" selectable="1" /><item icontype="M" iconid="2d" title="TechnoBase.FM (Techno)" selectable="1" /><item icontype="M" iconid="2d" title="Tvo  Radio" selectable="1" /></items></response>

ISCP!1NLAI010000----

 */
void NetOnkyoDialog::requestListData(int pos)
{
    //ISCP!1NLA L 0003 02 0000 000C
    int sequenceNumber = 1;
    QString cmd = QString::asprintf("NLAL%04X%02X%04X%04X", sequenceNumber, m_LayerNumber, pos, m_TotalNumberOfItems);
    qDebug() << "send " << cmd;
    SendCmd(cmd);
}

void NetOnkyoDialog::showAudioInfoLine()
{
    if (m_BitsPerSample == "")
        ui->FormatLabel->setText(m_PlayFormat + " " + m_PlayBitrate);
    else
        ui->FormatLabel->setText(m_PlayFormat + " " + m_BitsPerSample + "/" + m_PlayBitrate);
}

void NetOnkyoDialog::handleTime(QString str)
{
    //"00:25:55/--:--:--"
    QString time;
    QStringList times = str.split("/", QString::SkipEmptyParts);
    if (times.count() > 0) // current time
    {
        time =times[0];
    }
    if (times.count() > 1) // current time
    {
        if (times[1] != "--:--:--")
        {
            time += " / " + times[1];
        }
    }
    ui->TimeLabel->setText(time);
}


void NetOnkyoDialog::on_CursorUpButton_clicked()
{
    emit SendCmd("NTCUP");
}

void NetOnkyoDialog::on_CursorLeftButton_clicked()
{
    emit SendCmd("NTCLEFT");
}

void NetOnkyoDialog::on_CursorEnterButton_clicked()
{
    emit SendCmd("NTCSELECT");
}

void NetOnkyoDialog::on_CursorRightButton_clicked()
{
    emit SendCmd("NTCRIGHT");
}

void NetOnkyoDialog::on_CursorDownButton_clicked()
{
    emit SendCmd("NTCDOWN");
}

void NetOnkyoDialog::on_CursorReturnButton_clicked()
{
    emit SendCmd("NTCRETURN");
}

void NetOnkyoDialog::on_NetPauseButton_clicked()
{
    emit SendCmd("NTCPAUSE");
}

void NetOnkyoDialog::on_NetPlayButton_clicked()
{
    emit SendCmd("NTCPLAY");
}

void NetOnkyoDialog::on_NetStopButton_clicked()
{
    emit SendCmd("NTCSTOP");
}

void NetOnkyoDialog::on_NetPrevButton_clicked()
{
    emit SendCmd("NTCTRDN");
}

void NetOnkyoDialog::on_NetNextButton_clicked()
{
    emit SendCmd("NTCTRUP");
}

void NetOnkyoDialog::on_NetRevButton_clicked()
{
    emit SendCmd("NTCTREW");
}

void NetOnkyoDialog::on_NetFwdButton_clicked()
{
    emit SendCmd("NTCTFF");
}

void NetOnkyoDialog::on_listWidget_currentRowChanged(int /*currentRow*/)
{
//    qDebug() << "Current row " << currentRow;
//    QString cmd = QString("%1GFH").arg((uint)(currentRow + 1), 2, 10, QChar('0'));
//    emit SendCmd(cmd);
}

void NetOnkyoDialog::on_listWidget_itemDoubleClicked(QListWidgetItem * /*item*/)
{
    QListWidgetItem* listItem = ui->listWidget->currentItem();
    int number = listItem->data(ROLE_LINENUMBER).toInt();
    qDebug() << "selected" << listItem->data(ROLE_LINENUMBER);
    QString cmd;
    if (m_LayerNumber == 0)
    {
        cmd = QString::asprintf("NSV%02X%01d", number, 0);
    }
    else
    {
        //NLAI000101000A----
        //"Izzzzllxxxx----"
        //zzzz -> sequence number (0000-FFFF)
        //ll -> number of layer (00-FF)
        //xxxx -> index number (0000-FFFF : 1st to 65536th Item [4 HEX digits] )
        //---- -> not used
        cmd = QString::asprintf("NLAI%02X%04X----", m_LayerNumber, number);
    }
    emit SendCmd(cmd);
    //int currentRow = ui->listWidget->currentRow();
    //qDebug() << "Current row " << currentRow;
//    QString cmd = QString("%1GFH").arg((uint)(currentRow + 1), 2, 10, QChar('0'));
//    emit SendCmd(cmd);
}

void NetOnkyoDialog::on_NetAddFavButton_clicked()
{
    //emit SendCmd("32NW"); // program / add fav
//    QListWidgetItem listItem = ui->listWidget->currentItem();
//    if (listItem != NULL)
//    {
//        int number = listItem.data(ROLE_LINENUMBER);
//        cmd = QString::asprintf("NRF%02X", number);
//        emit SendCmd(0);
//    }
}

void NetOnkyoDialog::on_NetRemoveFavButton_clicked()
{
    //emit SendCmd("33NW"); // clear / remove fav
    QListWidgetItem* listItem = ui->listWidget->currentItem();
    if (listItem != NULL)
    {
        int number = listItem->data(ROLE_LINENUMBER).toInt();
        QString cmd = QString::asprintf("NAF%04X", number);
        emit SendCmd(0);
    }
}

void NetOnkyoDialog::on_NetSwitchToNetRadioButton_clicked()
{
    ui->NetSwitchToNetRadioButton->setChecked(!ui->NetSwitchToNetRadioButton->isChecked());
    QString cmd = QString::asprintf("NSV%02X%01d", 0x0e, 0);
    emit SendCmd(0);
}

void NetOnkyoDialog::on_NetSwitchToMediaServerButton_clicked()
{
    ui->NetSwitchToMediaServerButton->setChecked(!ui->NetSwitchToMediaServerButton->isChecked());
    QString cmd = QString::asprintf("NSV%02X%01d", 0x0e, 0);
    emit SendCmd(cmd);
}

void NetOnkyoDialog::on_NetSwitchToFavoritesButton_clicked()
{
    ui->NetSwitchToFavoritesButton->setChecked(!ui->NetSwitchToFavoritesButton->isChecked());
    QString cmd = QString::asprintf("NLAI%02X%04X----", 1/*m_LayerNumber*/, 0/*number*/);
    emit SendCmd(cmd);
}

void NetOnkyoDialog::on_pushButton_clicked()
{
    QString newText = ui->TitleLabel->text() + ";" + ui->ArtistLabel->text() + ";" + ui->AlbumLabel->text();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(newText);
}

void NetOnkyoDialog::on_RepeatButton_clicked()
{
    emit SendCmd("NTCREPEAT"); // repeat
}

void NetOnkyoDialog::on_RandomButton_clicked()
{
    emit SendCmd("NTCRANDOM"); // random
}

