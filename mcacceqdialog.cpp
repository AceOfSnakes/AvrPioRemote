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

    m_Speakers.append(QPair<QString, QRadioButton*>("L__", ui->RadioButtonFrontLeftCh));
    m_Speakers.append(QPair<QString, QRadioButton*>("R__", ui->RadioButtonFrontRightCh));
    m_Speakers.append(QPair<QString, QRadioButton*>("C__", ui->RadioButtonCenterCh));
    m_Speakers.append(QPair<QString, QRadioButton*>("SL_", ui->RadioButtonSurroundLeftCh));
    m_Speakers.append(QPair<QString, QRadioButton*>("SR_", ui->RadioButtonSurroundRightCh));
    m_Speakers.append(QPair<QString, QRadioButton*>("SBL", ui->RadioButtonSurroundBackLeftCh));
    m_Speakers.append(QPair<QString, QRadioButton*>("SBR", ui->RadioButtonSurroundBackRightCh));
    m_Speakers.append(QPair<QString, QRadioButton*>("LH_", ui->RadioButtonFrontHeightLeftCh));
    m_Speakers.append(QPair<QString, QRadioButton*>("RH_", ui->RadioButtonFrontHeightRightCh));
    m_Speakers.append(QPair<QString, QRadioButton*>("LW_", ui->RadioButtonFrontWideLeftCh));
    m_Speakers.append(QPair<QString, QRadioButton*>("RW_", ui->RadioButtonFrontWideRightCh));
    m_Speakers.append(QPair<QString, QRadioButton*>("SW_", ui->RadioButtonSW));

    m_Slider.append(ui->Slider63Hz);
    m_Slider.append(ui->Slider125Hz);
    m_Slider.append(ui->Slider250Hz);
    m_Slider.append(ui->Slider500Hz);
    m_Slider.append(ui->Slider1kHz);
    m_Slider.append(ui->Slider2kHz);
    m_Slider.append(ui->Slider4kHz);
    m_Slider.append(ui->Slider8kHz);
    m_Slider.append(ui->Slider16kHz);

    m_Labels.append(ui->Label63Hz);
    m_Labels.append(ui->Label125Hz);
    m_Labels.append(ui->Label250Hz);
    m_Labels.append(ui->Label500Hz);
    m_Labels.append(ui->Label1kHz);
    m_Labels.append(ui->Label2kHz);
    m_Labels.append(ui->Label4kHz);
    m_Labels.append(ui->Label8kHz);
    m_Labels.append(ui->Label16kHz);

    for(int i = 0; i < (int)m_Slider.size(); i++)
    {
        m_Slider[i]->setEnabled(false);
        m_Slider[i]->setValue(50);
    }

    for (int i = 0; i < (int)m_Slider.size(); i++)
    {
        connect(m_Slider[i],SIGNAL(sliderReleased()),this,SLOT(SliderValueChanged()));
    }

    m_EQData.resize(m_Speakers.size());
    m_Speakers[0].second->setChecked(true);
    for (int i = 0; i < (int)m_EQData.size(); i++)
    {
        m_EQData[i].resize(m_Slider.size());
        m_Speakers[i].second->setEnabled(false);
        connect(m_Speakers[i].second, SIGNAL(clicked()) ,this, SLOT(SpeakerClicked()));
        for (int j = 0; j < (int)m_EQData[i].size(); j++)
        {
            m_EQData[i][j] = 50;
        }
    }

    m_Distance.resize(m_Speakers.size());
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

        for (int i = 0; i < m_Speakers.size(); i++)
        {
            // ask for eq data
            RefreshSpeakerEq(m_Speakers[i].first);
            // ask the distance data
            SendCmd("?SSS" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + m_Speakers[i].first);
            //qDebug() << "Distance" << "?SSS" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + m_Speakers[i].first;
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
        for (int i = 0; i < m_Speakers.size(); i++)
        {
            if (mcacceq->GetSpeakerId() == m_Speakers[i].first)
            {
                idx = i;
                break;
            }
        }
        // not found, something wrong
        if (idx == -1)
            return;
        // enable this speaker
        m_Speakers[idx].second->setEnabled(true);
        // save the value for the given frequency
        m_EQData[idx][eqidx] = value;
        // show the data if the given speaker is selected
        if (m_Speakers[idx].second->isChecked())
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
            for (int i = 0; i < m_Speakers.size(); i++)
            {
                if (distance->GetSpeakerId() == m_Speakers[i].first)
                {
                    idx = i;
                    break;
                }
            }
            if (idx == -1)
                return;
            //qDebug() << "Distance" << distance->GetSpeakerId() << distance->GetValue();
            // save the distance for the given speaker
            m_Distance[idx] = distance->GetValue();
            // update gui if needed
            if (m_Speakers[idx].second->isChecked())
            {
                ui->groupBoxDistance->setEnabled(true);
                ui->DistanceSpinBox->setValue(m_Distance[idx]);
            }
        }
        return;
    }
}

void MCACCEQDialog::RefreshSpeakerEq(QString speaker)
{
    if (speaker != "SW_") {
        for (int i = 0; i < m_Slider.size(); i++) {
            emit SendCmd("?SUW" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + speaker + "0" + QString("%1").arg(i));
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

void MCACCEQDialog::SpeakerClicked()
{
    QString id;
    QObject* sender = QObject::sender();
    id = sender->objectName();
    bool isSubwoofer = false;
    for (int i = 0; i < (int)m_Speakers.size(); i++)
    {
        QString tempid = m_Speakers[i].second->objectName();
        qDebug() << tempid << m_Speakers[i].first;
        if (id == tempid)
        {
            m_SelectedChannel = i;
            isSubwoofer = m_Speakers[i].first == "SW_";
            EnableSlider(!isSubwoofer);
            if (!isSubwoofer) {
                for (int j = 0; j < (int)m_Slider.size(); j++)
                {
                    m_Slider[j]->setValue(m_EQData[i][j]);
                    m_Labels[j]->setText(MCACCEQResponse_SUW::GetDBValueString(m_EQData[i][j]));
                }
            } else {
                SendCmd("?SSS" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + "SW_");
            }
            ui->DistanceSpinBox->setEnabled(true);
            ui->DistanceSpinBox->setValue(m_Distance[i]);
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
            m_EQData[m_SelectedChannel][i] = m_Slider[i]->value();
            m_Labels[i]->setText(MCACCEQResponse_SUW::GetDBValueString(m_EQData[m_SelectedChannel][i]));
            QString cmd = QString("00%1%2%3SUW").arg(m_Speakers[m_SelectedChannel].first).arg(i, 2, 10, QChar('0')).arg(m_EQData[m_SelectedChannel][i]);
            SendCmd(cmd);
            j = m_SelectedChannel;
            if (ui->paar->isChecked()) // LS Paarweise gleich einstellen
            {
                if (j==0 || j==3 || j==5 || j==7 || j==9)
                {
                   cmd = QString("00%1%2%3SUW").arg(m_Speakers[(m_SelectedChannel+1)].first).arg(i, 2, 10, QChar('0')).arg(m_EQData[m_SelectedChannel][i]);
                }
                if (j==1 || j==4 || j==6 || j==8 || j==10)
                {
                   cmd = QString("00%1%2%3SUW").arg(m_Speakers[(m_SelectedChannel-1)].first).arg(i, 2, 10, QChar('0')).arg(m_EQData[m_SelectedChannel][i]);
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
    for (int i = 0; i < (int)m_Speakers.size(); i++)
    {
        m_Speakers[i].second->setEnabled(false);
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
    for (int i = 0; i < (int)m_EQData.size(); i++)
    {
        for( int j = 0; j < (int)m_EQData[i].size(); j++)
            settings.setValue(QString("CHANNEL%1_EQ%2").arg(i).arg(j), m_EQData[i][j]);
    }
    for(int i = 0; i < m_Distance.size(); i++)
    {
        settings.setValue(QString("DISTANCE%1").arg(i), m_Distance[i]);
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
    for (int i = 0; i < (int)m_EQData.size(); i++)
    {
        for( int j = 0; j < (int)m_EQData[i].size(); j++)
        {
            m_EQData[i][j] = settings.value(QString("CHANNEL%1_EQ%2").arg(i).arg(j), 50).toInt();
            QString cmd = QString("00%1%2%3SUW").arg(m_Speakers[i].first).arg(j, 2, 10, QChar('0')).arg(m_EQData[i][j]);
            SendCmd(cmd);
        }
        //QThread::usleep(200);
    }
    for(int i = 0; i < m_Distance.size(); i++)
    {
        m_Distance[i] = settings.value(QString("DISTANCE%1").arg(i), 0.0).toDouble();
        int distance = (int)(m_Distance[i] * 100.0 + 0.5);
        QString cmd = QString("00%1%2%3SSS").arg(m_Speakers[i].first).arg("1").arg(distance, 6, 10, QChar('0'));
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
    QString cmd = QString("00%1%2%3SSS").arg(m_Speakers[m_SelectedChannel].first).arg("1").arg(distance, 6, 10, QChar('0'));
    SendCmd(cmd);
}

void MCACCEQDialog::DisableControls()
{
    EnableSlider(false);
    ui->groupBoxDistance->setDisabled(true);
}
