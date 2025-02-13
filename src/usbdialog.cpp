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
#include "usbdialog.h"
#include "ui_usbdialog.h"
#include <QDebug>
#include <QDateTime>


usbDialog::usbDialog(QWidget *parent, QSettings &settings, ReceiverInterface &Comm) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::usbDialog),
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
    m_PlayTime = 0;
    m_PlayFormat = "-";
    m_PlayBitrate = "-";


    ui->setupUi(this);

    // restore the position of the window
    if (m_Settings.value("SaveUsbWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("UsbWindowGeometry").toByteArray());
    }

    connect((&m_Comm), SIGNAL(usbData(QString)), this, SLOT(usbrecData(QString)));
    connect((this),    SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));

    connect((&m_Timer), SIGNAL(timeout()), this, SLOT(Timeout()));
    connect((&m_PlayTimeTimer), SIGNAL(timeout()), this, SLOT(RefreshPlayTime()));

    m_Timer.setSingleShot(false);
    m_Timer.setInterval(10000);

    m_PlayTimeTimer.setInterval(200);

    ui->brandom->setText(tr("Random off"));
    ui->brepeat->setText(tr("Repeat off"));
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

    MsgDistributor::AddResponseListener(this, QStringList() << DisplayDataResponse_FL().getResponseID());
}



usbDialog::~usbDialog()
{
    delete ui;
}


void usbDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("UsbWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}


void usbDialog::ManualShowusbDialog()
{
    ShowusbDialog(false);
}

void usbDialog::ShowusbDialog(bool autoShow)
{   
    if (!m_Comm.IsPioneer())
    {
        return;
    }
    if ((!autoShow) || (m_Settings.value("AutoShowUSB", true).toBool() && !isVisible()))
    {
        emit SendCmd("?GAI");
        if (!m_PositionSet || !m_Settings.value("SaveUsbWindowGeometry", false).toBool())
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


void usbDialog::ResponseReceived(ReceivedObjectBase *response)
{
    if (!m_Comm.IsPioneer())
    {
        return;
    }
    DisplayDataResponse_FL* display = dynamic_cast<DisplayDataResponse_FL*>(response);
    if (display != NULL)
    {
        QString str, str1;
        str = display->getDisplayLine().trimmed();
        str1 = str.mid(0,6);
        if(str1 == "Repeat")
            ui->brepeat->setText(str);
        else if(str1 == "Random")
            ui->brandom->setText(str);
        return;
    }
}



void usbDialog::Timeout()
{
    if(isVisible())
    {
        if (m_Comm.IsPioneer())
        {
            emit SendCmd("?GAI");
        }
    }
}


void usbDialog::RefreshPlayTime()
{
    if (m_Comm.IsPioneer() && m_ScreenType >= 2 && m_ScreenType <= 5)
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


void usbDialog::usbrecData(QString data)
{
    if (data.startsWith("GBI"))
    {
        m_VisibleListSize = data.mid(3, 2).toInt();
        // qDebug() << "maximum number of list " << n;
    }
    else if (data.startsWith("GCI"))
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

        //        int screenType = data.mid(3, 2).toInt();
//        int listUpdateFlag = data.mid(5, 1).toInt();
//        int topMenuKey = data.mid(6, 1).toInt();
//        int toolsKey = data.mid(7, 1).toInt();
//        int returnKey = data.mid(8, 1).toInt();
//        QString screenName = data.mid(10);
//        qDebug() << "ScreenType " << screenType << " ListUpdateFlag " << listUpdateFlag
//                 << " TopMenuKey " << topMenuKey << " ToolsKey " << toolsKey
//                 << " ReturnKey " << returnKey << " ScreenName <" << screenName << ">";
    }
    else if (data.startsWith("GDI"))
    {
        m_IndexOfLine1 = data.mid(3, 5).toInt();
        m_IndexOfLastLine = data.mid(8, 5).toInt();
        m_TotalNumberOfItems = data.mid(13, 5).toInt();
        QString text = QString("%1/%2").arg(m_IndexOfLine1 + m_SelectedItemIndex).arg(m_TotalNumberOfItems);
        ui->UnderListEdit->setText(text);
/*        qDebug() << "IndexOfLine1 " << m_IndexOfLine1
                 << " IndexOfLastLine " << m_IndexOfLastLine
                 << " TotalNumberOfItems " << m_TotalNumberOfItems;*/
    }
    else if (data.startsWith("GEI"))
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

        int index = LineNumber - 1;
        if (m_ScreenType == 1)
        {
            ui->listWidget->addItem(DisplayLine);
            ui->listWidget->item(index)->setText(DisplayLine);
            ui->listWidget->item(index)->setIcon(m_Icons[LineDataType]);
            if (FocusInformation)
            {
                QBrush brush(QColor(0, 0, 255));
                ui->listWidget->item(index)->setForeground(brush);
                m_SelectedItemIndex = index;
                //QString text = QString("%1/%2").arg(m_IndexOfLine1 + m_SelectedItemIndex).arg(m_TotalNumberOfItems);
                //ui->UnderListEdit->setText(text);
            }
        }
        else if (m_ScreenType >= 2 && m_ScreenType <= 5)
        {
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
                QStringList list = DisplayLine.split(":",
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
                                  QString::SkipEmptyParts);
#else
                                   Qt::SkipEmptyParts);
#endif

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

    }
/*    else if (data.startsWith("GHH"))
    {
        QString text = QString("%1/%2").arg(m_IndexOfLine1 + m_SelectedItemIndex).arg(m_TotalNumberOfItems);
        ui->UnderListEdit->setText(text);
        //int SourceInformation = data.mid(3, 2).toInt();
        //qDebug() << "SourceInformation " << SourceInformation;
    }*/
}

void usbDialog::on_CursorUpButton_clicked()
{
    emit SendCmd("13IP");
}

void usbDialog::on_CursorLeftButton_clicked()
{
    emit SendCmd("16IP");
}

void usbDialog::on_CursorEnterButton_clicked()
{
    emit SendCmd("17IP");
}

void usbDialog::on_CursorRightButton_clicked()
{
    emit SendCmd("15IP");
}

void usbDialog::on_CursorDownButton_clicked()
{
    emit SendCmd("14IP");
}

void usbDialog::on_CursorReturnButton_clicked()
{
    emit SendCmd("19IP");
}

void usbDialog::on_NetPauseButton_clicked()
{
    emit SendCmd("01IP");
}

void usbDialog::on_NetPlayButton_clicked()
{
    emit SendCmd("00IP");
}

void usbDialog::on_NetStopButton_clicked()
{
    emit SendCmd("02IP");
}

void usbDialog::on_NetPrevButton_clicked()
{
    emit SendCmd("03IP");
}

void usbDialog::on_NetNextButton_clicked()
{
    emit SendCmd("04IP");
}

void usbDialog::on_NetRevButton_clicked()
{
    emit SendCmd("05IP");
}

void usbDialog::on_NetFwdButton_clicked()
{
    emit SendCmd("06IP");
}

void usbDialog::on_listWidget_currentRowChanged(int/* currentRow*/)
{
//    qDebug() << "Current row " << currentRow;
//    QString cmd = QString("%1GFH").arg((uint)(currentRow + 1), 2, 10, QChar('0'));
//    emit SendCmd(cmd);
}

void usbDialog::on_PageUpButton_clicked()
{
    int pos = m_IndexOfLine1 + m_SelectedItemIndex;
    pos -= m_VisibleListSize;
    if (pos <= 1)
        pos = m_TotalNumberOfItems + pos - 1;
    QString cmd = QString("%1GGI").arg(pos, 5, 10, QChar('0'));
    emit SendCmd(cmd);
}

void usbDialog::on_PageDownButton_clicked()
{
    int pos = m_IndexOfLine1 + m_SelectedItemIndex;
    pos += m_VisibleListSize;
    if (pos >= m_TotalNumberOfItems)
        pos = pos % m_TotalNumberOfItems -1;
    QString cmd = QString("%1GGI").arg(pos, 5, 10, QChar('0'));
    emit SendCmd(cmd);
}

void usbDialog::on_PageUpButton_2_clicked()
{
    int pos = m_IndexOfLine1 + m_SelectedItemIndex;
    pos -= 100;
    if (pos <= 1)
        pos=1;
//        pos = m_TotalNumberOfItems + pos - 1;
    QString cmd = QString("%1GGI").arg(pos, 5, 10, QChar('0'));
    emit SendCmd(cmd);
}

void usbDialog::on_PageDownButton_2_clicked()
{
    int pos = m_IndexOfLine1 + m_SelectedItemIndex;
    pos += 100;
    if (pos >= m_TotalNumberOfItems)
//        pos = pos % m_TotalNumberOfItems - 1;
          pos= m_TotalNumberOfItems-8;
        QString cmd = QString("%1GGI").arg(pos, 5, 10, QChar('0'));
    emit SendCmd(cmd);
}

void usbDialog::on_listWidget_itemDoubleClicked(QListWidgetItem * /*item*/)
{
//    qDebug() << "Current row " << currentRow;
    int currentRow = ui->listWidget->currentRow();
    QString cmd = QString("%1GFI").arg((uint)(currentRow + 1), 2, 10, QChar('0'));
    emit SendCmd(cmd);
}

void usbDialog::on_bicontrol_clicked()
{
    emit SendCmd("10IP"); // icontrol
}

void usbDialog::on_bdirectctl_clicked()
{
    emit SendCmd("20IP"); // direct icontrol
}

void usbDialog::on_brandom_clicked()
{
    emit SendCmd("08IP"); // random weiterschalten
}


void usbDialog::on_brepeat_clicked()
{
    emit SendCmd("07IP"); // repeat weiterschalten
}
