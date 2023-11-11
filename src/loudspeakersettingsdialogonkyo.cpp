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
#include "loudspeakersettingsdialogonkyo.h"
#include "ui_loudspeakersettingsdialogonkyo.h"
#include <QDebug>
#include "receiver_interface/receiverinterface.h"
#include <QThread>

LoudspeakerSettingsOnkyoDialog::LoudspeakerSettingsOnkyoDialog(QWidget *parent, QSettings &settings,ReceiverInterface &Comm ) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::LoudspeakerSettingsOnkyoDialog),
    m_Comm(Comm),
    m_PositionSet(false),
    m_RefreshSpeakerSettings(false)
{
    ui->setupUi(this);
    //this->setFixedSize(this->size());

    // restore the position of the window
    if (m_Settings.value("SaveLSSettingsWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("LSSettingsWindowGeometry").toByteArray());
    }

    initSpeakerConfiguration();

    // communication
//    connect(&m_Comm, SIGNAL(DataReceived(QString)), this, SLOT(SpeakerReceived(QString)));
    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));

    // save sliders in a list
    m_Sliders.append(ui->sfl);  // front left
    m_Sliders.append(ui->sfr);  // front right
    m_Sliders.append(ui->sc);   // center
    m_Sliders.append(ui->ssl);  // sur left
    m_Sliders.append(ui->ssr);  // sur right
    m_Sliders.append(ui->ssbl); // sur back left
    m_Sliders.append(ui->ssbr); // sur back right
    m_Sliders.append(ui->ssw);  // subw 1
    m_Sliders.append(ui->sfhl); // front height left
    m_Sliders.append(ui->sfhr); // front height right
    m_Sliders.append(ui->sfwl); // front wide lift
    m_Sliders.append(ui->sfwr); // front wide right
    m_Sliders.append(ui->ssw2); // subw 2

    foreach (QSlider* slider, m_Sliders) {
        connect(slider, SIGNAL(sliderReleased()), this, SLOT(ValueChanged()));
    }

    // save dB value labels in a list
    m_Labels.append(ui->lfl);
    m_Labels.append(ui->lfr);
    m_Labels.append(ui->lc);
    m_Labels.append(ui->lsl);
    m_Labels.append(ui->lsr);
    m_Labels.append(ui->lsbl);
    m_Labels.append(ui->lsbr);
    m_Labels.append(ui->lsw);
    m_Labels.append(ui->lfhl);
    m_Labels.append(ui->lfhr);
    m_Labels.append(ui->lfwl);
    m_Labels.append(ui->lfwr);
    m_Labels.append(ui->lsw2);

    m_MCACCButtons.append(ui->mc1);
    m_MCACCButtons.append(ui->mc2);
    m_MCACCButtons.append(ui->mc3);
    foreach (QCheckBox* checkBox, m_MCACCButtons) {
        connect(checkBox, SIGNAL(clicked()), this, SLOT(checkbox()));
    }

    QStringList list;
    list << "Small" << "Large";
    ui->comboBoxFrontSeting->addItems(list);

    list.clear();
    list << "No" << "Small" << "Large";
    ui->comboBoxCenterSetting->addItems(list);
    ui->comboBoxFrontHeight1Setting->addItems(list); // height 1
    ui->comboBoxFrontHeight2Setting->addItems(list); // height 2
    ui->comboBoxSurroundSetting->addItems(list);
    ui->comboBoxSurroundBackSetting->addItems(list);

    list.clear();
    list << "No" << "Yes 1ch" << "Yes 2ch";
    ui->comboBoxSubwooferSetting->addItems(list);

    m_SpeakerCombos << ui->comboBoxFrontSeting;
    m_SpeakerCombos << ui->comboBoxCenterSetting;
    m_SpeakerCombos << ui->comboBoxFrontHeight1Setting;
    m_SpeakerCombos << ui->comboBoxFrontHeight2Setting;
    //m_SpeakerSettings << ui->comboBoxFrontWideSetting;
    m_SpeakerCombos << ui->comboBoxSurroundSetting;
    m_SpeakerCombos << ui->comboBoxSurroundBackSetting;
    m_SpeakerCombos << ui->comboBoxSubwooferSetting;
    //m_SpeakerSettings << ui->comboBoxSubwoofer2Setting;
    m_SpeakerCombos << ui->comboBoxTopForwardSetting;
    m_SpeakerCombos << ui->comboBoxTopMiddleSetting;
    m_SpeakerCombos << ui->comboBoxTopBackwardSetting;
    foreach( QComboBox* comboBox, m_SpeakerCombos)
    {
        connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(speakerSettingsComboBoxValueChanged(int)));
    }

    connect(ui->radioButtonXOver50Hz, SIGNAL(clicked()) ,this, SLOT(XOver_selected()));
    connect(ui->radioButtonXOver80Hz, SIGNAL(clicked()) ,this, SLOT(XOver_selected()));
    connect(ui->radioButtonXOver100Hz, SIGNAL(clicked()) ,this, SLOT(XOver_selected()));
    connect(ui->radioButtonXOver150Hz, SIGNAL(clicked()) ,this, SLOT(XOver_selected()));
    connect(ui->radioButtonXOver200Hz, SIGNAL(clicked()) ,this, SLOT(XOver_selected()));


    QStringList responseList;
    responseList << MCACCNumberResponse_MC().getResponseID();
    //responseList << ErrorResponse_B_E().getResponseID(); //Fehler abfangen beim setzen der LSConfig
    //responseList << SpeakerSettingResponse_SSG().getResponseID();
    //responseList << SurroundPositionResponse_SSP().getResponseID();
    //responseList << SpeakerSystemRequest_SSF().getResponseID();
    //responseList << ChannelLevelResponse_CLV().getResponseID();
    //responseList << SpeakerControlResponse_SPK().getResponseID();
    responseList << SpeakerInformationResponse_SPI().getResponseID();
    responseList << SpeakerDistanceResponse_SPD().getResponseID();
    responseList << EmphasisResponse_ILV().getResponseID();
    //responseList << XOverResponse_SSQ().getResponseID();
    MsgDistributor::AddResponseListener(this, responseList);
}



LoudspeakerSettingsOnkyoDialog::~LoudspeakerSettingsOnkyoDialog()
{
    delete ui;
}


void LoudspeakerSettingsOnkyoDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("LSSettingsWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}


void LoudspeakerSettingsOnkyoDialog::ResponseReceived(ReceivedObjectBase *response)
{
    if (m_Comm.IsPioneer()) {
        return;
    }
    // mcacc number
    MCACCNumberResponse_MC* mcacc = dynamic_cast<MCACCNumberResponse_MC*>(response);
    if (mcacc != NULL)
    {
        clear_toggles();
        m_MCACCButtons[mcacc->GetMCACCNumber() - 1]->setChecked(true);
        disableControls();
        ui->groupBoxMcacc->setEnabled(true);
        requestData();
        return;
    }
//    ErrorResponse_B_E* error = dynamic_cast<ErrorResponse_B_E*>(response);
//    if (error != NULL)
//    {
//        // da keine weitere Unterscheidung hier möglich, generell setzen
//        if (errflag > 0)
//        {
//            SendCmd("?SSF");   //Wert nicht angenommen, korrigieren in DataReceived(QString data)
//            errflag--;
//        }
//        return;
//    }
//    SpeakerSettingResponse_SSG* sp_setting = dynamic_cast<SpeakerSettingResponse_SSG*>(response);
//    if (sp_setting != NULL)
//    {
//        int no = sp_setting->getSpeakerNo();
//        int setting = sp_setting->getSpeakerSetting();

//        if (no >= 0 && no < m_SpeakerSettings.size()) {
//            m_SpeakerSettings[no]->setEnabled(true);
//            m_SpeakerSettings[no]->blockSignals(true);
//            m_SpeakerSettings[no]->setCurrentIndex(setting);
//            m_SpeakerSettings[no]->blockSignals(false);
//        }
//        //ui->groupBoxSpeakerSettings->setEnabled(true);
//        if (m_RefreshSpeakerSettings) {
//            m_RefreshSpeakerSettings = false;
//        }
//        return;
//    }
//    SurroundPositionResponse_SSP* position = dynamic_cast<SurroundPositionResponse_SSP*>(response);
//    if (position != NULL)
//    {
//        ui->groupBoxSurroundPosition->setEnabled(true);
//        if (position->IsOnSide()) {
//            ui->radioButtonSurOnSide->blockSignals(true);
//            ui->radioButtonSurOnSide->setChecked(true);
//            ui->radioButtonSurOnSide->blockSignals(false);
//        } else {
//            ui->radioButtonSurBehind->blockSignals(true);
//            ui->radioButtonSurBehind->setChecked(true);
//            ui->radioButtonSurBehind->blockSignals(false);
//        }
//        return;
//    }
//    SpeakerSystemRequest_SSF* system = dynamic_cast<SpeakerSystemRequest_SSF*>(response);
//    if (system != NULL)
//    {
//        ui->groupBoxSpeakerConfiguration->setEnabled(true);
//        mCurrentSpeakerSetting = system->GetSpeakerSystem();
//        int nr = 0;
//        for(int i = 0; i < m_SpeakerConfigurations.size(); i++) {
//            if (mCurrentSpeakerSetting == m_SpeakerConfigurations[i].first) {
//                nr = i;
//                break;
//            }
//        }
//        ui->LSsystem->setCurrentIndex(nr);//aktuelle Konfig
//        return;
//    }
//    ChannelLevelResponse_CLV* level = dynamic_cast<ChannelLevelResponse_CLV*>(response);
//    if (level != NULL)
//    {
//        for (int i = 0; i < m_Sliders.size(); i++)
//        {
//            if(level->GetSpeaker() == channels[i]) {
//                mchannels[i] = level->GetLevel();
//                setslider(i, mchannels[i]);
//                m_Sliders[i]->setEnabled(true);
//            }
//        }
//        return;
//    }
//    SpeakerControlResponse_SPK* control = dynamic_cast<SpeakerControlResponse_SPK*>(response);
//    if (control != NULL)
//    {
//        ui->groupBoxSpeakerControl->setEnabled(true);
//        ui->spa->setChecked(false);
//        ui->spb->setChecked(false);
//        ui->spab->setChecked(false);
//        ui->spoff->setChecked(false);
//        switch (control->GetSpeakerSetup()) {
//        case 0:
//            ui->spoff->setChecked(true);
//            break;
//        case 1:
//            ui->spa->setChecked(true);
//            break;
//        case 2:
//            ui->spb->setChecked(true);
//            break;
//        case 3:
//            ui->spab->setChecked(true);
//            break;
//        default:
//            break;
//        }
//    }
    EmphasisResponse_ILV* tcl = dynamic_cast<EmphasisResponse_ILV*>(response);
    if (tcl != NULL)
    {
        for(int i = 0; i < tcl->GetEmphasisData().length() && i < m_Sliders.length(); i++)
        {
            m_Sliders[i]->setEnabled(true);
            setslider(i, tcl->GetEmphasisData()[i]);
        }
    }
    SpeakerInformationResponse_SPI* speaker = dynamic_cast<SpeakerInformationResponse_SPI*>(response);
    if (speaker != NULL)
    {
        m_SpeakerSettings = *speaker;
        ui->radioButtonXOver50Hz->setEnabled(true);
//        ui->radioButtonXOver80Hz->setEnabled(true);
//        ui->radioButtonXOver100Hz->setEnabled(true);
//        ui->radioButtonXOver150Hz->setEnabled(true);
//        ui->radioButtonXOver200Hz->setEnabled(true);
        ui->groupBoxXOver->setEnabled(true);
        switch(speaker->crossover) {
        case SpeakerInformationResponse_SPI::CROSSOVER_50HZ:
            ui->radioButtonXOver50Hz->setChecked(true);
            break;
        case SpeakerInformationResponse_SPI::CROSSOVER_80HZ:
            ui->radioButtonXOver80Hz->setChecked(true);
            break;
        case SpeakerInformationResponse_SPI::CROSSOVER_100HZ:
            ui->radioButtonXOver100Hz->setChecked(true);
            break;
        case SpeakerInformationResponse_SPI::CROSSOVER_150HZ:
            ui->radioButtonXOver150Hz->setChecked(true);
            break;
        case SpeakerInformationResponse_SPI::CROSSOVER_200HZ:
            ui->radioButtonXOver200Hz->setChecked(true);
            break;
        }
    }
//    XOverResponse_SSQ* xover = dynamic_cast<XOverResponse_SSQ*>(response);
//    if (xover != NULL)
//    {
//        if (xover->GetFrequency() >= 0 && xover->GetFrequency() < m_XOverButtons.size())
//        {
//            m_XOverButtons[xover->GetFrequency()]->setChecked(true);
//            ui->groupBoxXOver->setEnabled(true);
//        }
//        return;
//    }
}

void LoudspeakerSettingsOnkyoDialog::ShowLoudspeakerSettingsOnkyoDialog()
{
    if (m_Comm.IsPioneer())
    {
        return;
    }
    if (!isVisible())
    {
        disableControls();
        if (!m_PositionSet || !m_Settings.value("SaveLSSettingsWindowGeometry", false).toBool())
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
    SendCmd("SPIQSTN"); // speaker information
    SendCmd("SPDQSTN"); // speaker distance
    SendCmd("SPLQSTN"); // speaker layout
    SendCmd("MCMQSTN"); // MCACC number
    SendCmd("TCLQSTN"); // temporary channel level
    //ui->mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
    adjustSize();
    resize(minimumSizeHint());
}

void LoudspeakerSettingsOnkyoDialog::ValueChanged()
{
    QSlider* sender = dynamic_cast<QSlider*>(QObject::sender());
    if (sender == NULL)
        return;
//    for (int i = 0; i < m_Sliders.count(); i++)
//    {
//        if (sender == m_Sliders[i])
//        {
//            //SendCmd(QString("%1%2CLV").arg(channels[i]).arg(m_Sliders[i]->value()));
//        }
//    }
    emit SendCmd("TCL" + GetOnkyoChannelString());
    setslider();
}

QString LoudspeakerSettingsOnkyoDialog::GetOnkyoChannelString()
{
    QString str;
    for (int i = 0; i < m_Sliders.count(); i++)
    {
        str.append(EmphasisResponse_ILV::GetCmdData(m_Sliders[i]->value()));
    }
    return str;
}

void LoudspeakerSettingsOnkyoDialog::setslider(int idx, int value)
{
    if (idx < 0 || idx >= m_Sliders.size())
    {
        return;
    }
    if (value < 0 && value > 99)
    {
        value = 50;
    }
    m_Sliders[idx]->blockSignals(true);
    m_Sliders[idx]->setValue(value);
    m_Sliders[idx]->setSliderPosition(value);
    m_Sliders[idx]->blockSignals(false);

    m_Labels[idx]->setText(EmphasisResponse_ILV::GetDbString(value));
}

void LoudspeakerSettingsOnkyoDialog::setslider()
{
    for (int i = 0; i < m_Sliders.count(); i++)
    {
        int value = m_Sliders[i]->value();
        if (value < -0x1e && value > 0x18)
            value = 0;
        setslider(i, value);
    }
}

void LoudspeakerSettingsOnkyoDialog::checkbox()
{
    int i;
    QString str;
    QObject* sender =QObject::sender();
    str="ui->"+sender->objectName();
    i=str.mid(6,1).toInt()-1;
//      qDebug()  << "buttonchecked="  <<str <<"I=" <<i << m_buttons[i]->isChecked();
    if (m_MCACCButtons[i]->isChecked())
        {
            clear_toggles();
            if (m_Comm.IsPioneer())
            {
                str=QString("%1MC").arg(i+1);
                SendCmd(str);
            }
            else
            {
                SendCmd(QString::asprintf("MCM%02X", i + 1));
            }
            ShowLoudspeakerSettingsOnkyoDialog();
        }
        m_MCACCButtons[i]->setChecked(true);
}


void LoudspeakerSettingsOnkyoDialog::clear_toggles()
{
    for (int i = 0; i < m_MCACCButtons.length(); i++)
    m_MCACCButtons[i]->setChecked(false);
}

void LoudspeakerSettingsOnkyoDialog::on_spa_clicked()
{
    if (m_Comm.IsPioneer())
    {
        SendCmd("1SPK");
    }
}

void LoudspeakerSettingsOnkyoDialog::on_spb_clicked()
{
    if (m_Comm.IsPioneer())
    {
        SendCmd("2SPK");
    }
}

void LoudspeakerSettingsOnkyoDialog::on_spab_clicked()
{
    if (m_Comm.IsPioneer())
    {
        SendCmd("3SPK");
    }
}

void LoudspeakerSettingsOnkyoDialog::on_spoff_clicked()
{
    if (m_Comm.IsPioneer())
    {
        SendCmd("0SPK");
    }
}

void LoudspeakerSettingsOnkyoDialog::speakerSettingsComboBoxValueChanged(int index)
{
    QComboBox* sender = dynamic_cast<QComboBox*>(QObject::sender());
    if (sender == NULL)
        return;

    for(int i = 0; i < m_SpeakerCombos.size(); i++)
    {
        if (sender == m_SpeakerCombos[i]) {
            m_RefreshSpeakerSettings = true;
            SendCmd(QString("%1%2SSG").arg(i, 2, 10, QChar('0')).arg(index));
            //QThread::currentThread()->msleep(500);
            break;
        }
    }
}

void LoudspeakerSettingsOnkyoDialog::on_radioButtonSurOnSide_clicked()
{
    if (m_Comm.IsPioneer())
    {
        SendCmd("0SSP");
    }
}

void LoudspeakerSettingsOnkyoDialog::on_radioButtonSurBehind_clicked()
{
    if (m_Comm.IsPioneer())
    {
        SendCmd("1SSP");
    }
}

void LoudspeakerSettingsOnkyoDialog::enableSlider(bool enabled)
{
    foreach (QSlider* slider, m_Sliders) {
        slider->setEnabled(enabled);
    }
}

void LoudspeakerSettingsOnkyoDialog::enableSpeakerSettings(bool enabled)
{
    foreach (QComboBox* combo, m_SpeakerCombos) {
        combo->setEnabled(enabled);
    }
}

void LoudspeakerSettingsOnkyoDialog::disableControls()
{
    enableSlider(false);
    enableSpeakerSettings(false);
    ui->groupBoxXOver->setEnabled(false);
    ui->groupBoxMcacc->setEnabled(false);
    ui->groupBoxSpeakerControl->setEnabled(false);
    ui->groupBoxSurroundPosition->setEnabled(false);
    ui->groupBoxSpeakerConfiguration->setEnabled(false);
}

void LoudspeakerSettingsOnkyoDialog::requestData()
{
    if (!m_Comm.IsPioneer())
    {
        SendCmd("SPIQSTN"); // speaker information
        SendCmd("SPDQSTN"); // speaker distance
        SendCmd("SPLQSTN"); // speaker layout
        SendCmd("TCLQSTN"); // temporary channel level
    }
}

void LoudspeakerSettingsOnkyoDialog::XOver_selected()
{
    QRadioButton* sender = dynamic_cast<QRadioButton*>(QObject::sender());
    if (sender == NULL)
        return;
    QString name = sender->objectName();
    if (name == "radioButtonXOver50Hz")
    {
        m_SpeakerSettings.crossover = SpeakerInformationResponse_SPI::CROSSOVER_50HZ;
    }
    else if (name == "radioButtonXOver80Hz")
    {
        m_SpeakerSettings.crossover = SpeakerInformationResponse_SPI::CROSSOVER_80HZ;
    }
    else if (name == "radioButtonXOver100Hz")
    {
        m_SpeakerSettings.crossover = SpeakerInformationResponse_SPI::CROSSOVER_100HZ;
    }
    else if (name == "radioButtonXOver150Hz")
    {
        m_SpeakerSettings.crossover = SpeakerInformationResponse_SPI::CROSSOVER_150HZ;
    }
    else if (name == "radioButtonXOver200Hz")
    {
        m_SpeakerSettings.crossover = SpeakerInformationResponse_SPI::CROSSOVER_200HZ;
    }
    qDebug() << m_SpeakerSettings.getCmdString();
    emit SendCmd(m_SpeakerSettings.getCmdString());
    emit SendCmd("SPIQSTN");
//    if (sender->objectName())
//    for (int i = 0; i < m_XOverButtons.size(); i++)
//    {
//        if (sender == m_XOverButtons[i])
//        {
//            SendCmd(QString("%1SSQ").arg(i));
//            break;
//        }
//    }
}

void LoudspeakerSettingsOnkyoDialog::initSpeakerConfiguration()
{
    bool x922;
    x922=m_Settings.value("TunerCompatibilityMode").toBool();

    m_SpeakerConfigurations.clear();
    ui->LSsystem->clear();

    addSpkConf("00", "Normal(SB/FH)");
    addSpkConf("01", "Normal(SB/FW)");
    addSpkConf("02", "Speaker B");
    addSpkConf("03", "Front Bi-Amp");
    addSpkConf("04", "Zone 2");
    if (!x922) { //Im Kompatmodus nur verfügbare Typen anzeigen
        addSpkConf("05", "HD Zone");
        addSpkConf("07", "5.2");
        addSpkConf("08", "Front Bi-Amp (for 5ch model)");
        addSpkConf("09", "Speaker B (for 5ch model)");
        addSpkConf("10", "9.1 FH/FW");
        addSpkConf("11", "7.1 + Speaker B");
        addSpkConf("12", "7.1 Front Bi-Amp");
        addSpkConf("13", "7.1 + Zone 2");
        addSpkConf("14", "7.1 FH/FW + Zone 2");
        addSpkConf("15", "5.1 Bi-Amp + Zone 2");
        addSpkConf("16", "5.1 + Zone 2+3");
        addSpkConf("17", "5.1 + SP-B Bi-Amp");
        addSpkConf("18", "5.1 F+Surr Bi-Amp");
        addSpkConf("19", "5.1 F+C Bi-Amp");
        addSpkConf("20", "5.1 C+Surr Bi-Amp");
        addSpkConf("21", "Multi-Zone Music");
        addSpkConf("22", "7.2.2 TMd/FW (not use)");
        addSpkConf("23", "7.2.2 TMd/FH (not use)");
        addSpkConf("24", "5.2.4 (not use)");
        addSpkConf("25", "5.2 Zone 2 + HD Zone");
        addSpkConf("26", "7.2.2/5.2.2/7.2");
        addSpkConf("27", "7.2.2 Front Bi-Amp");
        addSpkConf("30", "9.2.2 TMd/FH");
        addSpkConf("31", "7.2.4 SB Pre out");
        addSpkConf("32", "7.2.4 Front Pre out");
    }

    for (int i = 0; i < m_SpeakerConfigurations.size(); i++) {
        ui->LSsystem->addItem(m_SpeakerConfigurations[i].second);

    }
//    for (int i = 0; i < m_SpeakerConfigurations.size(); i++) //LS-Konfig belegen
//        ui->LSsystem->addItem(m_SpeakerConfigurations.[i]);


}

void LoudspeakerSettingsOnkyoDialog::addSpkConf(const QString& nr, const QString& descr)
{
    m_SpeakerConfigurations.append(QPair<QString, QString>(nr, descr));
}

void LoudspeakerSettingsOnkyoDialog::on_LSsystem_currentIndexChanged(int index)
{
    QString str;
    QString nr = m_SpeakerConfigurations[index].first;
    //errflag = 0;
    if (nr != mCurrentSpeakerSetting)
    {
        if (m_Comm.IsPioneer())
        {
            str = QString("%1SSF").arg(nr);
            SendCmd(str);
            mCurrentSpeakerSetting = nr;
            //errflag = 1;
        }
    }

}
