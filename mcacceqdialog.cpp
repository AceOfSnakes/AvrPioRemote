#include "mcacceqdialog.h"
#include "ui_mcacceqdialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QThread>

MCACCEQDialog::MCACCEQDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm) :
    QDialog(parent),
    ui(new Ui::MCACCEQDialog),
    m_Settings(settings),
    m_Comm(Comm),
    m_PositionSet(false),
    m_CurrentMcacc(0),
    m_SelectedChannel(0)
{
    ui->setupUi(this);

    //this->setFixedSize(this->size());

    // restore the position of the window
    if (m_Settings.value("SaveMCACCEQWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("MCACCEQWindowGeometry").toByteArray());
    }

    ui->groupBoxMCACC->setEnabled(false);

    m_Slider.append(ui->Slider63Hz);
    m_Slider.append(ui->Slider125Hz);
    m_Slider.append(ui->Slider250Hz);
    m_Slider.append(ui->Slider500Hz);
    m_Slider.append(ui->Slider1kHz);
    m_Slider.append(ui->Slider2kHz);
    m_Slider.append(ui->Slider4kHz);
    m_Slider.append(ui->Slider8kHz);
    m_Slider.append(ui->Slider16kHz);
    m_Slider.append(ui->SliderWideTrim);
    m_Slider.append(ui->Slider31Hz);

    m_Labels.append(ui->Label63Hz);
    m_Labels.append(ui->Label125Hz);
    m_Labels.append(ui->Label250Hz);
    m_Labels.append(ui->Label500Hz);
    m_Labels.append(ui->Label1kHz);
    m_Labels.append(ui->Label2kHz);
    m_Labels.append(ui->Label4kHz);
    m_Labels.append(ui->Label8kHz);
    m_Labels.append(ui->Label16kHz);
    m_Labels.append(ui->LabelWideTrim);
    m_Labels.append(ui->Label31Hz);

    addChannel("L__", ui->RadioButtonFrontLeftCh);
    addChannel("R__", ui->RadioButtonFrontRightCh);
    addChannel("C__", ui->RadioButtonCenterCh);
    addChannel("SL_", ui->RadioButtonSurroundLeftCh);
    addChannel("SR_", ui->RadioButtonSurroundRightCh);
    addChannel("SBL", ui->RadioButtonSurroundBackLeftCh);
    addChannel("SBR", ui->RadioButtonSurroundBackRightCh);
    addChannel("LH_", ui->RadioButtonFrontHeightLeftCh);
    addChannel("RH_", ui->RadioButtonFrontHeightRightCh);
    addChannel("LW_", ui->RadioButtonFrontWideLeftCh);
    addChannel("RW_", ui->RadioButtonFrontWideRightCh);
    addChannel("SW_", ui->RadioButtonSW);
    addChannel("TML", ui->RadioButtonTopMiddleLeftCh);
    addChannel("TMR", ui->RadioButtonTopMiddleRightCh);
    addChannel("TFL", ui->RadioButtonTopFrontLeftCh);
    addChannel("TFR", ui->RadioButtonTopFrontRightCh);
    addChannel("TRL", ui->RadioButtonTopRearLeftCh);
    addChannel("TRR", ui->RadioButtonTopRearRightCh);
    addChannel("SW1", ui->RadioButtonSW1);
    addChannel("SW2", ui->RadioButtonSW2);

    for(int i = 0; i < (int)m_Slider.size(); i++)
    {
        m_Slider[i]->setEnabled(false);
        m_Slider[i]->setValue(50);
        connect(m_Slider[i],SIGNAL(sliderReleased()),this,SLOT(SliderValueChanged()));
    }

    m_Channels[0].radio->setChecked(true);
    for (int i = 0; i < (int)m_Channels.size(); i++)
    {
        m_Channels[i].radio->setEnabled(false);
        connect(m_Channels[i].radio, SIGNAL(clicked()) ,this, SLOT(ChannelClicked()));
    }

    ui->DistanceSpinBox->setValue(0);

    m_MCACCButtons << ui->radioButtonMCACC1;
    m_MCACCButtons << ui->radioButtonMCACC2;
    m_MCACCButtons << ui->radioButtonMCACC3;
    m_MCACCButtons << ui->radioButtonMCACC4;
    m_MCACCButtons << ui->radioButtonMCACC5;
    m_MCACCButtons << ui->radioButtonMCACC6;

    foreach (QRadioButton* radio, m_MCACCButtons) {
        connect(radio, SIGNAL(clicked()) ,this, SLOT(MCACC_selected()));
    }

    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));

    QStringList responseList;
    responseList << MCACCNumberResponse_MC().getResponseID();
    responseList << MCACCEQResponse_SUW().getResponseID();
    responseList << SpeakerDistanceResponse_SSS().getResponseID();
    MsgDistributor::AddResponseListener(this, responseList);
}

MCACCEQDialog::~MCACCEQDialog()
{
    delete ui;
}

void MCACCEQDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("MCACCEQWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}

void MCACCEQDialog::ShowMCACCEQDialog()
{
    if (!isVisible())
    {
        DisableControls();
        ui->ImmediatelyCheckBox->setChecked(false);
        ui->SetDistancePushButton->setDisabled(false);
        if (!m_PositionSet || !m_Settings.value("SaveMCACCEQWindowGeometry", false).toBool())
        {
            QWidget* Parent = dynamic_cast<QWidget*>(parent());
            int x = Parent->pos().x() + Parent->width() + 20;
            //int x = Parent->pos().x() - Parent->width() - 100;
            QPoint pos;
            pos.setX(x);
            pos.setY(Parent->pos().y());
            this->move(pos);
        }
        this->show();
        emit SendCmd("?MC"); // MCACC MEMORY SET
    }
    else
    {
        this->hide();
    }

}

void MCACCEQDialog::ResponseReceived(ReceivedObjectBase *response)
{
    if (!isVisible())
        return;
    // mcacc number
    MCACCNumberResponse_MC* mcacc = dynamic_cast<MCACCNumberResponse_MC*>(response);
    if (mcacc != NULL)
    {
        ui->groupBoxMCACC->setEnabled(true);
        int no = mcacc->GetMCACCNumber();
        if (m_CurrentMcacc == no)
            return;
        m_CurrentMcacc = no;
        no--;
        if (no < 0 && no >= m_MCACCButtons.size())
            return;
        m_MCACCButtons[no]->blockSignals(true);
        m_MCACCButtons[no]->setChecked(true);
        m_MCACCButtons[no]->blockSignals(false);

        DisableControls();

        for (int i = 0; i < m_Channels.size(); i++)
        {
            // ask for eq data
            for(int j = 0; j < m_Channels[i].eqData.size(); j++) {
                m_Channels[i].eqData[j].enabled = false;
                m_Channels[i].eqData[j].value = 50;
            }
            RefreshSpeakerEq(m_Channels[i].channel);
            // ask the distance data
            SendCmd("?SSS" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + m_Channels[i].channel);
        }

        ui->RadioButtonSW->setEnabled(true);
        return;
    }
    MCACCEQResponse_SUW* mcacceq = dynamic_cast<MCACCEQResponse_SUW*>(response);
    if (mcacceq != NULL)
    {
        // don't react to a not selected mcacc set
        if (mcacceq->GetMCACCNo() != m_CurrentMcacc)
            return;
        int idx = -1;
        int value = mcacceq->GetEQValue();
        int eqidx = mcacceq->GetEqNo();
        // find the speaker the data is for
        for (int i = 0; i < m_Channels.size(); i++)
        {
            if (mcacceq->GetSpeakerId() == m_Channels[i].channel)
            {
                idx = i;
                break;
            }
        }
        // not found, something wrong
        if (idx == -1)
            return;
        // enable this speaker
        m_Channels[idx].radio->setEnabled(true);
        // save the value for the given frequency
        m_Channels[idx].eqData[eqidx].value = value;
        m_Channels[idx].eqData[eqidx].enabled = true;
        // show the data if the given speaker is selected
        if (m_Channels[idx].radio->isChecked())
        {
            m_Slider[eqidx]->setValue(value);
            m_Slider[eqidx]->setEnabled(true);
            m_Labels[eqidx]->setText(mcacceq->GetDBValueString());
        }

        return;
    }
    SpeakerDistanceResponse_SSS* distance = dynamic_cast<SpeakerDistanceResponse_SSS*>(response);
    if (distance != NULL)
    {
        // wrong mcacc no
        if (distance->GetMCACCNo() != m_CurrentMcacc)
            return;
        // we only handle europian models for now
        if (distance->getRawUnits() == SpeakerDistanceResponse_SSS::METER) {
            //qDebug() << "Distance" << distance->GetSpeakerId() << distance->GetValue();
            int idx = -1;
            // find the speaker
            for (int i = 0; i < m_Channels.size(); i++)
            {
                if (distance->GetSpeakerId() == m_Channels[i].channel)
                {
                    idx = i;
                    break;
                }
            }
            if (idx == -1)
                return;
            //qDebug() << "Distance" << distance->GetSpeakerId() << distance->GetValue();
            // save the distance for the given speaker
            m_Channels[idx].distance = distance->GetValue();
            // update gui if needed
            if (m_Channels[idx].radio->isChecked())
            {
                ui->groupBoxDistance->setEnabled(true);
                ui->DistanceSpinBox->setValue(m_Channels[idx].distance);
            }
        }
        return;
    }
}

void MCACCEQDialog::RefreshSpeakerEq(QString speaker)
{
    if (speaker != "SW_") {
        if (speaker == "SW1" || speaker == "SW2") {
            emit SendCmd("?SUW" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + speaker + "00");
            emit SendCmd("?SUW" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + speaker + "01");
            emit SendCmd("?SUW" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + speaker + "02");
            emit SendCmd("?SUW" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + speaker + "09");
            emit SendCmd("?SUW" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + speaker + "10");
        } else {
            for (int i = 0; i < m_Slider.size(); i++) {
                emit SendCmd("?SUW" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + speaker + QString("%1").arg(i, 2, 10, QChar('0')));
            }
        }
    }
}

void MCACCEQDialog::EnableSlider(bool enabled)
{
    for (int i = 0; i < (int)m_Slider.size(); i++)
    {
        m_Slider[i]->setEnabled(enabled);
    }
}

void MCACCEQDialog::ChannelClicked()
{
    QString id;
    QObject* sender = QObject::sender();
    id = sender->objectName();
    bool isSubwoofer = false;
    for (int i = 0; i < (int)m_Channels.size(); i++)
    {
        QString tempid = m_Channels[i].radio->objectName();
        //qDebug() << tempid << m_Channels[i].channel;
        if (id == tempid)
        {
            m_SelectedChannel = i;
            isSubwoofer = m_Channels[i].channel == "SW_";// || m_Speakers[i].first == "SW1" || m_Speakers[i].first == "SW2";
            EnableSlider(!isSubwoofer);
            if (!isSubwoofer) {
                for (int j = 0; j < (int)m_Slider.size(); j++)
                {
                    m_Slider[j]->setEnabled(m_Channels[i].eqData[j].enabled);
                    m_Slider[j]->setValue(m_Channels[i].eqData[j].value);
                    m_Labels[j]->setText(MCACCEQResponse_SUW::GetDBValueString(m_Channels[i].eqData[j].value));
                }
            } else {
                SendCmd("?SSS" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + m_Channels[i].channel);
            }
            ui->DistanceSpinBox->setEnabled(true);
            ui->DistanceSpinBox->setValue(m_Channels[i].distance);
            ui->ImmediatelyCheckBox->setEnabled(true);
            break;
        }
    }
}

void MCACCEQDialog::SliderValueChanged()
{
    int j;
    QString id;
    QObject* sender = QObject::sender();
    id = sender->objectName();
    for (int i = 0; i < (int)m_Slider.size(); i++)
    {
        QString tempid = m_Slider[i]->objectName();
        if (id == tempid)
        {
            m_Channels[m_SelectedChannel].eqData[i].value = m_Slider[i]->value();
            m_Labels[i]->setText(MCACCEQResponse_SUW::GetDBValueString(m_Channels[m_SelectedChannel].eqData[i].value));
            QString cmd = QString("00%1%2%3SUW").arg(m_Channels[m_SelectedChannel].channel).arg(i, 2, 10, QChar('0')).arg(m_Channels[m_SelectedChannel].eqData[i].value);
            SendCmd(cmd);
            j = m_SelectedChannel;
            if (ui->paar->isChecked()) // LS Paarweise gleich einstellen
            {
                if (j==0 || j==3 || j==5 || j==7 || j==9)
                {
                   cmd = QString("00%1%2%3SUW").arg(m_Channels[(m_SelectedChannel+1)].channel).arg(i, 2, 10, QChar('0')).arg(m_Channels[m_SelectedChannel].eqData[i].value);
                }
                if (j==1 || j==4 || j==6 || j==8 || j==10)
                {
                   cmd = QString("00%1%2%3SUW").arg(m_Channels[(m_SelectedChannel-1)].channel).arg(i, 2, 10, QChar('0')).arg(m_Channels[m_SelectedChannel].eqData[i].value);
                }
                SendCmd(cmd);
            }
        }
    }
}

void MCACCEQDialog::MCACC_selected()
{
    int index = 0;
    QRadioButton* sender = dynamic_cast<QRadioButton*>(QObject::sender());
    if (sender == NULL)
        return;
    DisableControls();
    for(int i = 0; i < (int)m_Slider.size(); i++)
    {
        m_Slider[i]->setValue(50);
    }
    for (int i = 0; i < (int)m_Channels.size(); i++)
    {
        m_Channels[i].radio->setEnabled(false);
    }
    for (int i = 0; i < (int)m_MCACCButtons.size(); i++)
    {
        if (sender == m_MCACCButtons[i])
            index = i;
    }

    SendCmd(QString("%1MC").arg(index + 1));
}

void MCACCEQDialog::on_SaveToFilePushButton_clicked()
{
    int mcacc = 1;
    for (int i = 0; i < m_MCACCButtons.size(); i++)
    {
        if (m_MCACCButtons[i]->isChecked())
        {
            mcacc = i + 1;
            break;
        }
    }
    QString msg = tr("Beware: only the current MCACC memory (No %1) will be saved!").arg(mcacc);
    QMessageBox::warning(this, tr("Save to file"), msg);

    QString filename = QFileDialog::getSaveFileName(this, tr("Restore from..."),
                                              QString(), tr("Settings file (*.ini)"));
    if (filename.isEmpty())
        return;
    if (!(filename.endsWith(".ini", Qt::CaseInsensitive)))
        filename += ".ini";

    QSettings settings(filename, QSettings::IniFormat);
    settings.setValue("MCACC", mcacc);
    for (int i = 0; i < (int)m_Channels.size(); i++)
    {
        for( int j = 0; j < (int)m_Slider.size(); j++)
            settings.setValue(QString("CHANNEL%1_EQ%2").arg(i).arg(j), m_Channels[i].eqData[j].value);
    }
    for(int i = 0; i < m_Channels.size(); i++)
    {
        settings.setValue(QString("DISTANCE%1").arg(i), m_Channels[i].distance);
    }
}

void MCACCEQDialog::on_RestoreFromFilePushButton_clicked()
{
    int mcacc = 1;
    for (int i = 0; i < m_MCACCButtons.size(); i++)
    {
        if (m_MCACCButtons[i]->isChecked())
        {
            mcacc = i + 1;
            break;
        }
    }
    QString msg = tr("Beware: the current MCACC memory (No %1) will be overwritten!").arg(mcacc);
    int result = QMessageBox::question(this, tr("Restore from file"), msg, tr("Continue"), tr("Cancel"));

    if (result != 0)
        return;
    QString filename = QFileDialog::getOpenFileName(this, tr("Restore from..."),
                                              QString(), tr("Settings file (*.ini)"));
    if (filename.isEmpty())
        return;
    if (!(filename.endsWith(".ini", Qt::CaseInsensitive)))
        filename += ".ini";
    QSettings settings(filename, QSettings::IniFormat);
    for (int i = 0; i < (int)m_Channels.size(); i++)
    {
        for( int j = 0; j < (int)m_Slider.size(); j++)
        {
            m_Channels[i].eqData[j].value = settings.value(QString("CHANNEL%1_EQ%2").arg(i).arg(j), 50).toInt();
            QString cmd = QString("00%1%2%3SUW").arg(m_Channels[i].channel).arg(j, 2, 10, QChar('0')).arg(m_Channels[i].eqData[j].value);
            SendCmd(cmd);
        }
        //QThread::usleep(200);
    }
    for(int i = 0; i < m_Channels.size(); i++)
    {
        m_Channels[i].distance = settings.value(QString("DISTANCE%1").arg(i), 0.0).toDouble();
        int distance = (int)(m_Channels[i].distance * 100.0 + 0.5);
        QString cmd = QString("00%1%2%3SSS").arg(m_Channels[i].channel).arg("1").arg(distance, 6, 10, QChar('0'));
        SendCmd(cmd);
    }
}

void MCACCEQDialog::on_ImmediatelyCheckBox_stateChanged(int state)
{
    if (state == Qt::Unchecked) {
        ui->SetDistancePushButton->setEnabled(true);
    } else {
        ui->SetDistancePushButton->setEnabled(false);
    }
}

void MCACCEQDialog::on_DistanceSpinBox_valueChanged(double /*value*/)
{
    if (ui->ImmediatelyCheckBox->checkState() == Qt::Checked) {
        SendDistance();
    }
}

void MCACCEQDialog::on_SetDistancePushButton_clicked()
{
    SendDistance();
}

void MCACCEQDialog::SendDistance()
{
    int distance = (int)(ui->DistanceSpinBox->value() * 100.0 + 0.5);
    QString cmd = QString("00%1%2%3SSS").arg(m_Channels[m_SelectedChannel].channel).arg("1").arg(distance, 6, 10, QChar('0'));
    SendCmd(cmd);
}

void MCACCEQDialog::DisableControls()
{
    EnableSlider(false);
    ui->groupBoxDistance->setDisabled(true);
}

void MCACCEQDialog::addChannel(const QString& channel, QRadioButton* radio)
{
    MCACCEQDialog::ChannelData data;
    data.channel = channel;
    data.radio = radio;
    data.distance = 0;
    data.eqData.resize(m_Slider.size());

    m_Channels.append(data);
}
