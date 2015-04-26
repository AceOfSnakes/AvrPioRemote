#include "avsettingsdialog.h"
#include "ui_avsettingsdialog.h"

AVSettingsDialog::AVSettingsDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm) :
    QDialog(parent),
    ui(new Ui::AVSettingsDialog),
    m_Settings(settings),
    m_Comm(Comm),
    m_PositionSet(false),
    m_CmdRepeatCounter(0),
    m_CmdRepeatTimer(this)
{
    ui->setupUi(this);

    // restore the position of the window
    if (m_Settings.value("SaveAVSettingsWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("AVSettingsWindowGeometry").toByteArray());
    }

    connect(this,     SIGNAL(SendCmdSignal(QString)), (&m_Comm), SLOT(SendCmd(QString)));
    connect((&m_Comm), SIGNAL(DataReceived(QString)), this,      SLOT(NewDataReceived(QString)));

    ui->ResolutionComboBox->addItem("AUTO",     "00");
    ui->ResolutionComboBox->addItem("PURE",     "01");
    ui->ResolutionComboBox->addItem("480/576p", "03");
    ui->ResolutionComboBox->addItem("720p",     "04");
    ui->ResolutionComboBox->addItem("1080i",    "05");
    ui->ResolutionComboBox->addItem("1080p",    "06");
    ui->ResolutionComboBox->addItem("1080/24p", "07");
    ui->ResolutionComboBox->setCurrentIndex(-1);

    ui->StreamSmootherComboBox->addItem("OFF",  "0");
    ui->StreamSmootherComboBox->addItem("ON",   "1");
    ui->StreamSmootherComboBox->addItem("AUTO", "2");
    ui->StreamSmootherComboBox->setCurrentIndex(-1);

    ui->PureCinemaComboBox->addItem("AUTO",     "0");
    ui->PureCinemaComboBox->addItem("ON",       "1");
    ui->PureCinemaComboBox->addItem("OFF",      "2");
    ui->PureCinemaComboBox->setCurrentIndex(-1);

    ui->AdvancedVideoAdjustComboBox->addItem("PDP",             "0");
    ui->AdvancedVideoAdjustComboBox->addItem("LCD",             "1");
    ui->AdvancedVideoAdjustComboBox->addItem("Front Projector", "2");
    ui->AdvancedVideoAdjustComboBox->addItem("Professional",    "3");
    ui->AdvancedVideoAdjustComboBox->addItem(tr("Custom"),      "4");
    ui->AdvancedVideoAdjustComboBox->setCurrentIndex(-1);

    ui->BlackSetupComboBox->addItem("0",               "0");
    ui->BlackSetupComboBox->addItem("7.5",             "1");
    ui->BlackSetupComboBox->setCurrentIndex(-1);

    ui->AspectComboBox->addItem("THROUGH",             "0");
    ui->AspectComboBox->addItem("NORMAL",              "1");
    ui->AspectComboBox->setCurrentIndex(-1);

    ui->MemSetNoComboBox->addItem("Memory 1",          "1");
    ui->MemSetNoComboBox->addItem("Memory 2",          "2");
    ui->MemSetNoComboBox->addItem("Memory 3",          "3");
    ui->MemSetNoComboBox->addItem("Memory 4",          "4");
    ui->MemSetNoComboBox->addItem("Memory 5",          "5");
    ui->MemSetNoComboBox->addItem("Memory 6",          "6");
    ui->MemSetNoComboBox->setCurrentIndex(-1);

    ui->PhaseControlComboBox->addItem("Phase Control OFF",          "0");
    ui->PhaseControlComboBox->addItem("Phase Control ON",           "1");
    ui->PhaseControlComboBox->addItem("Full Band Phase Control ON", "2");
    ui->PhaseControlComboBox->setCurrentIndex(-1);

    ui->SignalSelectComboBox->addItem("AUTO",       "0");
    ui->SignalSelectComboBox->addItem("ANALOG",     "1");
    ui->SignalSelectComboBox->addItem("DIGITAL",    "2");
    ui->SignalSelectComboBox->addItem("HDMI",       "3");
    ui->SignalSelectComboBox->setCurrentIndex(-1);

    ui->DialogEnhancementComboBox->addItem("OFF",   "0");
    ui->DialogEnhancementComboBox->addItem("FLAT",  "1");
    ui->DialogEnhancementComboBox->addItem("UP1",   "2");
    ui->DialogEnhancementComboBox->addItem("UP2",   "3");
    ui->DialogEnhancementComboBox->addItem("UP3",   "4");
    ui->DialogEnhancementComboBox->addItem("UP4",   "5");
    ui->DialogEnhancementComboBox->setCurrentIndex(-1);

    ui->DualMonoComboBox->addItem("CH1 & CH2", "0");
    ui->DualMonoComboBox->addItem("CH1",       "1");
    ui->DualMonoComboBox->addItem("CH2",       "2");
    ui->DualMonoComboBox->setCurrentIndex(-1);

    ui->DRCComboBox->addItem("OFF",       "0");
    ui->DRCComboBox->addItem("AUTO",      "1");
    ui->DRCComboBox->addItem("MID",       "2");
    ui->DRCComboBox->addItem("MAX",       "3");
    ui->DRCComboBox->setCurrentIndex(-1);

    ui->LFEATTComboBox->addItem("OFF",      "5");
    ui->LFEATTComboBox->addItem("0dB",      "0");
    ui->LFEATTComboBox->addItem("-5dB",     "1");
    ui->LFEATTComboBox->addItem("-10dB",    "2");
    ui->LFEATTComboBox->addItem("-15dB",    "3");
    ui->LFEATTComboBox->addItem("-20dB",    "4");
    ui->LFEATTComboBox->setCurrentIndex(-1);

    ui->HDMIOutputComboBox->addItem("HDMI OUT ALL", "0");
    ui->HDMIOutputComboBox->addItem("HDMI 1",       "1");
    ui->HDMIOutputComboBox->addItem("HDMI 2",       "2");
    ui->HDMIOutputComboBox->setCurrentIndex(-1);

    ui->HDMIAudioComboBox->addItem("AMP",       "0");
    ui->HDMIAudioComboBox->addItem("THROUGH",   "1");
    ui->HDMIAudioComboBox->setCurrentIndex(-1);

    ui->PQLSComboBox->addItem("OFF",    "0");
    ui->PQLSComboBox->addItem("AUTO",   "1");
    ui->PQLSComboBox->setCurrentIndex(-1);

    ui->DimmerComboBox->addItem("Brightness",   "0");
    ui->DimmerComboBox->addItem("Bright",       "1");
    ui->DimmerComboBox->addItem("Dark",         "2");
    ui->DimmerComboBox->addItem("OFF",          "3");
    ui->DimmerComboBox->setCurrentIndex(-1);

    ui->SleepComboBox->addItem("OFF",           "000");
    ui->SleepComboBox->addItem("30min",         "030");
    ui->SleepComboBox->addItem("60min",         "060");
    ui->SleepComboBox->addItem("90min",         "090");
    ui->SleepComboBox->setCurrentIndex(-1);

    connect((&m_CmdRepeatTimer), SIGNAL(timeout()), this, SLOT(CmdRepeatTimeout()));
    m_CmdRepeatTimer.setSingleShot(true);
    m_CmdRepeatTimer.setInterval(1000);

    QStringList responseList;
    responseList << InputFunctionResponse_FN().getResponseID();
    responseList << MCACCNumberResponse_MC().getResponseID();
    responseList << ErrorResponse_B_E().getResponseID();
    MsgDistributor::AddResponseListener(this, responseList);
}

AVSettingsDialog::~AVSettingsDialog()
{
    delete ui;
}

void AVSettingsDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("AVSettingsWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}

void AVSettingsDialog::SendCmd(QString cmd)
{
    emit SendCmdSignal(cmd);
    m_LastCmd = cmd;
}

void AVSettingsDialog::CmdRepeatTimeout()
{
    //m_CmdRepeatTimer.stop();
    if (m_LastCmd != "")
    emit SendCmdSignal(m_LastCmd);
    m_LastCmd = "";
}

void AVSettingsDialog::EnableAVControls(bool enable)
{
    ui->CNRSlider->setEnabled(enable);
    ui->BNRSlider->setEnabled(enable);
    ui->MNRSlider->setEnabled(enable);
    ui->YNRSlider->setEnabled(enable);
    ui->DetailSlider->setEnabled(enable);
    ui->SharpnessSlider->setEnabled(enable);
    ui->BrightnessSlider->setEnabled(enable);
    ui->HueSlider->setEnabled(enable);
    ui->ContrastSlider->setEnabled(enable);
    ui->ChromaLevelSlider->setEnabled(enable);
}

void AVSettingsDialog::ResponseReceived(ReceivedObjectBase *response)
{
    // input
    InputFunctionResponse_FN* inputFunction = dynamic_cast<InputFunctionResponse_FN*>(response);
    if (inputFunction != NULL)
    {
        if (!this->isVisible())
            return;
        Refresh();
        return;
    }
    // mcacc memory number
    MCACCNumberResponse_MC* mcacc = dynamic_cast<MCACCNumberResponse_MC*>(response);
    if (mcacc != NULL)
    {
        //QString no = data.mid(2);
        //int idx = ui->MemSetNoComboBox->findData(no);
        //qDebug() << "mcacc idx:" << mcacc->GetMCACCNumber();
        ui->MemSetNoComboBox->setCurrentIndex(mcacc->GetMCACCNumber() - 1);
        return;
    }
    ErrorResponse_B_E* error = dynamic_cast<ErrorResponse_B_E*>(response);
    if (error != NULL)
    {
        if (error->GetError() == ErrorResponse_B_E::ErrorDoesntWorkNow || error->GetError() == ErrorResponse_B_E::ErrorBusy)
        {
            m_CmdRepeatTimer.start();
        }
        return;
    }
}

void AVSettingsDialog::ShowAVSettingsDialog()
{
    if (!isVisible())
    {
        if (!m_PositionSet || !m_Settings.value("SaveAVSettingsWindowGeometry", false).toBool())
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
        Refresh();
    }
    else
    {
        this->hide();
    }

}

void AVSettingsDialog::Refresh()
{
    if (!this->isVisible())
        return;
    SendCmd("?VTB"); // V.CONV
    SendCmd("?VTC"); // RESOLUTION
    SendCmd("?VTD"); // PURE CINEMA
    SendCmd("?VTE"); // PROG. MOTION
    SendCmd("?VTF"); // STREAM SMOOTHER
    SendCmd("?VTG"); // ADV. VIDEO ADJUST
//    SendCmd("?VTH"); // YNR
//    SendCmd("?VTI"); // CNR
//    SendCmd("?VTJ"); // BNR
//    SendCmd("?VTK"); // MNR
//    SendCmd("?VTL"); // DETAIL
//    SendCmd("?VTM"); // SHARPNESS
//    SendCmd("?VTN"); // BRIGHTNESS
//    SendCmd("?VTO"); // CONTRAST
//    SendCmd("?VTQ"); // CHROMA LEVEL
    SendCmd("?VTR"); // BLACK SETUP
    SendCmd("?VTS"); // ASPECT

    SendCmd("?MC");  // MCACC MEMORY SET
    SendCmd("?IS");  // PHASE CONTROL
    SendCmd("?VSB"); // VIRTUAL SB
    SendCmd("?VHT"); // VIRTUAL HEIGHT
    SendCmd("?ATA"); // SOUND RETRIEVER
    SendCmd("?SDA"); // SIGNAL SELECT
    SendCmd("?SDB"); // ANALOG INPUT ATT
    SendCmd("?ATC"); // MCACC EQ
    SendCmd("?ATD"); // STANDING WAVE
    SendCmd("?ATF"); // SOUND DELAY
    SendCmd("?ATG"); // DIGITAL NOISE REDUCTION
    SendCmd("?ATH"); // DIALOG ENHANCEMENT
    SendCmd("?ATI"); // HI BIT
    SendCmd("?ATJ"); // DUAL MONO
    SendCmd("?ATK"); // FIXED PCM
    SendCmd("?VTS"); // ASPECT
    SendCmd("?ATL"); // DRC
    SendCmd("?ATM"); // LFE ATT
    SendCmd("?ATN"); // SACD GAIN
    SendCmd("?ATO"); // AUTO DELAY
    SendCmd("?ATT"); // EFFECT STRENTH
    SendCmd("?VDP"); // VIRTUAL DEPTH
    SendCmd("?ATW"); // LOUDNESS
    SendCmd("?VWD"); // VIRTUAL WIDE
    SendCmd("?HO");  // HDMI OUTPUT
    SendCmd("?HA");  // HDMI AUDIO
    SendCmd("?PQ");  // PQLS
    SendCmd("?SAB"); // SLEEP
}

void AVSettingsDialog::NewDataReceived(QString data)
{
    if (!this->isVisible())
        return;
    if (data.startsWith("VTB0", Qt::CaseInsensitive))
    {
        ui->VideoConvCheckBox->setChecked(false);
    }
    else if (data.startsWith("VTB1", Qt::CaseInsensitive))
    {
        ui->VideoConvCheckBox->setChecked(true);
    }
    else if (data.startsWith("VTC", Qt::CaseInsensitive))
    {
        QString no = data.mid(3);
        int idx = ui->ResolutionComboBox->findData(no);
        ui->ResolutionComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("VTD", Qt::CaseInsensitive))
    {
        QString no = data.mid(3);
        int idx = ui->PureCinemaComboBox->findData(no);
        ui->PureCinemaComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("VTE", Qt::CaseInsensitive))
    {
        int no = data.mid(3).toInt() - 50;
        ui->ProgMotionSlider->setValue(no);
        if (no <= 0)
            ui->ProgMotionLabel->setText(QString("%1").arg(no));
        else
            ui->ProgMotionLabel->setText(QString("+%1").arg(no));
    }
    else if (data.startsWith("VTF", Qt::CaseInsensitive))
    {
        QString no = data.mid(3);
        int idx = ui->StreamSmootherComboBox->findData(no);
        ui->StreamSmootherComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("VTG", Qt::CaseInsensitive))
    {
        QString no = data.mid(3);
        int idx = ui->AdvancedVideoAdjustComboBox->findData(no);
        ui->AdvancedVideoAdjustComboBox->setCurrentIndex(idx);
        EnableAVControls(idx == 4);
        if (idx == 4)
        {
            SendCmd("?VTH"); // YNR
            SendCmd("?VTI"); // CNR
            SendCmd("?VTJ"); // BNR
            SendCmd("?VTK"); // MNR
            SendCmd("?VTL"); // DETAIL
            SendCmd("?VTM"); // SHARPNESS
            SendCmd("?VTN"); // BRIGHTNESS
            SendCmd("?VTO"); // CONTRAST
            SendCmd("?VTQ"); // CHROMA LEVEL
        }
    }
    else if (data.startsWith("VTH", Qt::CaseInsensitive))
    {
        int no = data.mid(3).toInt() - 50;
        ui->YNRSlider->setValue(no);
        if (no <= 0)
            ui->YNRLabel->setText(QString("%1").arg(no));
        else
            ui->YNRLabel->setText(QString("+%1").arg(no));
    }
    else if (data.startsWith("VTI", Qt::CaseInsensitive))
    {
        int no = data.mid(3).toInt() - 50;
        ui->CNRSlider->setValue(no);
        if (no <= 0)
            ui->CNRLabel->setText(QString("%1").arg(no));
        else
            ui->CNRLabel->setText(QString("+%1").arg(no));
    }
    else if (data.startsWith("VTJ", Qt::CaseInsensitive))
    {
        int no = data.mid(3).toInt() - 50;
        ui->BNRSlider->setValue(no);
        if (no <= 0)
            ui->BNRLabel->setText(QString("%1").arg(no));
        else
            ui->BNRLabel->setText(QString("+%1").arg(no));
    }
    else if (data.startsWith("VTK", Qt::CaseInsensitive))
    {
        int no = data.mid(3).toInt() - 50;
        ui->MNRSlider->setValue(no);
        if (no <= 0)
            ui->MNRLabel->setText(QString("%1").arg(no));
        else
            ui->MNRLabel->setText(QString("+%1").arg(no));
    }
    else if (data.startsWith("VTL", Qt::CaseInsensitive))
    {
        int no = data.mid(3).toInt() - 50;
        ui->DetailSlider->setValue(no);
        if (no <= 0)
            ui->DetailLabel->setText(QString("%1").arg(no));
        else
            ui->DetailLabel->setText(QString("+%1").arg(no));
    }
    else if (data.startsWith("VTM", Qt::CaseInsensitive))
    {
        int no = data.mid(3).toInt() - 50;
        ui->SharpnessSlider->setValue(no);
        if (no <= 0)
            ui->SharpnessLabel->setText(QString("%1").arg(no));
        else
            ui->SharpnessLabel->setText(QString("+%1").arg(no));
    }
    else if (data.startsWith("VTN", Qt::CaseInsensitive))
    {
        int no = data.mid(3).toInt() - 50;
        ui->BrightnessSlider->setValue(no);
        if (no <= 0)
            ui->BrightnessLabel->setText(QString("%1").arg(no));
        else
            ui->BrightnessLabel->setText(QString("+%1").arg(no));
    }
    else if (data.startsWith("VTO", Qt::CaseInsensitive))
    {
        int no = data.mid(3).toInt() - 50;
        ui->ContrastSlider->setValue(no);
        if (no <= 0)
            ui->ContrastLabel->setText(QString("%1").arg(no));
        else
            ui->ContrastLabel->setText(QString("+%1").arg(no));
    }
    else if (data.startsWith("VTP", Qt::CaseInsensitive))
    {
        int no = data.mid(3).toInt() - 50;
        ui->HueSlider->setValue(no);
        if (no <= 0)
            ui->HueLabel->setText(QString("%1").arg(no));
        else
            ui->HueLabel->setText(QString("+%1").arg(no));
    }
    else if (data.startsWith("VTQ", Qt::CaseInsensitive))
    {
        int no = data.mid(3).toInt() - 50;
        ui->ChromaLevelSlider->setValue(no);
        if (no <= 0)
            ui->ChromaLevelLabel->setText(QString("%1").arg(no));
        else
            ui->ChromaLevelLabel->setText(QString("+%1").arg(no));
    }
    else if (data.startsWith("VTR", Qt::CaseInsensitive))
    {
        QString no = data.mid(3);
        int idx = ui->BlackSetupComboBox->findData(no);
        ui->BlackSetupComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("VTS", Qt::CaseInsensitive))
    {
        QString no = data.mid(3);
        int idx = ui->AspectComboBox->findData(no);
        ui->AspectComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("IS", Qt::CaseInsensitive))
    {
        QString no = data.mid(2);
        int idx = ui->PhaseControlComboBox->findData(no);
        ui->PhaseControlComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("VSB", Qt::CaseInsensitive))
    {
        ui->VirtualSBCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("VHT", Qt::CaseInsensitive))
    {
        ui->VirtualHeightCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("VWD", Qt::CaseInsensitive))
    {
        ui->VirtualWideCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("VDP", Qt::CaseInsensitive))
    {
        ui->VirtualDepthCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("ATA", Qt::CaseInsensitive))
    {
        ui->SoundRetrieverCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("SDA", Qt::CaseInsensitive))
    {
        QString no = data.mid(3);
        int idx = ui->SignalSelectComboBox->findData(no);
        ui->SignalSelectComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("SDB", Qt::CaseInsensitive))
    {
        ui->AnalogInputATTCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("ATC", Qt::CaseInsensitive))
    {
        ui->MCACCEQCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("ATD", Qt::CaseInsensitive))
    {
        ui->StandingWaveCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("ATF", Qt::CaseInsensitive))
    {
        int no = data.mid(3).toInt();
        ui->SoundDelaySlider->setValue(no);
        double d = ((double)no) / 10.0;
        if (no <= 0)
            ui->SoundDelayLabel->setText("0");
        else
            ui->SoundDelayLabel->setText( QString("%1").arg(d, 3, 'f', 1));
    }
    else if (data.startsWith("ATG", Qt::CaseInsensitive))
    {
        ui->DigitalNoiseReductionCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("ATH", Qt::CaseInsensitive))
    {
        QString no = data.mid(3);
        int idx = ui->DialogEnhancementComboBox->findData(no);
        ui->DialogEnhancementComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("ATI", Qt::CaseInsensitive))
    {
        ui->HiBitCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("ATJ", Qt::CaseInsensitive))
    {
        QString no = data.mid(3);
        int idx = ui->DualMonoComboBox->findData(no);
        ui->DualMonoComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("ATK", Qt::CaseInsensitive))
    {
        ui->FixedPCMCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("ATL", Qt::CaseInsensitive))
    {
        QString no = data.mid(3);
        int idx = ui->DRCComboBox->findData(no);
        ui->DRCComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("ATM", Qt::CaseInsensitive))
    {
        QString no = data.mid(3);
        int idx = ui->LFEATTComboBox->findData(no);
        ui->LFEATTComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("ATN", Qt::CaseInsensitive))
    {
        ui->SACDGainCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("ATO", Qt::CaseInsensitive))
    {
        ui->AutoDelayCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("ATT", Qt::CaseInsensitive))
    {
        int no = data.mid(3).toInt();
        ui->EffectStrengthSlider->setValue(no);
        ui->EffectStrengthLabel->setText( QString("%1").arg(no * 10));
    }
    else if (data.startsWith("ATW", Qt::CaseInsensitive))
    {
        ui->LoudnessCheckBox->setChecked(data.mid(3) == "1");
    }
    else if (data.startsWith("HO", Qt::CaseInsensitive))
    {
        QString no = data.mid(2);
        int idx = ui->HDMIOutputComboBox->findData(no);
        ui->HDMIOutputComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("HA", Qt::CaseInsensitive))
    {
        QString no = data.mid(2);
        int idx = ui->HDMIAudioComboBox->findData(no);
        ui->HDMIAudioComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("PQ", Qt::CaseInsensitive))
    {
        QString no = data.mid(2);
        int idx = ui->PQLSComboBox->findData(no);
        ui->PQLSComboBox->setCurrentIndex(idx);
    }
    else if (data.startsWith("SAB", Qt::CaseInsensitive))
    {
        QString no = data.mid(3);
        int idx = ui->SleepComboBox->findData(no);
        ui->SleepComboBox->setCurrentIndex(idx);
    }
}

void AVSettingsDialog::on_VideoConvCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->VideoConvCheckBox->setChecked(checked);
    SendCmd((checked)?"0VTB":"1VTB");
}

void AVSettingsDialog::on_ResolutionComboBox_activated(int index)
{
    SendCmd(ui->ResolutionComboBox->itemData(index).toString() + "VTC");
    ui->ResolutionComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_PureCinemaComboBox_activated(int index)
{
    SendCmd(ui->PureCinemaComboBox->itemData(index).toString() + "VTD");
    ui->PureCinemaComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_ProgMotionSlider_valueChanged(int value)
{
    SendCmd(QString("%1VTE").arg(50 + value));
}

void AVSettingsDialog::on_StreamSmootherComboBox_activated(int index)
{
    SendCmd(ui->StreamSmootherComboBox->itemData(index).toString() + "VTF");
    ui->StreamSmootherComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_AdvancedVideoAdjustComboBox_activated(int index)
{
    SendCmd(ui->AdvancedVideoAdjustComboBox->itemData(index).toString() + "VTG");
    ui->AdvancedVideoAdjustComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_YNRSlider_valueChanged(int value)
{
    SendCmd(QString("%1VTH").arg(50 + value));
}

void AVSettingsDialog::on_CNRSlider_valueChanged(int value)
{
    SendCmd(QString("%1VTI").arg(50 + value));
}

void AVSettingsDialog::on_BNRSlider_valueChanged(int value)
{
    SendCmd(QString("%1VTJ").arg(50 + value));
}

void AVSettingsDialog::on_MNRSlider_valueChanged(int value)
{
    SendCmd(QString("%1VTK").arg(50 + value));
}

void AVSettingsDialog::on_DetailSlider_valueChanged(int value)
{
    SendCmd(QString("%1VTL").arg(50 + value));
}

void AVSettingsDialog::on_SharpnessSlider_valueChanged(int value)
{
    SendCmd(QString("%1VTM").arg(50 + value));
}

void AVSettingsDialog::on_BrightnessSlider_valueChanged(int value)
{
    SendCmd(QString("%1VTN").arg(50 + value));
}

void AVSettingsDialog::on_HueSlider_valueChanged(int value)
{
    SendCmd(QString("%1VTP").arg(50 + value));
}

void AVSettingsDialog::on_ContrastSlider_valueChanged(int value)
{
    SendCmd(QString("%1VTO").arg(50 + value));
}

void AVSettingsDialog::on_ChromaLevelSlider_valueChanged(int value)
{
    SendCmd(QString("%1VTQ").arg(50 + value));
}

void AVSettingsDialog::on_BlackSetupComboBox_activated(int index)
{
    SendCmd(ui->BlackSetupComboBox->itemData(index).toString() + "VTR");
    ui->BlackSetupComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_AspectComboBox_activated(int index)
{
    SendCmd(ui->AspectComboBox->itemData(index).toString() + "VTS");
    ui->AspectComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_MemSetNoComboBox_activated(int index)
{
    SendCmd(ui->MemSetNoComboBox->itemData(index).toString() + "MC");
    ui->MemSetNoComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_PhaseControlComboBox_activated(int index)
{
    SendCmd(ui->PhaseControlComboBox->itemData(index).toString() + "IS");
    ui->PhaseControlComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_VirtualSBCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->VirtualSBCheckBox->setChecked(checked);
    SendCmd((checked)?"0VSB":"1VSB");
}

void AVSettingsDialog::on_VirtualHeightCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->VirtualHeightCheckBox->setChecked(checked);
    SendCmd((checked)?"0VHT":"1VHT");
}

void AVSettingsDialog::on_VirtualWideCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->VirtualWideCheckBox->setChecked(checked);
    SendCmd((checked)?"0VWD":"1VWD");
}

void AVSettingsDialog::on_VirtualDepthCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->VirtualDepthCheckBox->setChecked(checked);
    SendCmd((checked)?"0VDP":"1VDP");
}

void AVSettingsDialog::on_SoundRetrieverCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->SoundRetrieverCheckBox->setChecked(checked);
    SendCmd((checked)?"0ATA":"1ATA");
}

void AVSettingsDialog::on_SignalSelectComboBox_activated(int index)
{
    SendCmd(ui->SignalSelectComboBox->itemData(index).toString() + "SDA");
    ui->SignalSelectComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_AnalogInputATTCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->AnalogInputATTCheckBox->setChecked(checked);
    SendCmd((checked)?"0SDB":"1SDB");
}

void AVSettingsDialog::on_MCACCEQCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->MCACCEQCheckBox->setChecked(checked);
    SendCmd((checked)?"0ATC":"1ATC");
}

void AVSettingsDialog::on_StandingWaveCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->StandingWaveCheckBox->setChecked(checked);
    SendCmd((checked)?"0ATD":"1ATD");
}

void AVSettingsDialog::on_SoundDelaySlider_valueChanged(int value)
{
    SendCmd(QString("%1ATF").arg(value, 3, 10, QLatin1Char('0')));
}

void AVSettingsDialog::on_DigitalNoiseReductionCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->DigitalNoiseReductionCheckBox->setChecked(checked);
    SendCmd((checked)?"0ATG":"1ATG");
}

void AVSettingsDialog::on_DialogEnhancementComboBox_activated(int index)
{
    SendCmd(ui->DialogEnhancementComboBox->itemData(index).toString() + "ATH");
    ui->DialogEnhancementComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_HiBitCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->HiBitCheckBox->setChecked(checked);
    SendCmd((checked)?"0ATI":"1ATI");
}

void AVSettingsDialog::on_DualMonoComboBox_activated(int index)
{
    SendCmd(ui->DualMonoComboBox->itemData(index).toString() + "ATJ");
    ui->DualMonoComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_FixedPCMCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->FixedPCMCheckBox->setChecked(checked);
    SendCmd((checked)?"0ATK":"1ATK");
}

void AVSettingsDialog::on_DRCComboBox_activated(int index)
{
    SendCmd(ui->DRCComboBox->itemData(index).toString() + "ATL");
    ui->DRCComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_LFEATTComboBox_activated(int index)
{
    SendCmd(ui->LFEATTComboBox->itemData(index).toString() + "ATM");
    ui->LFEATTComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_SACDGainCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->SACDGainCheckBox->setChecked(checked);
    SendCmd((checked)?"0ATN":"1ATN");
}

void AVSettingsDialog::on_AutoDelayCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->AutoDelayCheckBox->setChecked(checked);
    SendCmd((checked)?"0ATO":"1ATO");
}

void AVSettingsDialog::on_EffectStrengthSlider_valueChanged(int value)
{
    SendCmd(QString("%1ATT").arg(value, 2, 10, QLatin1Char('0')));
}

void AVSettingsDialog::on_LoudnessCheckBox_clicked(bool checked)
{
    checked = !checked;
    ui->LoudnessCheckBox->setChecked(checked);
    SendCmd((checked)?"0ATW":"1ATW");
}

void AVSettingsDialog::on_HDMIOutputComboBox_activated(int index)
{
    SendCmd(ui->HDMIOutputComboBox->itemData(index).toString() + "HO");
    ui->HDMIOutputComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_HDMIAudioComboBox_activated(int index)
{
    SendCmd(ui->HDMIAudioComboBox->itemData(index).toString() + "HA");
    ui->HDMIAudioComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_PQLSComboBox_activated(int index)
{
    SendCmd(ui->PQLSComboBox->itemData(index).toString() + "PQ");
    ui->PQLSComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_DimmerComboBox_activated(int index)
{
    SendCmd(ui->DimmerComboBox->itemData(index).toString() + "SAA");
    ui->DimmerComboBox->setCurrentIndex(-1);
}

void AVSettingsDialog::on_SleepComboBox_activated(int index)
{
    SendCmd(ui->SleepComboBox->itemData(index).toString() + "SAB");
    ui->SleepComboBox->setCurrentIndex(-1);
}
