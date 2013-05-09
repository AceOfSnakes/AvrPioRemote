#include "testdialog.h"
#include "ui_testdialog.h"
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>


TestDialog::TestDialog(QWidget *parent, ReceiverInterface &Comm, QSettings &Settings) :
    QDialog(parent),
    ui(new Ui::TestDialog),
    m_Comm(Comm),
    m_Settings(Settings),
    m_PositionSet(false)
{
    ui->setupUi(this);

    // restore the position of the window
    if (m_Settings.value("SaveTestWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("TestWindowGeometry").toByteArray());
    }

    connect((&m_Comm), SIGNAL(DataReceived(QString)), this,  SLOT(NewDataReceived(QString)));
    connect((this),    SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));
    connect((&m_Comm),  SIGNAL(CmdToBeSend(QString)), this,  SLOT(LogSendCmd(QString)));
}


TestDialog::~TestDialog()
{
    delete ui;
}


void TestDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("TestWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}


void TestDialog::ShowTestDialog()
{
    if (!this->isVisible())
    {
        if (!m_PositionSet || !m_Settings.value("SaveTestWindowGeometry", false).toBool())
        {
            QWidget* Parent = dynamic_cast<QWidget*>(parent());
            int x = Parent->pos().x() - 20 - this->width();
            QPoint pos;
            pos.setX(x);
            pos.setY(Parent->pos().y());
            this->move(pos);
        }
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


void TestDialog::on_SaveButton_clicked()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                              QString(), tr("Log file (*.txt)"));
    if (fn.isEmpty())
        return;
    if (!(fn.endsWith(".txt", Qt::CaseInsensitive)))
        fn += ".txt";

    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QString msg = QString("Error: Cannot write file <%1>: %2").arg(fn).arg(file.errorString());
        Logger::Log(msg);
        return;
    }
    QTextStream ts(&file);
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        ts << ui->listWidget->item(i)->text() << endl;
    }
}
