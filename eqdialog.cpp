/*
 * AVRPioRemote
 * Copyright (C) 2013  Andreas Müller, Ulrich Mensfeld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "eqdialog.h"
#include "ui_eqdialog.h"
#include <QDebug>

const char* eqnames[] = {
    "Eq63",
    "Eq125",
    "Eq250",
    "Eq500",
    "Eq1k",
    "Eq2k",
    "Eq4k",
    "Eq8k",
    "Eq16k",
};



EQDialog::EQDialog(QWidget *parent, ReceiverInterface &Comm, QSettings &settings) :
    QDialog(parent),
    ui(new Ui::EQDialog),
    m_Comm(Comm),
    m_Timer(this),
    m_Settings(settings),
    m_SelectedPreset(-1), // flat
    m_PositionSet(false),
    m_ToneON(false)
{
    ui->setupUi(this);
    //this->setFixedSize(this->size());

    // restore the position of the window
    if (m_Settings.value("SaveEQWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("EQWindowGeometry").toByteArray());
    }

    // communication
    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));

    // slider
    connect(ui->eq63,  SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq125, SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq250, SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq500, SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq1k,  SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq2k,  SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq4k,  SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq8k,  SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq16k, SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));

    // emphasis slider
    connect(ui->eqEmphasisBass, SIGNAL(valueChanged(int)), this, SLOT(OnEmphasisSliderReleased()));
    connect(ui->eqEmphasisCenter, SIGNAL(valueChanged(int)), this, SLOT(OnEmphasisSliderReleased()));

    // save sliders in a list
    m_EQSliders.append(ui->eq63);
    m_EQSliders.append(ui->eq125);
    m_EQSliders.append(ui->eq250);
    m_EQSliders.append(ui->eq500);
    m_EQSliders.append(ui->eq1k);
    m_EQSliders.append(ui->eq2k);
    m_EQSliders.append(ui->eq4k);
    m_EQSliders.append(ui->eq8k);
    m_EQSliders.append(ui->eq16k);

    // save dB value labels in a list
    m_EQLabels.append(ui->wert63);
    m_EQLabels.append(ui->wert125);
    m_EQLabels.append(ui->wert250);
    m_EQLabels.append(ui->wert500);
    m_EQLabels.append(ui->wert1k);
    m_EQLabels.append(ui->wert2k);
    m_EQLabels.append(ui->wert4k);
    m_EQLabels.append(ui->wert8k);
    m_EQLabels.append(ui->wert16k);

    // configure the timer
    connect((&m_Timer), SIGNAL(timeout()), this, SLOT(Timeout()));
    m_Timer.setSingleShot(true);
    m_Timer.setInterval(200);

  //  QString path = QDir::currentPath() + "/" + "ATBEQPresets.xml";
  //  ReadFile(path);

    QStringList mstr1;
    mstr1 << "Memory 1"  << "Memory 2" << "Memory 3" << "Memory 4" << "Memory 5" << "Memory 6";
    ui->selectmem->addItems(mstr1);

    QString str1 = m_Settings.value("IP/4").toString(); //letztes Oktett IP anhängen, falls mehrere Reciever
    QString str = QString("EQ-%1/SelectedPreset").arg(str1);
    m_SelectedPreset = m_Settings.value(str, 0).toInt();

    SelectPreset(m_SelectedPreset);
    ui->selectmem->setCurrentIndex(m_SelectedPreset);
    connect(ui->selectmem, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectmemCurrentIndexChanged(int)));

    // emphasis dialog
    m_EmphasisDialog = new EmphasisDialog(this, m_Settings);
    connect(m_EmphasisDialog, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));
    connect(m_EmphasisDialog, SIGNAL(CenterChanged(int)), this, SLOT(CenterChanged(int)));
    connect(m_EmphasisDialog, SIGNAL(BassChanged(int)), this, SLOT(BassChanged(int)));

    // save/restore filter
    RestoreFilter();

    connect(ui->saveEqCheckBox, SIGNAL(toggled(bool)), this, SLOT(onSaveCheckBoxToggled(bool)));
    connect(ui->saveEmphasisCheckBox, SIGNAL(toggled(bool)), this, SLOT(onSaveCheckBoxToggled(bool)));
    connect(ui->saveToneCheckBox, SIGNAL(toggled(bool)), this, SLOT(onSaveCheckBoxToggled(bool)));
    connect(ui->saveXCurveCheckBox, SIGNAL(toggled(bool)), this, SLOT(onSaveCheckBoxToggled(bool)));

    QStringList responseList;
    responseList << EQResponse_ATB().getResponseID();
    responseList << ToneResponse_TO_ZGA().getResponseID();
    responseList << BassResponse_BA_ZGB().getResponseID();
    responseList << TrebleResponse_TR_ZGG().getResponseID();
    responseList << XCurveResponse_SST().getResponseID();
    responseList << DigitalFilterResponse_DGF().getResponseID();
    responseList << DRCResponse_LTN().getResponseID();
    responseList << TheaterFilterResponse_RAS().getResponseID();
    responseList << UpsamplinResponse_UPS().getResponseID();
    responseList << LoudnessResponse_LDM().getResponseID();
    responseList << FixedPCMResponse_FXP().getResponseID();
    responseList << AudioScalarResponse_ASC().getResponseID();
    MsgDistributor::AddResponseListener(this, responseList);

    ui->mainLayout->setSizeConstraint(QLayout::SetFixedSize);
}


EQDialog::~EQDialog()
{
    delete m_EmphasisDialog;
    delete ui;
}


void EQDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("EQWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}

void EQDialog::SetEqSlider(int value, QSlider* slider, QLabel* label)
{
    QString str;
    double eqValue = 0;
    slider->setValue(value);
    if (m_Comm.IsPioneer())
    {
        eqValue = ((double)value - 50.0) / 2.0;
    }
    else
    {
        eqValue = ((double)value) / 2.0;
    }

    if (eqValue == 0.0) //eqValue > -0.1 && eqValue <= 0.1)
    {
        str = "0.0";
    }
    else if (eqValue < 0.0)
        str = QString("%1").arg(eqValue, 3, 'f', 1);
    else
        str = QString("+%1").arg(eqValue, 3, 'f', 1);
    label->setText(str);
}

void EQDialog::SetToneSlider(int value, QSlider* slider, QLabel* label)
{
    QString str;
    slider->setValue(value);
    if (m_Comm.IsPioneer())
    {
        value = 6 - value;
    }
    else
    {
        value = -value;
    }
    str = QString("%1dB").arg(value);
    label->setText(str);
}

void EQDialog::SetXCurveSlider(int value, QSlider* slider, QLabel* label)
{
    slider->setValue(value);
    if (value <= 6 && value > 0)
    {
        double dB = (double)value * (-.5f);
        label->setText(QString("%1").arg(dB, 3, 'f', 1));
    }
    else
    {
        label->setText(tr("OFF"));
    }
}

void EQDialog::SetEmphasisSlider(int value, QSlider* slider, QLabel* label)
{
    QString str;
    slider->blockSignals(true);
    slider->setValue(value);
    slider->blockSignals(false);
    double dValue = ((double)value - 50.0)*0.5;
    str = QString("%1dB").arg(dValue);
    label->setText(str);
}

void EQDialog::RestorePreset(int nr, bool eq, bool emph, bool tone, bool xcurve)
{
    //Channel-Level aus public-Speicher in Memory x sichern, gem. Auswahl Combobox,
    QString str;
    QString str1;
    int value;
    int ip;
    ip = m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever

    // restore eq settings
    if (eq)
    {
        for (int i = 0; i < m_EQSliders.count(); i++)
        {
            str = QString("mem%1-%2/%3").arg(nr).arg(ip).arg(eqnames[i]);
            if (m_Comm.IsPioneer())
            {
                value = m_Settings.value(str, 50).toInt();
            }
            else
            {
                value = m_Settings.value(str, 0).toInt();
            }
            SetEqSlider(value, m_EQSliders[i], m_EQLabels[i]);
        }
    }

    // restore emphasis settings
    if (emph)
    {
        str = QString("mem%1-%2/Emphasis").arg(nr).arg(ip);
        str1 = m_Settings.value(str).toString();
        if (str1 != "")
        {
            m_EmphasisDialog->SetChannelString(str1);
            SetEmphasisSlider(m_EmphasisDialog->GetBass(), ui->eqEmphasisBass, ui->wertEmphasisBass);
            SetEmphasisSlider(m_EmphasisDialog->GetCenter(), ui->eqEmphasisCenter, ui->wertEmphasisCenter);
            //SendCmd("?ILV");
        }
    }

    // restore tone settings
    if (tone)
    {
        // bass
        str = QString("mem%1-%2/Eqbass").arg(nr).arg(ip);
        if (m_Comm.IsPioneer())
        {
            value = m_Settings.value(str, 6).toInt();
        }
        else
        {
            value = m_Settings.value(str, 0).toInt();
        }
        SetToneSlider(value, ui->eqba, ui->wertbass);
        str = QString("%1BA").arg(value, 2, 10, QChar('0'));
        SendCmd(str);

        // treble
        str = QString("mem%1-%2/Eqtreble").arg(nr).arg(ip);
        value = m_Settings.value(str, 6).toInt();
        SetToneSlider(value, ui->eqtr, ui->werttreble);
        str = QString("%1TR").arg(value, 2, 10, QChar('0'));
        SendCmd(str);
    }

    // restore X-Curve
    if (xcurve)
    {
        str = QString("mem%1-%2/X-Curve").arg(nr).arg(ip);
        value = m_Settings.value(str, 0).toInt();
        SetXCurveSlider(value, ui->XCurveSlider, ui->XCurveLabel);
        str = QString("%1SST").arg(value);
        SendCmd(str);
    }

    // restore user defined preset info
    str = QString("mem%1-%2/EQset").arg(ui->selectmem->currentIndex()).arg(ip);
    str1 = m_Settings.value(str, "").toString();
    ui->meminf->setText(str1);
}

void EQDialog::SavePreset(int nr, bool eq, bool emph, bool tone, bool xcurve)
{
    //Channel-Level aus public-Speicher in Memory x sichern, gem. Auswahl Combobox,
    QString str;
    QString str1;
    int ip;
    ip = m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever

    // save eq settings
    if (eq)
    {
        for (int i = 0; i < m_EQSliders.count(); i++)
        {
            str = QString("mem%1-%2/%3").arg(nr).arg(ip).arg(eqnames[i]);
            m_Settings.setValue(str, m_EQSliders[i]->value());
        }
    }

    // save emphasis settings
    if (emph)
    {
        str = QString("mem%1-%2/Emphasis").arg(nr).arg(ip);
        str1 = m_EmphasisDialog->GetChannelString();
        if (str1 != "")
        {
            m_Settings.setValue(str, str1);
        }
    }

    // save tone settings
    if (tone)
    {
        // bass
        str = QString("mem%1-%2/Eqbass").arg(nr).arg(ip);
        m_Settings.setValue(str, ui->eqba->value());
        // treble
        str = QString("mem%1-%2/Eqtreble").arg(nr).arg(ip);
        m_Settings.setValue(str, ui->eqtr->value());
    }

    // save X-Curve
    if (xcurve)
    {
        str = QString("mem%1-%2/X-Curve").arg(nr).arg(ip);
        m_Settings.setValue(str, ui->XCurveSlider->value());
    }

    // save user defined preset info
    str1 = ui->meminf->text();
    if (str1 != "")
    {
        str = QString("mem%1-%2/EQset").arg(ui->selectmem->currentIndex()).arg(ip);
        m_Settings.setValue(str, str1);
    }
}

void EQDialog::SelectPreset(int preset)
{
    if (preset >= 0 && preset <= 5)
    {
        m_SelectedPreset = preset;
        RestorePreset(preset, ui->saveEqCheckBox->isChecked(), ui->saveEmphasisCheckBox->isChecked(), ui->saveToneCheckBox->isChecked(), ui->saveXCurveCheckBox->isChecked());
    }
    else // FLAT
    {
        // set eq sliders to flat
        if (ui->saveEqCheckBox->isChecked())
        {
            for (int i = 0; i < m_EQSliders.count(); i++)
            {
                if (m_Comm.IsPioneer())
                {
                    SetEqSlider(50, m_EQSliders[i], m_EQLabels[i]);
                }
                else
                {
                    SetEqSlider(0, m_EQSliders[i], m_EQLabels[i]);
                }
            }
        }
        //SetToneSlider(6, ui->eqba, ui->wertbass); // Bass
        //SetToneSlider(6, ui->eqtr, ui->werttreble); // Treble
        if (m_Comm.IsPioneer())
        {
            m_EmphasisDialog->SetCenter(50);
            m_EmphasisDialog->SetBass(50);
        }
        else
        {
            m_EmphasisDialog->SetCenter(0);
            m_EmphasisDialog->SetBass(0);
        }
        // select the FLAT button
        ui->eqFlatPushButton->setChecked(false);
    }
    m_Timer.start();
}

void EQDialog::ShowEQDialog()
{
    if (!isVisible())
    {
        if (!m_PositionSet || !m_Settings.value("SaveEQWindowGeometry", false).toBool())
        {
            QWidget* Parent = dynamic_cast<QWidget*>(parent());
            int x = Parent->pos().x() + Parent->width() + 20;
            QPoint pos;
            pos.setX(x);
            pos.setY(Parent->pos().y());
            this->move(pos);
            m_PositionSet = true;
        }
        this->show();
    }
    m_EmphasisDialog->SetIsPioneer(m_Comm.IsPioneer());
    if (m_Comm.IsPioneer())
    {
        SendCmd("?ATB");
        SendCmd("?BA");
        SendCmd("?TR");
        SendCmd("?TO");
        SendCmd("?SST");
        SendCmd("?ILV");
        ui->eqba->setMaximum(12);
        ui->eqba->setMinimum(0);
        ui->eqba->setValue(6);
        ui->eqtr->setMaximum(12);
        ui->eqtr->setMinimum(0);
        ui->eqtr->setValue(6);
        m_ToneON = false;
        ui->bypass->setVisible(true);
        ui->eqtr->setEnabled(false);
        ui->eqba->setEnabled(false);
        ui->frame_x_curve->setVisible(true);
        ui->label_3->setText("+6 dB");
        ui->label_2->setText("-6 dB");
        ui->saveXCurveCheckBox->setVisible(true);
        foreach (QSlider *slider, m_EQSliders) {
            slider->setMaximum(62);
            slider->setMinimum(38);
            slider->setValue(50);
        }

        ui->emphasisPushButton->setEnabled(true);
        ui->eqEmphasisBass->setEnabled(true);
        ui->eqEmphasisCenter->setEnabled(true);
        ui->saveEmphasisCheckBox->setEnabled(true);
        ui->saveXCurveCheckBox->setVisible(true);
    }
    else
    {
        SendCmd("ACEQSTN"); // EQ
        SendCmd("TFRQSTN"); // Bass/Treble
        SendCmd("TCLQSTN"); // Temporary channel level
        SendCmd("DGFQSTN"); // Digital Filter
        SendCmd("UPSQSTN"); // Upsampling
        SendCmd("LDMQSTN"); // Loudness managemant
        SendCmd("RASQSTN"); // Theater filter
        SendCmd("LTNQSTN"); // DRC
        SendCmd("ASCQSTN"); // Audio scalar
        ui->eqba->setMaximum(10);
        ui->eqba->setMinimum(-10);
        ui->eqba->setValue(0);
        ui->eqtr->setMaximum(10);
        ui->eqtr->setMinimum(-10);
        ui->eqtr->setValue(0);
        m_ToneON = true;
        ui->bypass->setVisible(false);
        ui->eqtr->setEnabled(true);
        ui->eqba->setEnabled(true);
        ui->frame_x_curve->setVisible(false);
        ui->label_3->setText("+12 dB");
        ui->label_2->setText("-12 dB");
        ui->saveXCurveCheckBox->setVisible(false);
        foreach (QSlider *slider, m_EQSliders) {
            slider->setMaximum(24);
            slider->setMinimum(-24);
            slider->setValue(0);
        }

        ui->emphasisPushButton->setEnabled(false);
        ui->eqEmphasisBass->setEnabled(false);
        ui->eqEmphasisCenter->setEnabled(false);
        ui->saveEmphasisCheckBox->setEnabled(false);
        ui->saveXCurveCheckBox->setVisible(false);
    }
    //ui->mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
    //ui->mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    ui->mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
    adjustSize();
    resize(minimumSizeHint());
}

void EQDialog::ResponseReceived(ReceivedObjectBase *response)
{
    if (!isVisible())
    {
        return;
    }

    // EQ
    EQResponse_ATB* eq = dynamic_cast<EQResponse_ATB*>(response);
    if (eq != NULL)
    {
        const QVector<int>& eqData = eq->GetEQData();
        for (int i = 0; i < m_EQSliders.count() && i < eqData.count(); i++)
        {
            SetEqSlider(eqData[i], m_EQSliders[i], m_EQLabels[i]);
        }
        return;
    }

    // Tone on/off
    ToneResponse_TO_ZGA* tone = dynamic_cast<ToneResponse_TO_ZGA*>(response);
    if (tone != NULL)
    {
        if (tone->IsToneOn())
        {
            m_ToneON = true;
            ui->bypass->setText("Tone On");
            ui->bypass->setChecked(true);
            ui->eqtr->setEnabled(true);
            ui->eqba->setEnabled(true);
            SendCmd("?BA");
            SendCmd("?TR");
        }
        else
        {
            m_ToneON = false;
            ui->bypass->setText("Tone Off");
            ui->bypass->setChecked(false);
            ui->eqtr->setDisabled(true);
            ui->eqba->setDisabled(true);

            ui->eqba->setValue(6);
            ui->wertbass->setText((tr("OFF")));

            ui->eqtr->setValue(6);
            ui->werttreble->setText((tr("OFF")));
        }
        return;
    }

    // BASS
    BassResponse_BA_ZGB* bass = dynamic_cast<BassResponse_BA_ZGB*>(response);
    if (bass != NULL)
    {
        if (m_ToneON)
        {
            SetToneSlider(bass->GetValue(), ui->eqba, ui->wertbass);
        }
        else
        {
            if (m_Comm.IsPioneer())
            {
                ui->eqba->setValue(6);
            }
            else
            {
                ui->eqba->setValue(0);
            }
            ui->wertbass->setText((tr("OFF")));
        }
        return;
    }

    // TREBLE
    TrebleResponse_TR_ZGG* treble = dynamic_cast<TrebleResponse_TR_ZGG*>(response);
    if (treble != NULL)
    {
        if (m_ToneON)
        {
            SetToneSlider(treble->GetValue(), ui->eqtr, ui->werttreble);
        }
        else
        {
            if (m_Comm.IsPioneer())
            {
                ui->eqtr->setValue(6);
            }
            else
            {
                ui->eqtr->setValue(0);
            }
            ui->werttreble->setText((tr("OFF")));
        }
        return;
    }

    // X-Curve
    XCurveResponse_SST* xcurve = dynamic_cast<XCurveResponse_SST*>(response);
    if (xcurve != NULL)
    {
        SetXCurveSlider(xcurve->GetValue(), ui->XCurveSlider, ui->XCurveLabel);
        return;
    }

    // Digital filter
    DigitalFilterResponse_DGF* dfilter = dynamic_cast<DigitalFilterResponse_DGF*>(response);
    if (dfilter != NULL)
    {
        switch(dfilter->GetValue())
        {
        case 1:
            ui->dfilter_sharp_radioButton->setChecked(true);
            break;
        case 2:
            ui->dfilter_short_radioButton->setChecked(true);
            break;
        case 0:
        default:
            ui->dfilter_slow_radioButton->setChecked(true);
            break;
        }
        ui->dfilter_slow_radioButton->setEnabled(true);
        ui->dfilter_sharp_radioButton->setEnabled(true);
        ui->dfilter_short_radioButton->setEnabled(true);
        return;
    }

    // Theater filter
    TheaterFilterResponse_RAS* tfilter = dynamic_cast<TheaterFilterResponse_RAS*>(response);
    if (tfilter != NULL)
    {
        ui->theater_checkBox->setChecked(tfilter->GetValue() != 0);
        ui->theater_checkBox->setEnabled(true);
        return;
    }

    // DRC
    DRCResponse_LTN* drc = dynamic_cast<DRCResponse_LTN*>(response);
    if (drc != NULL)
    {
        switch(drc->GetValue())
        {
        case 1:
            ui->drc_on_radioButton->setChecked(true);
            break;
        case 2:
            ui->drc_auto_radioButton->setChecked(true);
            break;
        case 0:
        default:
            ui->drc_off_radioButton->setChecked(true);
            break;
        }
        ui->drc_off_radioButton->setEnabled(true);
        ui->drc_on_radioButton->setEnabled(true);
        ui->drc_auto_radioButton->setEnabled(true);
        return;
    }

    // Upsampling
    UpsamplinResponse_UPS* upsampling = dynamic_cast<UpsamplinResponse_UPS*>(response);
    if (upsampling != NULL)
    {
        switch(upsampling->GetValue())
        {
        case 1:
            ui->upsampling_x2_radioButton->setChecked(true);
            break;
        case 2:
            ui->upsampling_x4_radioButton->setChecked(true);
            break;
        case 0:
        default:
            ui->upsampling_x1_radioButton->setChecked(true);
            break;
        }
        ui->upsampling_x1_radioButton->setEnabled(true);
        ui->upsampling_x2_radioButton->setEnabled(true);
        ui->upsampling_x4_radioButton->setEnabled(true);
        return;
    }

    // Loudness
    LoudnessResponse_LDM* loudness = dynamic_cast<LoudnessResponse_LDM*>(response);
    if (loudness != NULL)
    {
        ui->loudness_checkBox->setChecked(loudness->GetValue() != 0);
        ui->loudness_checkBox->setEnabled(true);
        return;
    }

    // Fixed PCM
    FixedPCMResponse_FXP* fixedpcm = dynamic_cast<FixedPCMResponse_FXP*>(response);
    if (fixedpcm != NULL)
    {
        ui->fixed_pcm_checkBox->setChecked(fixedpcm->GetValue() != 0);
        ui->fixed_pcm_checkBox->setEnabled(true);
        return;
    }

    // Audio Scalar
    AudioScalarResponse_ASC* audioscalar = dynamic_cast<AudioScalarResponse_ASC*>(response);
    if (audioscalar != NULL)
    {
        ui->audio_scalar_checkBox->setChecked(audioscalar->GetValue() != 0);
        ui->audio_scalar_checkBox->setEnabled(true);
        return;
    }
}

void EQDialog::Timeout()
{
    // send the eq settings to the receiver
    QString str;

/*  bass+treble, set not here because of sound-mode may not allow setting and generate a "not avail"
    str=QString("%1BA").arg(m_Sliders[9]->value()*-1);
    if (str.size() <4)
            str="0"+str;
     SendCmd(str);

     str=QString("%1TR").arg(m_Sliders[10]->value()*-1);
     if (str.size() <4)
             str="0"+str;
      SendCmd(str);
*/

    if (m_Comm.IsPioneer())
    {
        QString cmd = "00";
        for (int i = 0; i < m_EQSliders.count(); i++)
        {
            QString str = QString("%1").arg(m_EQSliders[i]->value(), 2, 10, QChar('0'));
            cmd.append(str);
        }
        cmd.append("50ATB");
        emit SendCmd(cmd);
    }
    else
    {
        QString cmd = "ACE";
        for (int i = 0; i < m_EQSliders.count(); i++)
        {
            QString str;
            if (m_EQSliders[i]->value() == 0)
            {
                str = "000";
            }
            else if (m_EQSliders[i]->value() < 0)
            {
                str = QString::asprintf("-%02X", -m_EQSliders[i]->value());
            }
            else
            {
                str = QString::asprintf("+%02X", m_EQSliders[i]->value());
            }
            cmd.append(str);
        }
        emit SendCmd(cmd);
    }
}


void EQDialog::OnSliderValueChanged(int/* value*/)
{
    // don't change the eq settings yet, because the receiver is somewhat slow
    // and get irritated if the next command is sent to early
    // so wait till the value is not changing anymore
    m_Timer.start();
}


void EQDialog::OnEmphasisSliderReleased()
{
    QObject* sender = QObject::sender();
    //QString id = sender->objectName();
    //qDebug() << id;
    if (sender == ui->eqEmphasisCenter)
    {
        m_EmphasisDialog->SetCenter(ui->eqEmphasisCenter->value());
        SetEmphasisSlider(ui->eqEmphasisCenter->value(), ui->eqEmphasisCenter, ui->wertEmphasisCenter);
    }
    else if (sender == ui->eqEmphasisBass)
    {
        m_EmphasisDialog->SetBass(ui->eqEmphasisBass->value());
        SetEmphasisSlider(ui->eqEmphasisBass->value(), ui->eqEmphasisBass, ui->wertEmphasisBass);
    }
}



void EQDialog::on_eqFlatPushButton_clicked() // FLAT
{
    SelectPreset(-1);
}


void EQDialog::on_savebutt_clicked()
{
    SavePreset(m_SelectedPreset, ui->saveEqCheckBox->isChecked(), ui->saveEmphasisCheckBox->isChecked(), ui->saveToneCheckBox->isChecked(), ui->saveXCurveCheckBox->isChecked());
}


void EQDialog::on_restbutt_clicked()
{
    RestorePreset(m_SelectedPreset, ui->saveEqCheckBox->isChecked(), ui->saveEmphasisCheckBox->isChecked(), ui->saveToneCheckBox->isChecked(), ui->saveXCurveCheckBox->isChecked());
}


void EQDialog::onSelectmemCurrentIndexChanged(int index)
{
    QString str;
    int ip;
    m_SelectedPreset = index;
    ip = m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever
    str = QString("mem%1-%2/EQset").arg(index).arg(ip);
    str = m_Settings.value(str).toString();
    ui->meminf->setText(str);
    str = QString("EQ-%1/SelectedPreset").arg(ip);
    m_Settings.setValue(str, m_SelectedPreset);
}


void EQDialog::on_eqba_sliderReleased()
{
    QString str;
    int i = ui->eqba->value();
    if (m_Comm.IsPioneer())
    {
        str = QString("%1BA").arg(i, 2, 10, QChar('0'));
        SendCmd(str);
    }
    else
    {
        if (i == 0)
        {
            str = "TFRB00";
        }
        else if (i < 0)
        {
            str = QString::asprintf("TFRB+%1X", -i);
        }
        else
        {
            str = QString::asprintf("TFRB-%1X", i);
        }
        SendCmd(str);
    }
}


void EQDialog::on_eqtr_sliderReleased()
{
    QString str;
    int i = ui->eqtr->value();
    if (m_Comm.IsPioneer())
    {
        str = QString("%1TR").arg(i, 2, 10, QChar('0'));
        SendCmd(str);
    }
    else
    {
        if (i == 0)
        {
            str = "TFRT00";
        }
        else if (i < 0)
        {
            str = QString::asprintf("TFRT+%1X", -i);
        }
        else
        {
            str = QString::asprintf("TFRT-%1X", i);
        }
        SendCmd(str);
    }
}


void EQDialog::on_bypass_clicked()
{
    if (m_Comm.IsPioneer())
    {
        if (!ui->bypass->isChecked())
        {
            SendCmd("0TO");
            ui->bypass->setText("Tone Off");
        }
        else
        {
            SendCmd("1TO");
            ui->bypass->setText("Tone On");
        }
        SendCmd("?TO");
    }
}


void EQDialog::on_XCurveSlider_sliderReleased()
{
    if (m_Comm.IsPioneer())
    {
        QString cmd = QString("%1SST").arg(ui->XCurveSlider->sliderPosition());
        emit SendCmd(cmd);
    }
}


void EQDialog::on_emphasisPushButton_clicked()
{
    m_EmphasisDialog->show();
}


void EQDialog::CenterChanged(int n)
{
    SetEmphasisSlider(n, ui->eqEmphasisCenter, ui->wertEmphasisCenter);
}


void EQDialog::BassChanged(int n)
{
    SetEmphasisSlider(n, ui->eqEmphasisBass, ui->wertEmphasisBass);
}


void EQDialog::SaveFilter()
{
    QString str;
    int ip;
    ip = m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever
    str = QString("EQ-%1/SaveEq").arg(ip);
    m_Settings.setValue(str, ui->saveEqCheckBox->isChecked());
    str = QString("EQ-%1/SaveEmphasis").arg(ip);
    m_Settings.setValue(str, ui->saveEmphasisCheckBox->isChecked());
    str = QString("EQ-%1/SaveTone").arg(ip);
    m_Settings.setValue(str, ui->saveToneCheckBox->isChecked());
    str = QString("EQ-%1/SaveXCurve").arg(ip);
    m_Settings.setValue(str, ui->saveXCurveCheckBox->isChecked());
}


void EQDialog::RestoreFilter()
{
    QString str;
    int ip;
    ip = m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever
    str = QString("EQ-%1/SaveEq").arg(ip);
    ui->saveEqCheckBox->setChecked(m_Settings.value(str, true).toBool());
    str = QString("EQ-%1/SaveEmphasis").arg(ip);
    ui->saveEmphasisCheckBox->setChecked(m_Settings.value(str, true).toBool());
    str = QString("EQ-%1/SaveTone").arg(ip);
    ui->saveToneCheckBox->setChecked(m_Settings.value(str, true).toBool());
    str = QString("EQ-%1/SaveXCurve").arg(ip);
    ui->saveXCurveCheckBox->setChecked(m_Settings.value(str, true).toBool());
}


void EQDialog::onSaveCheckBoxToggled(bool)
{
    SaveFilter();
    if (ui->saveEqCheckBox->isChecked() ||
            ui->saveEmphasisCheckBox->isChecked() ||
            ui->saveToneCheckBox->isChecked() ||
            ui->saveXCurveCheckBox->isChecked())
    {
        ui->savebutt->setEnabled(true);
        ui->restbutt->setEnabled(true);
    }
    else
    {
        ui->savebutt->setEnabled(false);
        ui->restbutt->setEnabled(false);
    }
}

void EQDialog::on_dfilter_slow_radioButton_clicked()
{
    SendCmd("DGF00");
    SendCmd("DGFQSTN");
}

void EQDialog::on_dfilter_sharp_radioButton_clicked()
{
    SendCmd("DGF01");
    SendCmd("DGFQSTN");
}

void EQDialog::on_dfilter_short_radioButton_clicked()
{
    SendCmd("DGF02");
    SendCmd("DGFQSTN");
}

void EQDialog::on_theater_checkBox_clicked()
{
    if (ui->theater_checkBox->isChecked())
    {
        SendCmd("RAS01");
    }
    else
    {
        SendCmd("RAS00");
    }
    SendCmd("RASQSTN");
    //ui->theater_checkBox->setChecked(!ui->theater_checkBox->isChecked());
    //ui->theater_checkBox->setEnabled(false);
}

void EQDialog::on_drc_off_radioButton_clicked()
{
    SendCmd("LTN00");
    SendCmd("LTNQSTN");
    //ui->drc_off_radioButton->setEnabled(false);
    //ui->drc_on_radioButton->setEnabled(false);
    //ui->drc_auto_radioButton->setEnabled(false);
}

void EQDialog::on_drc_on_radioButton_clicked()
{
    SendCmd("LTN01");
    SendCmd("LTNQSTN");
    //ui->drc_off_radioButton->setEnabled(false);
    //ui->drc_on_radioButton->setEnabled(false);
    //ui->drc_auto_radioButton->setEnabled(false);
}

void EQDialog::on_drc_auto_radioButton_clicked()
{
    SendCmd("LTN02");
    SendCmd("LTNQSTN");
    //ui->drc_off_radioButton->setEnabled(false);
    //ui->drc_on_radioButton->setEnabled(false);
    //ui->drc_auto_radioButton->setEnabled(false);
}

void EQDialog::on_upsampling_x1_radioButton_clicked()
{
    SendCmd("UPS00");
    SendCmd("UPSQSTN");
    //ui->upsampling_x1_radioButton->setEnabled(false);
    //ui->upsampling_x2_radioButton->setEnabled(false);
    //ui->upsampling_x4_radioButton->setEnabled(false);
}

void EQDialog::on_upsampling_x2_radioButton_clicked()
{
    SendCmd("UPS01");
    SendCmd("UPSQSTN");
    //ui->upsampling_x1_radioButton->setEnabled(false);
    //ui->upsampling_x2_radioButton->setEnabled(false);
    //ui->upsampling_x4_radioButton->setEnabled(false);
}

void EQDialog::on_upsampling_x4_radioButton_clicked()
{
    SendCmd("UPS02");
    SendCmd("UPSQSTN");
    //ui->upsampling_x1_radioButton->setEnabled(false);
    //ui->upsampling_x2_radioButton->setEnabled(false);
    //ui->upsampling_x4_radioButton->setEnabled(false);
}

void EQDialog::on_loudness_checkBox_clicked()
{
    if (ui->loudness_checkBox->isChecked())
    {
        SendCmd("LDM01");
    }
    else
    {
        SendCmd("LDM00");
    }
    SendCmd("LDMQSTN");
    ui->loudness_checkBox->setChecked(!ui->loudness_checkBox->isChecked());
    //ui->loudness_checkBox->setEnabled(false);
}

void EQDialog::on_fixed_pcm_checkBox_clicked()
{
    if (ui->fixed_pcm_checkBox->isChecked())
    {
        SendCmd("FXP01");
    }
    else
    {
        SendCmd("FXP00");
    }
    SendCmd("FXPQSTN");
    ui->fixed_pcm_checkBox->setChecked(!ui->fixed_pcm_checkBox->isChecked());
    //ui->fixed_pcm_checkBox->setEnabled(false);
}

void EQDialog::on_audio_scalar_checkBox_clicked()
{
    if (ui->audio_scalar_checkBox->isChecked())
    {
        SendCmd("ASC01");
    }
    else
    {
        SendCmd("ASC00");
    }
    SendCmd("ASCQSTN");
    ui->audio_scalar_checkBox->setChecked(!ui->audio_scalar_checkBox->isChecked());
    //ui->audio_scalar_checkBox->setEnabled(false);
}

