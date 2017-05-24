#include "wiringdialog.h"
#include "ui_wiringdialog.h"
#include <QDebug>

const STRING_PAIR WIRING_INPUTS[] = {
    {"00", "PHONO"},
    {"01", "CD"},
    {"02", "TUNER"},
    {"03", "CD-R / TAPE"},
    {"04", "DVD"},
    {"05", "TV / SAT"},
    {"06", "SAT/CBL"},
    {"10", "VIDEO 1"},//(VIDEO)",
    {"12", "MULTI CH IN"},
    {"13", "USB DAC"},
    {"14", "VIDEO 2"},
    {"15", "DVR / BDR"},
    {"17", "iPod / USB"},
    //{"18", "XM RADIO"},
    {"19", "HDMI 1"},
    {"20", "HDMI 2"},
    {"21", "HDMI 3"},
    {"22", "HDMI 4"},
    {"23", "HDMI 5"},
    {"24", "HDMI 6"},
    {"25", "BD"},
    //{"26", "HMG (Internet Radio)"}, //HOME MEDIA GALLERY
    //{"27", "SIRIUS"},
    //{"31", "HDMI (cyclic)"},
    {"33", "ADAPTER PORT"},
    {"34", "HDMI 7"},
    //{"38", "INTERNET RADIO"},
    //{"40", "SiriusXM"},
    //{"41", "PANDORA"},
    //{"44", "MEDIA SERVER"},
    //{"45", "FAVORITES"},
    {"48", "MHL"},
    {"", ""}
};

WiringDialog::WiringDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::WiringDialog),
    m_Comm(Comm),
    m_PositionSet(false),
    m_ParameterErrorCount(0),
    m_InputAnswersCount(0),
    m_CurrentAction(ACTION_NOTHING)
{
    ui->setupUi(this);

    // restore the position of the window
    if (m_Settings.value("SaveWiringWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("WiringWindowGeometry").toByteArray());
    }

    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));
    ui->tableView->setModel(&m_WiringModel);
    ComboBoxDelegate* combodelegate = new ComboBoxDelegate(this);
    ui->tableView->setItemDelegateForColumn(2, combodelegate);
    ui->tableView->setItemDelegateForColumn(3, combodelegate);
    ui->tableView->setItemDelegateForColumn(4, combodelegate);
    CheckBoxDelegate* checkdelegate = new CheckBoxDelegate(this);
    ui->tableView->setItemDelegateForColumn(5, checkdelegate);

}

WiringDialog::~WiringDialog()
{
    delete ui;
}

void WiringDialog::changeEvent(QEvent *e)
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

void WiringDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("WiringWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}

void WiringDialog::ShowWiringDialog()
{
    if ((/*!autoShow) || (m_Settings.value("AutoShowTuner", true).toBool() && */!isVisible()))
    {
        if (!m_PositionSet || !m_Settings.value("SaveWiringWindowGeometry", false).toBool())
        {
            QWidget* Parent = dynamic_cast<QWidget*>(parent());
            int x = Parent->pos().x() + Parent->width() + 20;
            QPoint pos;
            pos.setX(x);
            pos.setY(Parent->pos().y());
            this->move(pos);
        }
        connect(&m_Comm, SIGNAL(DataReceived(QString)), this, SLOT(DataReceived(QString)));
        this->show();
        //SendCmd("?SSC0400");

        AquireData();

        //        EnableControls(true);
//        ui->SaveButton->setEnabled(false);
//        ui->OkButton->setEnabled(false);
//        ui->FrequencyEdit->setReadOnly(true);
//        ui->FrequencyEdit->setInputMask("");
//        ui->RenamePresetButton->setEnabled(true);
//        ui->CancelButton->setEnabled(false);
    }
//    SendCmd("?FR");
//    SendCmd("?PR");
}

void WiringDialog::ResponseReceived(ReceivedObjectBase *response)
{
    InputNameResponse_RGB* inputname = dynamic_cast<InputNameResponse_RGB*>(response);
    if (inputname != NULL)
    {
        if (m_CurrentAction == ACTION_GET_NAME)
        {
            QString id = QString("%1").arg(inputname->GetInputNo(), 2, 10, (QChar)'0');
            if (m_CurrentInput->id == id)
            {
                m_CurrentInput->alias = inputname->GetInputName();
                m_ParameterErrorCount = 0;
                m_CurrentAction = ACTION_GET_AUDIO_ASSIGNMENT;
                QString cmd = QString("?SSC%1000").arg(m_CurrentInput->id);
                SendCmd(cmd);
                return;
            }
            QString str = QString("?RGB%1").arg(m_CurrentInput->id);
            SendCmd(str);
            return;
        }
        return;
    }
}
void WiringDialog::AquireData()
{
    m_ParameterErrorCount = 0;
    m_CurrentAction = ACTION_GET_NAME;
    //qDebug() << "Start aquiring";
    //m_WiringModel.m_Inputs.clear();
    //m_WiringModel.refreshView();
    //m_WiringModel.removeRows(0, m_WiringModel.rowCount());
    //ui->tableView->reset();
    m_Inputs.clear();
    m_WiringModel.clear();
    int i = 0;
    while(strlen(WIRING_INPUTS[i].key) != 0)
    {
        m_Inputs.append(InputContainer(WIRING_INPUTS[i].key, WIRING_INPUTS[i].text));
        i++;
    }
    m_CurrentInput = m_Inputs.begin();
    QString str = QString("?RGB%1").arg(m_CurrentInput->id);
    SendCmd(str);
}

void WiringDialog::DataReceived(QString data)
{
    //qDebug() << "Wiring received: " << data;
    if (m_CurrentAction == ACTION_NOTHING)
        return;

    if (data.startsWith("E06"))
    {
        m_ParameterErrorCount++;
        if (m_ParameterErrorCount > 2)
        {
            m_ParameterErrorCount = 0;
            //qDebug() << "Erase " << m_CurrentInput->id << ", " << m_CurrentInput->name << ", count " << m_Inputs.size();
            m_CurrentInput = m_Inputs.erase(m_CurrentInput);
            if (m_CurrentInput == m_Inputs.end())
            {
                m_CurrentAction = ACTION_NOTHING;
                m_WiringModel.FillTable(m_Inputs);
                // Show the current name in the main window
                SendCmd("?F");
                //qDebug() << "Aquire done";
                return;
            }
            m_CurrentAction = ACTION_GET_NAME;
        }
    }

    if (m_CurrentAction == ACTION_GET_AUDIO_ASSIGNMENT)
    {
        if (data.startsWith("SSC"))
        {
            if (data.length() >= 8)
            {
                QString tmp = data.mid(5, 2);
                if (tmp == "00")
                {
                    QString id = data.mid(3, 2);
                    QString audio = data.mid(7, 2);
                    if (m_CurrentInput->id == id)
                    {
                        m_CurrentInput->audio = audio;
                        m_ParameterErrorCount = 0;
                        m_CurrentAction = ACTION_GET_HDMI_ASSIGNMENT;
                        QString cmd = QString("?SSC%1001").arg(m_CurrentInput->id);
                        SendCmd(cmd);
                        return;
                    }
                }
            }
        }
        QString cmd = QString("?SSC%1000").arg(m_CurrentInput->id);
        SendCmd(cmd);
        return;
    }
    else if (m_CurrentAction == ACTION_GET_HDMI_ASSIGNMENT)
    {
        if (data.startsWith("SSC"))
        {
            if (data.length() >= 8)
            {
                QString tmp = data.mid(5, 2);
                if (tmp == "01")
                {
                    QString id = data.mid(3, 2);
                    QString hdmi = data.mid(7, 2);
                    if (m_CurrentInput->id == id)
                    {
                        m_CurrentInput->hdmi = hdmi;
                        m_ParameterErrorCount = 0;
                        m_CurrentAction = ACTION_GET_VIDEO_ASSIGNMENT;
                        QString cmd = QString("?SSC%1002").arg(m_CurrentInput->id);
                        SendCmd(cmd);
                        return;
                    }
                }
            }
        }
        QString cmd = QString("?SSC%1001").arg(m_CurrentInput->id);
        SendCmd(cmd);
        return;
    }
    else if (m_CurrentAction == ACTION_GET_VIDEO_ASSIGNMENT)
    {
        if (data.startsWith("SSC"))
        {
            if (data.length() >= 8)
            {
                QString tmp = data.mid(5, 2);
                if (tmp == "02")
                {
                    QString id = data.mid(3, 2);
                    QString video = data.mid(7, 2);
                    if (m_CurrentInput->id == id)
                    {
                        m_CurrentInput->video = video;
                        m_ParameterErrorCount = 0;
                        m_CurrentAction = ACTION_GET_SKIP;
                        QString cmd = QString("?SSC%1003").arg(m_CurrentInput->id);
                        SendCmd(cmd);
                        return;
                    }
                }
            }
        }
        QString cmd = QString("?SSC%1002").arg(m_CurrentInput->id);
        SendCmd(cmd);
        return;
    }
    else if (m_CurrentAction == ACTION_GET_SKIP)
    {
        if (data.startsWith("SSC"))
        {
            if (data.length() >= 8)
            {
                QString tmp = data.mid(5, 2);
                if (tmp == "03")
                {
                    QString id = data.mid(3, 2);
                    QString skip = data.mid(7, 2);
                    if (m_CurrentInput->id == id)
                    {
                        m_CurrentInput->skip = skip;
                        m_ParameterErrorCount = 0;
                        m_CurrentInput++;
                        if (m_CurrentInput == m_Inputs.end())
                        {
                            m_CurrentAction = ACTION_NOTHING;
                            //qDebug() << "Aquire done";
                            //m_WiringModel.refreshView();
                            m_WiringModel.FillTable(m_Inputs);
                            // Show the current name in the main window
                            SendCmd("?F");
                            return;
                        }
                        m_CurrentAction = ACTION_GET_NAME;
                        QString str = QString("?RGB%1").arg(m_CurrentInput->id);
                        SendCmd(str);
                        return;
                    }
                }
            }
        }
        QString cmd = QString("?SSC%1003").arg(m_CurrentInput->id);
        SendCmd(cmd);
        return;
    }
}

void WiringDialog::closeEvent(QCloseEvent *event)
{
    //m_ReceiverInterface.Disconnect();
    // save the window position
    //m_Settings.setValue("MainWindowGeometry", saveGeometry());
    //qDebug() << "Wiring close";
    m_Comm.disconnect(SIGNAL(DataReceived(QString)));
    QDialog::closeEvent(event);
}
