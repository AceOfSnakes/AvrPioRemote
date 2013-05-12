#include "usbdialog.h"
#include "ui_usbdialog.h"
#include <QDebug>
#include <qtextcodec.h>


usbDialog::usbDialog(QWidget *parent, QSettings &settings, ReceiverInterface &Comm) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::usbDialog),
    m_Comm(Comm),
    m_PositionSet(false)
{
    m_IndexOfLine1 = 0;
    m_IndexOfLastLine = 0;
    m_TotalNumberOfItems = 0;
    m_SelectedItemIndex = 0;
    m_VisibleListSize = 0;

    ui->setupUi(this);

    this->setFixedSize(this->size());

    // restore the position of the window
    if (m_Settings.value("SaveUsbWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("UsbWindowGeometry").toByteArray());
    }

    connect((&m_Comm), SIGNAL(usbData(QString)), this, SLOT(usbrecData(QString)));
    connect((this),    SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));
    connect((&m_Comm), SIGNAL(DisplayData(int, QString)),this,SLOT(randrepeattest(int,QString)));

    connect((&m_Timer), SIGNAL(timeout()), this, SLOT(Timeout()));

    m_Timer.setSingleShot(false);
    m_Timer.setInterval(10000);
//    Timeout();
    ui->brandom->setText(tr("Random off"));
    ui->brepeat->setText(tr("Repeat off"));
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


void usbDialog::ShowusbDialog()
{   
    if (m_Settings.value("AutoShowUSB", true).toBool() && !isVisible())
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




void usbDialog::Timeout()
{
    if(isVisible())
        emit SendCmd("?GAI");
}


void usbDialog::usbrecData(QString data)
{
     qDebug() << " >>>>>>>>>>>>>>>>>>>>> " << data;
    if (data.startsWith("GBI"))
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
//        int LineNumber = data.mid(3, 2).toInt();
        int FocusInformation = data.mid(5, 1).toInt();
//        int LineDataType = data.mid(6, 2).toInt();
        QString DisplayLine = data.mid(8);
        DisplayLine = DisplayLine.trimmed();
        while (DisplayLine.startsWith("\""))
            DisplayLine.remove(0, 1);
        while (DisplayLine.endsWith("\""))
            DisplayLine.chop(1);
//        qDebug() << DisplayLine <<"GEI       xxxxxxxxxxxxxxxxxxxx";

        if (DisplayLine!="0")
           ui->listWidget->addItem(DisplayLine);
        if (FocusInformation)
        {
            QBrush brush(QColor(0, 0, 255));
            //int index = ui->listWidgetLog->currentIndex().row();
            int index = ui->listWidget->count() - 1;
            ui->listWidget->item(index)->setForeground(brush);
            m_SelectedItemIndex = index;
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

void usbDialog::on_listWidget_itemDoubleClicked(QListWidgetItem */*item*/)
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

void usbDialog::randrepeattest(int no, QString name)
{
    QString str,str1;
    str=name.trimmed();
    str1=str.mid(0,6);
    if(str1=="Repeat")
        ui->brepeat->setText(str);
    else if(str1=="Random")
        ui->brandom->setText(str);
}
