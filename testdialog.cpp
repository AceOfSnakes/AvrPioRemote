#include "testdialog.h"
#include "ui_testdialog.h"
#include <QDebug>

TestDialog::TestDialog(QWidget *parent, ReceiverInterface &Comm) :
    QDialog(parent),
    ui(new Ui::TestDialog),
    m_Comm(Comm)
{
    ui->setupUi(this);

    connect((&m_Comm), SIGNAL(DataReceived(QString)), this,  SLOT(NewDataReceived(QString)));
    connect((this),    SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));
    connect((&m_Comm),  SIGNAL(CmdToBeSend(QString)), this,  SLOT(LogSendCmd(QString)));
}

TestDialog::~TestDialog()
{
    delete ui;
}

void TestDialog::ShowTestDialog()
{
    if (!this->isVisible())
    {
        QWidget* Parent = dynamic_cast<QWidget*>(parent());
        int x = Parent->pos().x() - 20 - this->width();
        QPoint pos;
        pos.setX(x);
        pos.setY(Parent->pos().y());
        this->move(pos);
        this->show();
    }
}


void TestDialog::on_ClearButton_clicked()
{
    //ui->lineEdit->clear();
    ui->listWidget->clear();
}

void TestDialog::NewDataReceived(QString data)
{
    AddToList("<-- " + data);
}

void TestDialog::LogSendCmd(QString data)
{
    AddToList("--> " + data);
}

void TestDialog::AddToList(const QString& str)
{
    if (ui->listWidget->count() > 1000)
    {
        delete ui->listWidget->item(0);
    }
    ui->listWidget->addItem(str);
    if (ui->AutoScrollingCheckBox->isChecked())
    {
        ui->listWidget->scrollToBottom();
    }
}

void TestDialog::on_SendButton_clicked()
{
    QString str = ui->lineEdit->text().trimmed();
    if (str != "")
    {
        emit SendCmd(str);
    }
}
