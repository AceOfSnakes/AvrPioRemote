#include "testdialog.h"
#include "ui_testdialog.h"
#include <QDebug>

TestDialog::TestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestDialog),
    m_Timer(this)
{
    ui->setupUi(this);
    m_Done = true;
    m_Recording = false;

    connect((&m_Timer), SIGNAL(timeout()), this, SLOT(Timeout()));
    m_Timer.setSingleShot(true);
}

TestDialog::~TestDialog()
{
    delete ui;
}


void TestDialog::Timeout()
{
   //m_Done = true;
    QString str = m_Remembered.trimmed();
    if (str == "")
    {
        m_Remembered = "";
        m_Firstline = "";
        emit SendCmd("27NW");
        return;
    }
    if (m_FavList.contains(m_Remembered))
    {
        m_Recording = false;
        m_Done = true;
        ui->lineEdit->setText("");
    }
    else
    {
        m_FavList.append(m_Remembered);
        ui->listWidget->addItem(m_Remembered);
        m_Remembered = "";
        m_Firstline = "";
        emit SendCmd("27NW");
    }
}


void TestDialog::InputFunctionData(int no, QString name)
{
//    qDebug() << "INPUT " << no << " " << name;
//    if (no == 45 && !m_Done)
//    {
//        m_Recording = true;
//    }
}


void TestDialog::on_ClearButton_clicked()
{
    ui->lineEdit->clear();
    ui->listWidget->clear();
    m_Remembered = "";
    m_Firstline = "";
    m_Done = false;
    m_Recording = true;
    m_FavList.clear();
   // m_Recording = true;
    emit SendCmd("20NW");
    emit SendCmd("45FN");
//    emit SendCmd("27NW");
    m_Timer.start(2000);
}

void TestDialog::DisplayData(int no, QString data)
{
//    QString str = QString("%1 <%2>").arg(no).arg(data);
//    qDebug() << str;
    if (no == 3 && m_Recording)
    {
        m_Timer.stop();
        if (!m_Done)
        {
            GetScrolledString(data);
            ui->lineEdit->setText(m_Remembered);
        }
        else
        {
            ui->lineEdit->setText("");
            QString str = m_Remembered.trimmed();
            if (str != "")
            {

                if (m_FavList.contains(m_Remembered))
                {
                    m_Done = true;
                    m_Remembered = "";
                    m_Firstline = "";
                    m_Recording = false;
                    return;
                }
                m_Done = false;
                ui->listWidget->addItem(m_Remembered);
                m_FavList.append(m_Remembered);
            }
            m_Remembered = "";
            m_Firstline = "";
            emit SendCmd("27NW");
        }
        //emit SendCmd("29NW"); // don't go back to play window
        m_Timer.start(2000);
    }
}

bool TestDialog::GetScrolledString(QString input)
{
    input = input.mid(1);
    if (m_Firstline == input)
    {
        m_Remembered = m_Remembered.mid(0, m_Remembered.count() - m_Firstline.count() + 1);
        m_Done = true;
        return true;
    }
    QString tmp = input.mid(0, input.count() - 2);
    int pos = m_Remembered.indexOf(tmp);
//    qDebug() << "|" << input << "| |" << tmp << "| " << pos;
    if (pos == -1)
    {
        m_Remembered = input;
        m_Firstline = input;
    }
    else
    {
        m_Remembered.replace(pos, input.length(), input);
    }
    return false;
}
