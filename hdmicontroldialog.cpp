#include "hdmicontroldialog.h"
#include "ui_hdmicontroldialog.h"
#include <QDebug>

HdmiControlDialog::HdmiControlDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::HdmiControlDialog),
    m_Comm(Comm),
    m_PositionSet(false),
    m_PowerOn(false)
{
    ui->setupUi(this);

    // restore the position of the window
    if (m_Settings.value("SaveHdmiControlWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("HdmiControlWindowGeometry").toByteArray());
    }

    QStringList mstr;
    mstr << HDMIPassThroughResponse::GetFunctionNamesList();
    ui->standbyPassThroughComboBox->addItems(mstr);

    QStringList responseList;
    responseList << PowerResponse().getResponseID();
    responseList << HDMIPassThroughResponse().getResponseID();
    responseList << HDMIControlResponse().getResponseID();
    responseList << HDMIControlModeResponse().getResponseID();
    responseList << HDMIControlARCResponse().getResponseID();
    MsgDistributor::AddResponseListener(this, responseList);

    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));
    connect(&m_Comm, SIGNAL(Connected()), this, SLOT(CommConnected()));
}

HdmiControlDialog::~HdmiControlDialog()
{
    delete ui;
}

void HdmiControlDialog::changeEvent(QEvent *e)
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

void HdmiControlDialog::closeEvent(QCloseEvent *event)
{
    //m_Comm.disconnect(SIGNAL(DataReceived(QString)));
    QDialog::closeEvent(event);
}

void HdmiControlDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("HdmiControlWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}

void HdmiControlDialog::ShowHdmiControlDialog()
{
    if (!isVisible())
    {
        if (!m_PositionSet || !m_Settings.value("SaveHdmiControlWindowGeometry", false).toBool())
        {
            QWidget* Parent = dynamic_cast<QWidget*>(parent());
            int x = Parent->pos().x() + Parent->width() + 20;
            QPoint pos;
            pos.setX(x);
            pos.setY(Parent->pos().y());
            this->move(pos);
        }
        this->show();
    }
    if (m_PowerOn)
    {
        SendCmd("?STQ");
        SendCmd("?STR");
        SendCmd("?STT");
    }
    SendCmd("?STU");
}

void HdmiControlDialog::ResponseReceived(ReceivedObjectBase *response)
{
    // Power
    PowerResponse* power = dynamic_cast<PowerResponse*>(response);
    if (power != NULL && power->GetZone() == PowerResponse::ZoneMain)
    {
        m_PowerOn = power->IsPoweredOn();
        return;
    }
    // HDMI pass through
    HDMIPassThroughResponse* pass_through = dynamic_cast<HDMIPassThroughResponse*>(response);
    if (pass_through != NULL)
    {
        int n = pass_through->GetPassThroughFunction();
        if (n >= 0 && n < ui->standbyPassThroughComboBox->count())
            ui->standbyPassThroughComboBox->setCurrentIndex(n);
        return;
    }
    // HDMI Control
    HDMIControlResponse* control = dynamic_cast<HDMIControlResponse*>(response);
    if (control != NULL)
    {
        ui->hdmiControlCheckBox->setChecked(control->IsHDMIControlOn());
        return;
    }
    // HDMI Control Mode
    HDMIControlModeResponse* control_mode = dynamic_cast<HDMIControlModeResponse*>(response);
    if (control_mode != NULL)
    {
        ui->hdmiControlModeCheckBox->setChecked(control_mode->IsHDMIControlModeOn());
        return;
    }
    // ARC
    HDMIControlARCResponse* control_arc = dynamic_cast<HDMIControlARCResponse*>(response);
    if (control_arc != NULL)
    {
        ui->arcCheckBox->setChecked(control_arc->IsHDMIControlARCOn());
        return;
    }
}

void HdmiControlDialog::CommConnected()
{
    SendCmd("?STU");
}

void HdmiControlDialog::on_hdmiControlCheckBox_toggled(bool on)
{
    if (on)
        SendCmd("1STQ");
    else
        SendCmd("0STQ");
}

void HdmiControlDialog::on_hdmiControlModeCheckBox_toggled(bool on)
{
    if (on)
        SendCmd("1STR");
    else
        SendCmd("0STR");
}

void HdmiControlDialog::on_arcCheckBox_toggled(bool on)
{
    if (on)
        SendCmd("1STT");
    else
        SendCmd("0STT");
}

void HdmiControlDialog::on_standbyPassThroughComboBox_currentIndexChanged(int n)
{
    SendCmd(QString("%1STU").arg(n, 2, 10, QChar('0')));
}

