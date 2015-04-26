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
#include "netradiodialog.h"
#include "ui_netradiodialog.h"
#include <QDebug>
#include <qtextcodec.h>
#include <QDateTime>
#include <QClipboard>

NetRadioDialog::NetRadioDialog(QWidget *parent, QSettings &settings, ReceiverInterface &Comm) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::NetRadioDialog),
    m_Comm(Comm),
    m_PositionSet(false),
    m_PlayTimeTimer(this)
{
    m_IndexOfLine1 = 0;
    m_IndexOfLastLine = 0;
    m_TotalNumberOfItems = 0;
    m_SelectedItemIndex = 0;
    m_VisibleListSize = 0;
    m_ScreenType = 0;
    m_ListUpdateFlag = 0;
    m_PlayTime = 0;
    m_PlayFormat = "-";
    m_PlayBitrate = "-";

    ui->setupUi(this);

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
    connect((&m_PlayTimeTimer), SIGNAL(timeout()), this, SLOT(RefreshPlayTime()));

    // directory
    QIcon icon1(QString::fromUtf8(":/new/prefix1/images/Gnome-folder-open.png"));
    m_Icons.insert(1, icon1);
    // audio
    QIcon icon2(QString::fromUtf8(":/new/prefix1/images/Gnome-audio-x-generic.png"));
    m_Icons.insert(2, icon2);
    // photo
    QIcon icon3(QString::fromUtf8(":/new/prefix1/images/Gnome-emblem-photos.png"));
    m_Icons.insert(3, icon3);
    // video
    QIcon icon4(QString::fromUtf8(":/new/prefix1/images/Gnome-media-playback-start.png"));
    m_Icons.insert(4, icon4);
    // now playing
    QIcon icon5(QString::fromUtf8(":/new/prefix1/images/Gnome-video-x-generic.png"));
    m_Icons.insert(5, icon5);

    m_Timer.setSingleShot(false);
    m_Timer.setInterval(10000);
    //Timeout();

    m_PlayTimeTimer.setInterval(200);
}


NetRadioDialog::~NetRadioDialog()
{
    delete ui;
}


void NetRadioDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("NetRadioWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}

void NetRadioDialog::ManualShowNetDialog()
{
    ShowNetDialog(false);
}

void NetRadioDialog::ShowNetDialog(bool autoShow)
{   
    if ((!autoShow) || (m_Settings.value("AutoShowNetRadio", true).toBool() && !isVisible()))
    {
        emit SendCmd("?GAH");
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
        show();
     }
}

void NetRadioDialog::ResponseReceived(ReceivedObjectBase *response)
{
    InputFunctionResponse_FN* inputFunction = dynamic_cast<InputFunctionResponse_FN*>(response);
    if (inputFunction != NULL)
    {
        int no = inputFunction->getNumber();
        if (no == 26 || // NET cyclic
            no == 38 || // internet radio
            no == 40 || // sirius xm
            no == 41 || // pandora
            no == 44 || // media server
            no == 45    // favorites
                )
        {
            if (no == 45)
            {
                ui->NetAddFavButton->setEnabled(false);
                ui->NetRemoveFavButton->setEnabled(true);
                ui->NetSwitchToFavoritesButton->setChecked(true);
                ui->NetSwitchToMediaServerButton->setChecked(false);
                ui->NetSwitchToNetRadioButton->setChecked(false);
            }
            else
            {
                ui->NetAddFavButton->setEnabled(true);
                ui->NetRemoveFavButton->setEnabled(false);
                if (no == 38)
                {
                    ui->NetSwitchToFavoritesButton->setChecked(false);
                    ui->NetSwitchToMediaServerButton->setChecked(false);
                    ui->NetSwitchToNetRadioButton->setChecked(true);
                }
                else if (no == 44)
                {
                    ui->NetSwitchToFavoritesButton->setChecked(false);
                    ui->NetSwitchToMediaServerButton->setChecked(true);
                    ui->NetSwitchToNetRadioButton->setChecked(false);
                }
                else
                {
                    ui->NetSwitchToFavoritesButton->setChecked(false);
                    ui->NetSwitchToMediaServerButton->setChecked(false);
                    ui->NetSwitchToNetRadioButton->setChecked(false);
                }
            }
            this->setWindowTitle(inputFunction->getName());
        }
        else
        {
            this->setWindowTitle(tr("NetRadio"));
        }
    }
}


void NetRadioDialog::Timeout()
{
    if(isVisible())
        emit SendCmd("?GAH");
}


void NetRadioDialog::RefreshPlayTime()
{
    if (m_ScreenType >= 2 && m_ScreenType <= 5)
    {
        uint64_t tmp = QDateTime::currentMSecsSinceEpoch() / 1000ULL - m_PlayTime;
        uint64_t hour = tmp / 3600ULL;
        tmp = tmp - hour * 3600;
        uint64_t min = tmp / 60;
        uint64_t sec = tmp % 60;
        if (hour > 0)
        {
            ui->TimeLabel->setText(QString("%1:%2:%3").arg(hour, 2, 10, QChar('0')).arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0')));
        }
        else
        {
            ui->TimeLabel->setText(QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0')));
        }
    }
    else
    {
        m_PlayTimeTimer.stop();
    }
}


void NetRadioDialog::NetData(QString data)
{
     //qDebug() << " >>> " << data;
    if (data.startsWith("GBH"))
    {
        m_VisibleListSize = data.mid(3, 2).toInt();
         qDebug() << "visible list size " << m_VisibleListSize;
    }
    else if (data.startsWith("GCH"))
    {
        int ScreenType = data.mid(3, 2).toInt();
        if (ScreenType >= 2 && ScreenType <= 5)
        {
            // play window
            ui->DisplayWidget->setCurrentIndex(1);
            m_PlayTimeTimer.start();
        }
        else
        {
            m_PlayTimeTimer.stop();
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
    else if (data.startsWith("GDH"))
    {
        m_IndexOfLine1 = data.mid(3, 5).toInt();
        m_IndexOfLastLine = data.mid(8, 5).toInt();
        m_TotalNumberOfItems = data.mid(13, 5).toInt();
        qDebug() << "IndexOfLine1 " << m_IndexOfLine1
                 << " IndexOfLastLine " << m_IndexOfLastLine
                 << " TotalNumberOfItems " << m_TotalNumberOfItems;
    }
    else if (data.startsWith("GEH"))
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
                break;
            case 21: // Artist
                ui->ArtistLabel->setText(DisplayLine);
                break;
            case 22: // Album
                ui->AlbumLabel->setText(DisplayLine);
                break;
            case 23: // Time
            {
                uint64_t hour = 0;
                uint64_t min = 0;
                uint64_t sec = 0;
                QStringList list = DisplayLine.split(":", QString::SkipEmptyParts);
                if (list.count() > 2)
                {
                    hour = list[0].toInt();
                    min = list[1].toInt();
                    sec = list[2].toInt();
                }
                else
                if (list.count() > 1)
                {
                    min = list[0].toInt();
                    sec = list[1].toInt();
                }
                m_PlayTime = QDateTime::currentMSecsSinceEpoch() / 1000ULL - (hour * 3600ULL + min * 60ULL + sec);
                //ui->TimeLabel->setText(DisplayLine);
                m_PlayTimeTimer.start(); // resync
                break;
            }
            case 24: // Genre
                ui->GenreLabel->setText(DisplayLine);
                break;
            case 26: // Format
                m_PlayFormat = DisplayLine;
                ui->FormatLabel->setText(m_PlayFormat + " " + m_PlayBitrate);
                break;
            case 27: // Bitrate
            {
                int bitrate = DisplayLine.toInt() / 1000;
                m_PlayBitrate = QString("%1kBit/s").arg(bitrate);
                ui->FormatLabel->setText(m_PlayFormat + " " + m_PlayBitrate);
                break;
            }
            }
        }
//        qDebug() << "LineNumber " << LineNumber << " FocusInformation " << FocusInformation
//                 << " LineDataType " << LineDataType << " DisplayLine <" << DisplayLine << ">";
    }
    else if (data.startsWith("GHH"))
    {
        if (m_ScreenType != 1)
        {
            ui->UnderListEdit->setText("");
        }
        //int SourceInformation = data.mid(3, 2).toInt();
        //qDebug() << "SourceInformation " << SourceInformation;
    }
}

void NetRadioDialog::on_CursorUpButton_clicked()
{
    emit SendCmd("26NW");
}

void NetRadioDialog::on_CursorLeftButton_clicked()
{
    emit SendCmd("29NW");
}

void NetRadioDialog::on_CursorEnterButton_clicked()
{
    emit SendCmd("30NW");
}

void NetRadioDialog::on_CursorRightButton_clicked()
{
    emit SendCmd("28NW");
//    m_StationList.clear();
//    emit SendCmd("?ATB");
}

void NetRadioDialog::on_CursorDownButton_clicked()
{
    emit SendCmd("27NW");
}

void NetRadioDialog::on_CursorReturnButton_clicked()
{
    emit SendCmd("31NW");
}

void NetRadioDialog::on_NetPauseButton_clicked()
{
    emit SendCmd("11NW");
}

void NetRadioDialog::on_NetPlayButton_clicked()
{
    emit SendCmd("10NW");
}

void NetRadioDialog::on_NetStopButton_clicked()
{
    emit SendCmd("20NW");
}

void NetRadioDialog::on_NetPrevButton_clicked()
{
    emit SendCmd("12NW");
}

void NetRadioDialog::on_NetNextButton_clicked()
{
    emit SendCmd("13NW");
}

void NetRadioDialog::on_NetRevButton_clicked()
{
    emit SendCmd("14NW");
}

void NetRadioDialog::on_NetFwdButton_clicked()
{
    emit SendCmd("15NW");
}

void NetRadioDialog::on_listWidget_currentRowChanged(int /*currentRow*/)
{
//    qDebug() << "Current row " << currentRow;
//    QString cmd = QString("%1GFH").arg((uint)(currentRow + 1), 2, 10, QChar('0'));
//    emit SendCmd(cmd);
}

void NetRadioDialog::on_PageUpButton_clicked()
{
    int pos = m_IndexOfLine1;// + m_SelectedItemIndex;
    pos -= m_VisibleListSize;
    if (pos < 1)
        pos = (m_TotalNumberOfItems / 8) * 8;// + pos - 1;
    QString cmd = QString("%1GGH").arg(pos, 5, 10, QChar('0'));
    emit SendCmd(cmd);
}

void NetRadioDialog::on_PageDownButton_clicked()
{
    int pos = m_IndexOfLine1;// + m_SelectedItemIndex;
    pos += m_VisibleListSize;
    if (pos > m_TotalNumberOfItems)
        pos = 1;//pos % m_TotalNumberOfItems -1;
    QString cmd = QString("%1GGH").arg(pos, 5, 10, QChar('0'));
    emit SendCmd(cmd);
}

void NetRadioDialog::on_PageUpButton_2_clicked()
{
    int pos = m_IndexOfLine1;// + m_SelectedItemIndex;
    pos -= 108;
    if (pos < 1)
        pos = (m_TotalNumberOfItems / 8) * 8;// + pos - 1;
    QString cmd = QString("%1GGH").arg(pos, 5, 10, QChar('0'));
    emit SendCmd(cmd);
}

void NetRadioDialog::on_PageDownButton_2_clicked()
{
    int pos = m_IndexOfLine1;// + m_SelectedItemIndex;
    pos += 108;
    if (pos > m_TotalNumberOfItems)
        pos = 1;//pos % m_TotalNumberOfItems - 1;
    QString cmd = QString("%1GGH").arg(pos, 5, 10, QChar('0'));
    emit SendCmd(cmd);
}

void NetRadioDialog::on_listWidget_itemDoubleClicked(QListWidgetItem */*item*/)
{
//    qDebug() << "Current row " << currentRow;
    int currentRow = ui->listWidget->currentRow();
    QString cmd = QString("%1GFH").arg((uint)(currentRow + 1), 2, 10, QChar('0'));
    emit SendCmd(cmd);
}

void NetRadioDialog::on_NetAddFavButton_clicked()
{
    emit SendCmd("32NW"); // program / add fav
}

void NetRadioDialog::on_NetRemoveFavButton_clicked()
{
    emit SendCmd("33NW"); // clear / remove fav
}

void NetRadioDialog::on_NetSwitchToNetRadioButton_clicked()
{
    ui->NetSwitchToNetRadioButton->setChecked(!ui->NetSwitchToNetRadioButton->isChecked());
    SendCmd("38FN");
}

void NetRadioDialog::on_NetSwitchToMediaServerButton_clicked()
{
    ui->NetSwitchToMediaServerButton->setChecked(!ui->NetSwitchToMediaServerButton->isChecked());
    SendCmd("44FN");
}

void NetRadioDialog::on_NetSwitchToFavoritesButton_clicked()
{
    ui->NetSwitchToFavoritesButton->setChecked(!ui->NetSwitchToFavoritesButton->isChecked());
    SendCmd("45FN");
}

void NetRadioDialog::on_pushButton_clicked()
{
    QString newText = ui->TitleLabel->text() + ";" + ui->ArtistLabel->text() + ";" + ui->AlbumLabel->text();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(newText);
}

void NetRadioDialog::on_RepeatButton_clicked()
{
    emit SendCmd("34NW"); // repeat
}

void NetRadioDialog::on_RandomButton_clicked()
{
    emit SendCmd("35NW"); // random
}

