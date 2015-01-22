#include "hdmicontroldialog.h"
#include "ui_hdmicontroldialog.h"
#include <QDebug>

HdmiControlDialog::HdmiControlDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::HdmiControlDialog),
    m_Comm(Comm),
    m_PositionSet(false),
    m_PowerOn(false),
    m_LastEnabled(false)
{
    ui->setupUi(this);

    // restore the position of the window
    if (m_Settings.value("SaveHdmiControlWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("HdmiControlWindowGeometry").toByteArray());
    }

    QStringList mstr;
    mstr << "Off" << "Last" << "BD" << "HDMI 1" << "HDMI 2" << "HDMI 3" << "HDMI 4" << "HDMI 5" << "HDMI 6" << "HDMI 7" << "HDMI 8" << "HDMI 9" << "HDMI 10";
    ui->standbyPassThroughComboBox->addItems(mstr);

    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));
    connect(&m_Comm, SIGNAL(DataReceived(QString)), this, SLOT(DataReceived(QString)));
    connect(&Comm, SIGNAL(Connected()), this, SLOT(CommConnected()));
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

void HdmiControlDialog::DataReceived(QString data)
{
    int n = 0;
    if (data.startsWith("STQ"))
    {
        sscanf(data.toLatin1(), "STQ%d", &n);
        ui->hdmiControlCheckBox->setChecked(n != 0);
    }
    else if (data.startsWith("PWR"))
    {
        int n = 0;
        sscanf(data.toLatin1(), "PWR%d", &n);
        m_PowerOn = (n == 0);
    }
    else if (data.startsWith("STR"))
    {
        sscanf(data.toLatin1(), "STR%d", &n);
        ui->hdmiControlModeCheckBox->setChecked(n != 0);
    }
    else if (data.startsWith("STT"))
    {
        sscanf(data.toLatin1(), "STT%d", &n);
        ui->arcCheckBox->setChecked(n != 0);
    }
    else if (data.startsWith("STU"))
    {
        sscanf(data.toLatin1(), "STU%d", &n);
        m_LastEnabled = (n == 1);
        if (n >= 0 && n < ui->standbyPassThroughComboBox->count())
            ui->standbyPassThroughComboBox->setCurrentIndex(n);
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

bool HdmiControlDialog::IsLastEnabled()
{
    return m_LastEnabled;
}
