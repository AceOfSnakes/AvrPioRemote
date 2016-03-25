#include "mcaccprogressdialog.h"
#include "ui_mcaccprogressdialog.h"

MCACCProgressDialog::MCACCProgressDialog(QWidget *parent, QSettings &settings, ReceiverInterface &Comm) :
    QDialog(parent),
    ui(new Ui::MCACCProgressDialog),
    m_Settings(settings),
    m_Comm(Comm),
    m_RefreshTimer(this)
{
    ui->setupUi(this);

    // restore the position of the window
    if (m_Settings.value("SaveMCACCProgressWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("MCACCProgressWindowGeometry").toByteArray());
    }

    connect((&m_RefreshTimer), SIGNAL(timeout()), this, SLOT(RefreshTimeout()));
    m_RefreshTimer.setSingleShot(true);
    m_RefreshTimer.setInterval(1000);

    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));

    QStringList responseList;
    responseList << MCACCProgressResponse_SSJ().getResponseID();
    MsgDistributor::AddResponseListener(this, responseList);
}

MCACCProgressDialog::~MCACCProgressDialog()
{
    delete ui;
}

void MCACCProgressDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MCACCProgressDialog::closeEvent(QCloseEvent *event)
{
    //m_Comm.disconnect(SIGNAL(DataReceived(QString)));
    QDialog::closeEvent(event);
}

void MCACCProgressDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("MCACCProgressWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}

void MCACCProgressDialog::ShowMCACCProgressDialog()
{
    if (!isVisible())
    {
        if (!m_PositionSet || !m_Settings.value("SaveMCACCProgressWindowGeometry", false).toBool())
        {
            QWidget* Parent = dynamic_cast<QWidget*>(parent());
            int x = Parent->pos().x() + Parent->width() + 20;
            QPoint pos;
            pos.setX(x);
            pos.setY(Parent->pos().y());
            this->move(pos);
        }
        this->show();
        m_RefreshTimer.start();
        SendCmd("?SSJ");
    } else {
        this->hide();
    }
}

void MCACCProgressDialog::ResponseReceived(ReceivedObjectBase *response)
{
    // MCACC progress
    MCACCProgressResponse_SSJ* progress = dynamic_cast<MCACCProgressResponse_SSJ*>(response);
    if (progress)
    {
        ui->progressBar->setMaximum(progress->GetTotal());
        ui->progressBar->setValue(progress->GetProgress());
        ui->errorLabel->setText(progress->GetErrorDescription());
        return;
    }
}

void MCACCProgressDialog::RefreshTimeout()
{
    if (isVisible()) {
        SendCmd("?SSJ");
        m_RefreshTimer.start();
    }
}

