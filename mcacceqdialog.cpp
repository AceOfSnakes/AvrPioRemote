#include "mcacceqdialog.h"
#include "ui_mcacceqdialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QThread>

const char* eqchannels[]={
    "L__",
    "R__",
    "C__",
    "SL_",
    "SR_",
    "SBL",
    "SBR",
    "LH_",
    "RH_",
    "LW_",
    "RW_",
};

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

    this->setFixedSize(this->size());

    // restore the position of the window
    if (m_Settings.value("SaveMCACCEQWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("MCACCEQWindowGeometry").toByteArray());
    }

    ui->MCACCcomboBox->setEnabled(false);

    m_Speakers.append(ui->RadioButtonFrontLeftCh);
    m_Speakers.append(ui->RadioButtonFrontRightCh);
    m_Speakers.append(ui->RadioButtonCenterCh);
    m_Speakers.append(ui->RadioButtonSurroundLeftCh);
    m_Speakers.append(ui->RadioButtonSurroundRightCh);
    m_Speakers.append(ui->RadioButtonSurroundBackLeftCh);
    m_Speakers.append(ui->RadioButtonSurroundBackRightCh);
    m_Speakers.append(ui->RadioButtonFrontHeightLeftCh);
    m_Speakers.append(ui->RadioButtonFrontHeightRightCh);
    m_Speakers.append(ui->RadioButtonFrontWideLeftCh);
    m_Speakers.append(ui->RadioButtonFrontWideRightCh);

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

    m_EQData.resize(m_Speakers.size());
    for (int i = 0; i < (int)m_EQData.size(); i++)
    {
        m_EQData[i].resize(m_Slider.size());
        m_Speakers[i]->setEnabled(false);
        connect(m_Speakers[i],SIGNAL(clicked()),this,SLOT(SpeakerClicked()));
        for (int j = 0; j < (int)m_EQData[i].size(); j++)
        {
            m_EQData[i][j] = 50;
        }
    }

    for(int i = 0; i < (int)m_Slider.size(); i++)
    {
        m_Slider[i]->setEnabled(false);
        m_Slider[i]->setValue(50);
    }

    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));

    for (int i = 0; i < (int)m_Slider.size(); i++)
    {
        connect(m_Slider[i],SIGNAL(sliderReleased()),this,SLOT(SliderValueChanged()));
    }

    m_Distance.resize(m_Speakers.size());
    ui->DistanceSpinBox->setValue(0);

    QStringList responseList;
    responseList << MCACCNumberResponse().getResponseID();
    responseList << MCACCEQResponse().getResponseID();
    responseList << SpeakerDistanceResponse().getResponseID();
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
        if (!m_PositionSet || !m_Settings.value("SaveMCACCEQWindowGeometry", false).toBool())
        {
            QWidget* Parent = dynamic_cast<QWidget*>(parent());
            int x = Parent->pos().x() + Parent->width() + 20;
            //int x = Parent->pos().x() - Parent->width() - 100;
            QPoint pos;
            pos.setX(x);
            pos.setY(Parent->pos().y());
            this->move(pos);
            //qDebug() << "SET POSITION";
        }
        this->show();
        emit SendCmd("?MC"); // MCACC MEMORY SET

/*         if (!m_Settings.value("TunerCompatibilityMode").toBool())
         {
         }*/
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
    MCACCNumberResponse* mcacc = dynamic_cast<MCACCNumberResponse*>(response);
    if (mcacc != NULL)
    {
        int no = mcacc->GetMCACCNumber();
        if (m_CurrentMcacc == no)
            return;
        m_CurrentMcacc = no;
        ui->MCACCcomboBox->setEnabled(true);
        ui->MCACCcomboBox->setCurrentIndex(no - 1);

        EnableSlider(false);
        ui->DistanceSpinBox->setEnabled(false);
        ui->ImmediatelyCheckBox->setEnabled(false);

        for (int i = 0; i < m_Speakers.size(); i++)
        {
            RefreshSpeakerEq(eqchannels[i]);
            SendCmd("?SSS" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + eqchannels[i]);
        }
        m_Speakers[0]->setChecked(true);

        return;
    }
    MCACCEQResponse* mcacceq = dynamic_cast<MCACCEQResponse*>(response);
    if (mcacceq != NULL)
    {
        if (mcacceq->GetMCACCNo() != m_CurrentMcacc)
            return;
        int idx = -1;
        int value = mcacceq->GetEQValue();
        int eqidx = mcacceq->GetEqNo();
        for (int i = 0; i < m_Speakers.size(); i++)
        {
            if (mcacceq->GetSpeakerId() == eqchannels[i])
            {
                idx = i;
                break;
            }
        }
        if (idx == -1)
            return;
        m_Speakers[idx]->setEnabled(true);
        m_EQData[idx][eqidx] = value;
        if (m_Speakers[idx]->isChecked())
        {
            m_Slider[eqidx]->setValue(value);
            m_Slider[eqidx]->setEnabled(true);
            m_Labels[eqidx]->setText(mcacceq->GetDBValueString());
        }

        return;
    }
    SpeakerDistanceResponse* distance = dynamic_cast<SpeakerDistanceResponse*>(response);
    if (distance != NULL)
    {
        if (distance->GetMCACCNo() != m_CurrentMcacc)
            return;
        if (distance->getRawUnits() == SpeakerDistanceResponse::METER) {
            int idx = -1;
            for (int i = 0; i < m_Speakers.size(); i++)
            {
                if (distance->GetSpeakerId() == eqchannels[i])
                {
                    idx = i;
                    break;
                }
            }
            if (idx == -1)
                return;
            m_Distance[idx] = distance->GetValue();
            if (m_Speakers[idx]->isChecked())
            {
                ui->DistanceSpinBox->setEnabled(true);
                ui->DistanceSpinBox->setValue(distance->GetValue());
                ui->ImmediatelyCheckBox->setEnabled(true);
            }
        }
        return;
    }
}

void MCACCEQDialog::RefreshSpeakerEq(QString speaker)
{
    for (int i = 0; i < m_Slider.size(); i++)
        emit SendCmd("?SUW" + QString("%1").arg(m_CurrentMcacc, 2, 10, QChar('0')) + speaker + "0" + QString("%1").arg(i));
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
    for (int i = 0; i < (int)m_Speakers.size(); i++)
    {
        QString tempid = m_Speakers[i]->objectName();
        if (id == tempid)
        {
            m_SelectedChannel = i;
            EnableSlider(true);
            for (int j = 0; j < (int)m_Slider.size(); j++)
            {
                m_Slider[j]->setValue(m_EQData[i][j]);
                m_Labels[j]->setText(MCACCEQResponse::GetDBValueString(m_EQData[i][j]));
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
            m_Labels[i]->setText(MCACCEQResponse::GetDBValueString(m_EQData[m_SelectedChannel][i]));
            QString cmd = QString("00%1%2%3SUW").arg(eqchannels[m_SelectedChannel]).arg(i, 2, 10, QChar('0')).arg(m_EQData[m_SelectedChannel][i]);
            SendCmd(cmd);
            j=m_SelectedChannel;
            if (ui->paar->isChecked()) // LS Paarweise gleich einstellen
            {
                if (j==0 || j==3 || j==5 || j==7 || j==9)
                {
                   cmd = QString("00%1%2%3SUW").arg(eqchannels[(m_SelectedChannel+1)]).arg(i, 2, 10, QChar('0')).arg(m_EQData[m_SelectedChannel][i]);
                }
                if (j==1 || j==4 || j==6 || j==8 || j==10)
                {
                   cmd = QString("00%1%2%3SUW").arg(eqchannels[(m_SelectedChannel-1)]).arg(i, 2, 10, QChar('0')).arg(m_EQData[m_SelectedChannel][i]);
                }
                SendCmd(cmd);
            }
        }
    }
}

void MCACCEQDialog::on_MCACCcomboBox_activated(int index)
{
    EnableSlider(false);
    for(int i = 0; i < (int)m_Slider.size(); i++)
    {
        m_Slider[i]->setValue(50);
    }
    for (int i = 0; i < (int)m_Speakers.size(); i++)
    {
        m_Speakers[i]->setEnabled(false);
    }
    SendCmd(QString("%1MC").arg(index + 1));
}

void MCACCEQDialog::on_SaveToFilePushButton_clicked()
{
    int mcacc = ui->MCACCcomboBox->currentIndex() + 1;
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
    int mcacc = ui->MCACCcomboBox->currentIndex() + 1;
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
            QString cmd = QString("00%1%2%3SUW").arg(eqchannels[i]).arg(j, 2, 10, QChar('0')).arg(m_EQData[i][j]);
            SendCmd(cmd);
        }
        //QThread::usleep(200);
    }
    for(int i = 0; i < m_Distance.size(); i++)
    {
        m_Distance[i] = settings.value(QString("DISTANCE%1").arg(i), 0.0).toDouble();
        int distance = (int)(m_Distance[i] * 100.0 + 0.5);
        QString cmd = QString("00%1%2%3SSS").arg(eqchannels[i]).arg("1").arg(distance, 6, 10, QChar('0'));
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
    QString cmd = QString("00%1%2%3SSS").arg(eqchannels[m_SelectedChannel]).arg("1").arg(distance, 6, 10, QChar('0'));
    SendCmd(cmd);
}
