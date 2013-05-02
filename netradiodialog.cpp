#include "netradiodialog.h"
#include "ui_netradiodialog.h"
#include <QDebug>
#include <qtextcodec.h>

NetRadioDialog::NetRadioDialog(QWidget *parent, QSettings &settings) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::NetRadioDialog)
{
    m_IndexOfLine1 = 0;
    m_IndexOfLastLine = 0;
    m_TotalNumberOfItems = 0;
    m_SelectedItemIndex = 0;
    m_VisibleListSize = 0;

    ui->setupUi(this);

    this->setFixedSize(this->size());
}

NetRadioDialog::~NetRadioDialog()
{
    delete ui;
}

void NetRadioDialog::NetData(QString data)
{
    //qDebug() << " >>> " << data;
    if (data.startsWith("GBH"))
    {
        m_VisibleListSize = data.mid(3, 2).toInt();
        ui->listWidget->clear();
        // qDebug() << "maximum number of list " << n;
    }
//    else if (data.startsWith("GCH"))
//    {
//        int screenType = data.mid(3, 2).toInt();
//        int listUpdateFlag = data.mid(5, 1).toInt();
//        int topMenuKey = data.mid(6, 1).toInt();
//        int toolsKey = data.mid(7, 1).toInt();
//        int returnKey = data.mid(8, 1).toInt();
//        QString screenName = data.mid(10);
//        qDebug() << "ScreenType " << screenType << " ListUpdateFlag " << listUpdateFlag
//                 << " TopMenuKey " << topMenuKey << " ToolsKey " << toolsKey
//                 << " ReturnKey " << returnKey << " ScreenName <" << screenName << ">";
//    }
    else if (data.startsWith("GDH"))
    {
        m_IndexOfLine1 = data.mid(3, 5).toInt();
        m_IndexOfLastLine = data.mid(8, 5).toInt();
        m_TotalNumberOfItems = data.mid(13, 5).toInt();
//        qDebug() << "IndexOfLine1 " << m_IndexOfLine1
//                 << " IndexOfLastLine " << m_IndexOfLastLine
//                 << " TotalNumberOfItems " << m_TotalNumberOfItems;
    }
    else if (data.startsWith("GEH"))
    {
//        int LineNumber = data.mid(3, 2).toInt();
        int FocusInformation = data.mid(5, 1).toInt();
//        int LineDataType = data.mid(6, 2).toInt();
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
        ui->listWidget->addItem(DisplayLine);
        if (FocusInformation)
        {
            QBrush brush(QColor(0, 0, 255));
            //int index = ui->listWidgetLog->currentIndex().row();
            int index = ui->listWidget->count() - 1;
            ui->listWidget->item(index)->setForeground(brush);
            m_SelectedItemIndex = index;
        }
//        qDebug() << "LineNumber " << LineNumber << " FocusInformation " << FocusInformation
//                 << " LineDataType " << LineDataType << " DisplayLine <" << DisplayLine << ">";
    }
    else if (data.startsWith("GHH"))
    {
        QString text = QString("%1/%2").arg(m_IndexOfLine1 + m_SelectedItemIndex).arg(m_TotalNumberOfItems);
        ui->UnderListEdit->setText(text);
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

void NetRadioDialog::on_listWidget_currentRowChanged(int currentRow)
{
//    qDebug() << "Current row " << currentRow;
//    QString cmd = QString("%1GFH").arg((uint)(currentRow + 1), 2, 10, QChar('0'));
//    emit SendCmd(cmd);
}

void NetRadioDialog::on_PageUpButton_clicked()
{
    int pos = m_IndexOfLine1 + m_SelectedItemIndex;
    pos -= m_VisibleListSize;
    if (pos <= 1)
        pos = m_TotalNumberOfItems + pos - 1;
    QString cmd = QString("%1GGH").arg(pos, 5, 10, QChar('0'));
    emit SendCmd(cmd);
}

void NetRadioDialog::on_PageDownButton_clicked()
{
    int pos = m_IndexOfLine1 + m_SelectedItemIndex;
    pos += m_VisibleListSize;
    if (pos >= m_TotalNumberOfItems)
        pos = pos % m_TotalNumberOfItems -1;
    QString cmd = QString("%1GGH").arg(pos, 5, 10, QChar('0'));
    emit SendCmd(cmd);
}

void NetRadioDialog::on_PageUpButton_2_clicked()
{
    int pos = m_IndexOfLine1 + m_SelectedItemIndex;
    pos -= 100;
    if (pos <= 1)
        pos = m_TotalNumberOfItems + pos - 1;
    QString cmd = QString("%1GGH").arg(pos, 5, 10, QChar('0'));
    emit SendCmd(cmd);
}

void NetRadioDialog::on_PageDownButton_2_clicked()
{
    int pos = m_IndexOfLine1 + m_SelectedItemIndex;
    pos += 100;
    if (pos >= m_TotalNumberOfItems)
        pos = pos % m_TotalNumberOfItems - 1;
    QString cmd = QString("%1GGH").arg(pos, 5, 10, QChar('0'));
    emit SendCmd(cmd);
}

void NetRadioDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
//    qDebug() << "Current row " << currentRow;
    int currentRow = ui->listWidget->currentRow();
    QString cmd = QString("%1GFH").arg((uint)(currentRow + 1), 2, 10, QChar('0'));
    emit SendCmd(cmd);
}
