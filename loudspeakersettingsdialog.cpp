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
#include "loudspeakersettingsdialog.h"
#include "ui_loudspeakersettingsdialog.h"
#include <QDebug>
#include "receiver_interface/receiverinterface.h"
#include <QThread>


const char* LStyp[] = {
    "Normal(SB/FH)", //0
    "Normal(SB/FW)",
    "Speaker B",
    "Front Bi-Amp",
    "Zone 2",
    "9.1ch FH/FW",
    "7.1ch+Speaker B",
    "7.1ch Front Bi-Amp",
    "7.1ch+ZONE2",
    "7.1ch FH/FW+ZONE2",
    "5.1ch Bi-Amp+ZONE2",
    "5.1ch+ZONE 2+3",
    "5.1ch+SP-B Bi-Amp",
    "5.1ch F+Surr Bi-Amp",
    "5.1ch F+C Bi-Amp", //14
    "5.1ch C+Surr Bi-Amp",
    };

const int LSwert[] = {    //codewert des LSTyps im AVR
    0,1,2,3,4,10,11,12,13,14,15,16,17,18,19,20,
    };

const char* channels[]={
    "L__",
    "R__",
    "C__",
    "SL_",
    "SR_",
    "SBL",
    "SBR",
    "SW_",
    "LH_",
    "RH_",
    "LW_",
    "RW_",
};

LoudspeakerSettingsDialog::LoudspeakerSettingsDialog(QWidget *parent, QSettings &settings,ReceiverInterface &Comm ) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::LoudspeakerSettingsDialog),
    m_Comm(Comm),
    m_PositionSet(false),
    m_RefreshSpeakerSettings(false)
{
    errflag=0;
    bool x922;
    int lsanz=14;

    ui->setupUi(this);
    //this->setFixedSize(this->size());

    // restore the position of the window
    if (m_Settings.value("SaveLSSettingsWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("LSSettingsWindowGeometry").toByteArray());
    }

    // communication
//    connect(&m_Comm, SIGNAL(DataReceived(QString)), this, SLOT(SpeakerReceived(QString)));
    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));

    // save sliders in a list
    m_Sliders.append(ui->sfl);
    m_Sliders.append(ui->sfr);
    m_Sliders.append(ui->sc);
    m_Sliders.append(ui->ssl);
    m_Sliders.append(ui->ssr);
    m_Sliders.append(ui->ssbl);
    m_Sliders.append(ui->ssbr);
    m_Sliders.append(ui->ssw);
    m_Sliders.append(ui->sfhl);
    m_Sliders.append(ui->sfhr);
    m_Sliders.append(ui->sfwl);
    m_Sliders.append(ui->sfwr);
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

    m_MCACCButtons.append(ui->mc1);
    m_MCACCButtons.append(ui->mc2);
    m_MCACCButtons.append(ui->mc3);
    m_MCACCButtons.append(ui->mc4);
    m_MCACCButtons.append(ui->mc5);
    m_MCACCButtons.append(ui->mc6);
    foreach (QCheckBox* checkBox, m_MCACCButtons) {
        connect(checkBox, SIGNAL(clicked()), this, SLOT(checkbox()));
    }

    x922=m_Settings.value("TunerCompatibilityMode").toBool();
    if (x922)   //Im Kompatmodus nur verfügbare Typen anzeigen
        lsanz=5;

    for(int i=0;i<lsanz;i++) //LS-Konfig belegen
        ui->LSsystem->addItem(LStyp[i]);

    for (int i=0;i<12;i++)
        mchannels[i]=50;

    QStringList list;
    list << "Small" << "Large";
    ui->comboBoxFrontSeting->addItems(list);

    list << "No";
    ui->comboBoxCenterSetting->addItems(list);
    ui->comboBoxFrontHeightSetting->addItems(list);
    ui->comboBoxFrontWideSetting->addItems(list);
    ui->comboBoxSurroundSetting->addItems(list);

    list.clear();
    list << "Small * 2" << "Large * 1" << "Large * 2" << "No" << "Small * 1";
    ui->comboBoxSurroundBackSetting->addItems(list);

    list.clear();
    list << "Yes" << "Plus" << "No";
    ui->comboBoxSubwooferSetting->addItems(list);

    QStringList mstr1;
    mstr1 << "Memory 1"  << "Memory 2" << "Memory 3" << "Memory 4" << "Memory 5" << "Memory 6";
    ui->selectmem->addItems(mstr1);

    m_SpeakerSettings << ui->comboBoxFrontSeting;
    m_SpeakerSettings << ui->comboBoxCenterSetting;
    m_SpeakerSettings << ui->comboBoxFrontHeightSetting;
    m_SpeakerSettings << ui->comboBoxFrontWideSetting;
    m_SpeakerSettings << ui->comboBoxSurroundSetting;
    m_SpeakerSettings << ui->comboBoxSurroundBackSetting;
    m_SpeakerSettings << ui->comboBoxSubwooferSetting;
    foreach( QComboBox* comboBox, m_SpeakerSettings)
    {
        connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(speakerSettingsComboBoxValueChanged(int)));
    }

    m_XOverButtons << ui->radioButtonXOver50Hz;
    m_XOverButtons << ui->radioButtonXOver80Hz;
    m_XOverButtons << ui->radioButtonXOver100Hz;
    m_XOverButtons << ui->radioButtonXOver150Hz;
    m_XOverButtons << ui->radioButtonXOver200Hz;
    foreach( QRadioButton* radio, m_XOverButtons)
    {
        connect(radio, SIGNAL(clicked()) ,this, SLOT(XOver_selected()));
    }


    QStringList responseList;
    responseList << MCACCNumberResponse_MC().getResponseID();
    responseList << ErrorResponse_B_E().getResponseID(); //Fehler abfangen beim setzen der LSConfig
    responseList << SpeakerSettingResponse_SSG().getResponseID();
    responseList << SurroundPositionResponse_SSP().getResponseID();
    responseList << SpeakerSystemRequest_SSF().getResponseID();
    responseList << ChannelLevelResponse_CLV().getResponseID();
    responseList << SpeakerControlResponse_SPK().getResponseID();
    responseList << XOverResponse_SSQ().getResponseID();
    MsgDistributor::AddResponseListener(this, responseList);
}



LoudspeakerSettingsDialog::~LoudspeakerSettingsDialog()
{
    delete ui;
}


void LoudspeakerSettingsDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("LSSettingsWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}


void LoudspeakerSettingsDialog::ResponseReceived(ReceivedObjectBase *response)
{
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
    ErrorResponse_B_E* error = dynamic_cast<ErrorResponse_B_E*>(response);
    if (error != NULL)
    {
        // da keine weitere Unterscheidung hier möglich, generell setzen
        if (errflag > 0)
        {
            SendCmd("?SSF");   //Wert nicht angenommen, korrigieren in DataReceived(QString data)
            errflag--;
        }
        return;
    }
    SpeakerSettingResponse_SSG* sp_setting = dynamic_cast<SpeakerSettingResponse_SSG*>(response);
    if (sp_setting != NULL)
    {
        int no = sp_setting->getSpeakerNo();
        int setting = sp_setting->getSpeakerSetting();

        if (no >= 0 && no < m_SpeakerSettings.size()) {
            m_SpeakerSettings[no]->setEnabled(true);
            m_SpeakerSettings[no]->blockSignals(true);
            m_SpeakerSettings[no]->setCurrentIndex(setting);
            m_SpeakerSettings[no]->blockSignals(false);
        }
        //ui->groupBoxSpeakerSettings->setEnabled(true);
        if (m_RefreshSpeakerSettings) {
            m_RefreshSpeakerSettings = false;
            requestSpeakerSettings();
        }
        return;
    }
    SurroundPositionResponse_SSP* position = dynamic_cast<SurroundPositionResponse_SSP*>(response);
    if (position != NULL)
    {
        ui->groupBoxSurroundPosition->setEnabled(true);
        if (position->IsOnSide()) {
            ui->radioButtonSurOnSide->blockSignals(true);
            ui->radioButtonSurOnSide->setChecked(true);
            ui->radioButtonSurOnSide->blockSignals(false);
        } else {
            ui->radioButtonSurBehind->blockSignals(true);
            ui->radioButtonSurBehind->setChecked(true);
            ui->radioButtonSurBehind->blockSignals(false);
        }
        return;
    }
    SpeakerSystemRequest_SSF* system = dynamic_cast<SpeakerSystemRequest_SSF*>(response);
    if (system != NULL)
    {
        ui->groupBoxSpeakerConfiguration->setEnabled(true);
        ui->LSsystem->setCurrentIndex(system->GetSpeakerSystem());//aktuelle Konfig
        mVal = system->GetSpeakerSystem();  //in public speichern zum Vergleich beim setzen
        return;
    }
    ChannelLevelResponse_CLV* level = dynamic_cast<ChannelLevelResponse_CLV*>(response);
    if (level != NULL)
    {
        for (int i = 0; i < m_Sliders.size(); i++)
        {
            if(level->GetSpeaker() == channels[i]) {
                mchannels[i] = level->GetLevel();
                setslider(i, mchannels[i]);
                m_Sliders[i]->setEnabled(true);
            }
        }
        return;
    }
    SpeakerControlResponse_SPK* control = dynamic_cast<SpeakerControlResponse_SPK*>(response);
    if (control != NULL)
    {
        ui->groupBoxSpeakerControl->setEnabled(true);
        ui->spa->setChecked(false);
        ui->spb->setChecked(false);
        ui->spab->setChecked(false);
        ui->spoff->setChecked(false);
        switch (control->GetSpeakerSetup()) {
        case 0:
            ui->spoff->setChecked(true);
            break;
        case 1:
            ui->spa->setChecked(true);
            break;
        case 2:
            ui->spb->setChecked(true);
            break;
        case 3:
            ui->spab->setChecked(true);
            break;
        default:
            break;
        }
    }
    XOverResponse_SSQ* xover = dynamic_cast<XOverResponse_SSQ*>(response);
    if (xover != NULL)
    {
        if (xover->GetFrequency() >= 0 && xover->GetFrequency() < m_XOverButtons.size())
        {
            m_XOverButtons[xover->GetFrequency()]->setChecked(true);
            ui->groupBoxXOver->setEnabled(true);
        }
        return;
    }
}

void LoudspeakerSettingsDialog::ShowLoudspeakerSettingsDialog()
{
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
    SendCmd("?MC");
}


void LoudspeakerSettingsDialog::on_SetBut_clicked()
{
    QString str;
    int i=LSwert[ui->LSsystem->currentIndex()];
    errflag=0;
    if (i != mVal)
    {
      str = QString("%1SSF").arg(i);
      if (str.size()<5)
          str="0"+str;
      SendCmd(str);
      mVal=i;
      errflag=1;
    }
}


void LoudspeakerSettingsDialog::on_savebutt_clicked()
{   //Channel-Level aus public-Speicher in Memory x sichern, gem. Auswahl Combobox,
    QString str;
    int str1;
    str1 = m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever

    for (int i = 0; i< 12; i++)
    {
        str = QString("mem%1-%2/%3").arg(ui->selectmem->currentIndex()).arg(str1).arg(channels[i]);
          m_Settings.setValue(str,mchannels[i]);
    }

    str = QString("mem%1-%2/LSCONFIG").arg(ui->selectmem->currentIndex()).arg(str1);
    m_Settings.setValue(str, mVal);
    for (int i = 0; i < m_SpeakerSettings.size(); i++)
    {
          str = QString("mem%1-%2/%3").arg(ui->selectmem->currentIndex()).arg(str1).arg(i, 2, 10, QChar('0'));
          m_Settings.setValue(str, m_SpeakerSettings[i]->currentIndex());
    }
    str = QString("mem%1-%2/LSset").arg(ui->selectmem->currentIndex()).arg(str1);
     m_Settings.setValue(str,ui->meminf->text());

/*     for (int i=0;i<6;i++) //mcacc nicht speichern, da LS-Settings dort auch drin sind
     {
         if (m_buttons[i]->isChecked())
         {
             str=QString("mem%1-%2/mcacc").arg(ui->selectmem->currentIndex()).arg(str1);
              m_Settings.setValue(str,i);
              break;
         }
     }*/
}


void LoudspeakerSettingsDialog::on_restbutt_clicked()
{   //Channel-Level aus Speicher 1-5 zurücklesen und public speichern
    int j;
    QString str;
    int str1;
    str1 = m_Settings.value("IP/4").toInt();//letztes Oktett IP anhängen, falls mehrere Reciever

/*    ui->speaker->setCurrentIndex(0);  //Speicherung entfernt, da LS-Settings auch in mcacc gespeichert werden--Konflikte
    ui->speakermode->setCurrentIndex(mLSpaar[0]);
    str=QString("mem%1-%2/mcacc").arg(ui->selectmem->currentIndex()).arg(str1);
    str1=m_Settings.value(str).toInt();   //Nummer des Buttons aus der m_button Liste (0-5)
    if (!m_buttons[str1]->isChecked())
        m_buttons[str1]->click();
*/
//    str1=m_Settings.value("IP/4").toInt();//letztes Oktett IP anhängen, falls mehrere Reciever

    for (int i = 0; i < 12; i++)
    {
          str = QString("mem%1-%2/%3").arg(ui->selectmem->currentIndex()).arg(str1).arg(channels[i]);
          j = m_Settings.value(str).toInt();
          if (j != 0)
          {
              mchannels[i] = j;
              str = (QString("%1%2CLV").arg(channels[i]).arg(j));
              //qDebug() <<str;
              SendCmd(str);
          }
          setslider();
    }
    str = QString("mem%1-%2/LSCONFIG").arg(ui->selectmem->currentIndex()).arg(str1);
    mVal = m_Settings.value(str).toInt();
    str = QString("%1SSF").arg(mVal);
    if (str.size() < 5)
        str = "0" + str;
    SendCmd(str);

    for (int i = 0; i < m_SpeakerSettings.size(); i++)
    {
          str = QString("mem%1-%2/%3").arg(ui->selectmem->currentIndex()).arg(str1).arg(i, 2, 10, QChar('0'));
          int idx = m_Settings.value(str).toInt();
          m_SpeakerSettings[i]->setCurrentIndex(idx);
//          str = QString("%1%2SSG").arg(i, 2, 10, QChar('0')).arg(idx);
//          SendCmd(str);
    }

 }



void LoudspeakerSettingsDialog::ValueChanged()
{
    QSlider* sender = dynamic_cast<QSlider*>(QObject::sender());
    if (sender == NULL)
        return;
    for (int i = 0; i < m_Sliders.count(); i++)
    {
        if (sender == m_Sliders[i])
        {
            SendCmd(QString("%1%2CLV").arg(channels[i]).arg(m_Sliders[i]->value()));
        }
    }
    setslider();
}


void LoudspeakerSettingsDialog::setslider(int idx, int value)
{
    QString str;
    double j;
    if (idx < 0 || idx >= 12)
        return;
    if (value <= 25 && value >= 75)
        value = 50;
    m_Sliders[idx]->blockSignals(true);
    m_Sliders[idx]->setValue(value);
    m_Sliders[idx]->setSliderPosition(value);
    m_Sliders[idx]->blockSignals(false);
    j = (double) value;
    j = j / 2 - 25;
    str = QString("%1").arg(j, 0, 'f', 1);
    m_Labels[idx]->setText(str);
}

void LoudspeakerSettingsDialog::setslider()
{
    for (int i = 0; i < m_Sliders.count(); i++)
    {
        if (mchannels[i] <= 25 && mchannels[i] >= 75)
            mchannels[i] = 50;
        setslider(i, mchannels[i]);
    }
}

void LoudspeakerSettingsDialog::on_selectmem_currentIndexChanged(int index)
{
    QString str;
    int str1;
    str1=m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever
    str=QString("mem%1-%2/LSset").arg(index).arg(str1);
    str=m_Settings.value(str).toString();
    ui->meminf->setText(str);
}



void LoudspeakerSettingsDialog::checkbox()
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
            str=QString("%1MC").arg(i+1);
            SendCmd(str);
            ShowLoudspeakerSettingsDialog();
        }
        m_MCACCButtons[i]->setChecked(true);
}


void LoudspeakerSettingsDialog::clear_toggles()
{
    for (int i=0;i<6;i++)
    m_MCACCButtons[i]->setChecked(false);
}

void LoudspeakerSettingsDialog::on_spa_clicked()
{
        SendCmd("1SPK");
}

void LoudspeakerSettingsDialog::on_spb_clicked()
{
        SendCmd("2SPK");
}

void LoudspeakerSettingsDialog::on_spab_clicked()
{
        SendCmd("3SPK");
}

void LoudspeakerSettingsDialog::on_spoff_clicked()
{
        SendCmd("0SPK");
}

void LoudspeakerSettingsDialog::speakerSettingsComboBoxValueChanged(int index)
{
    QComboBox* sender = dynamic_cast<QComboBox*>(QObject::sender());
    if (sender == NULL)
        return;

    for(int i = 0; i < m_SpeakerSettings.size(); i++)
    {
        if (sender == m_SpeakerSettings[i]) {
            m_RefreshSpeakerSettings = true;
            SendCmd(QString("%1%2SSG").arg(i, 2, 10, QChar('0')).arg(index));
            //QThread::currentThread()->msleep(500);
            //requestSpeakerSettings();
            break;
        }
    }
}

void LoudspeakerSettingsDialog::requestSpeakerSettings()
{
    for (int i = 0; i < m_SpeakerSettings.size(); i++)
    {
        SendCmd(QString("?SSG%1").arg(i, 2, 10, QChar('0')));
    }
}

void LoudspeakerSettingsDialog::on_radioButtonSurOnSide_clicked()
{
    SendCmd("0SSP");
}

void LoudspeakerSettingsDialog::on_radioButtonSurBehind_clicked()
{
    SendCmd("1SSP");
}

void LoudspeakerSettingsDialog::enableSlider(bool enabled)
{
    foreach (QSlider* slider, m_Sliders) {
        slider->setEnabled(enabled);
    }
}

void LoudspeakerSettingsDialog::enableSpeakerSettings(bool enabled)
{
    foreach (QComboBox* combo, m_SpeakerSettings) {
        combo->setEnabled(enabled);
    }
}

void LoudspeakerSettingsDialog::disableControls()
{
    enableSlider(false);
    enableSpeakerSettings(false);
    ui->groupBoxXOver->setEnabled(false);
    ui->groupBoxMcacc->setEnabled(false);
    ui->groupBoxSpeakerControl->setEnabled(false);
    ui->groupBoxSurroundPosition->setEnabled(false);
    ui->groupBoxSpeakerConfiguration->setEnabled(false);
}

void LoudspeakerSettingsDialog::requestData()
{
    SendCmd("?SSF");
    requestSpeakerSettings();
// hier aktuellen Channel Level anfordern

    for (int i = 0; i < 12; i++)
    {
        SendCmd(QString("?%1CLV").arg(channels[i]));
    }
    SendCmd("?SPK");
    SendCmd("?SSP");
    SendCmd("?SSQ");
}

void LoudspeakerSettingsDialog::XOver_selected()
{
    QRadioButton* sender = dynamic_cast<QRadioButton*>(QObject::sender());
    if (sender == NULL)
        return;
    for (int i = 0; i < m_XOverButtons.size(); i++)
    {
        if (sender == m_XOverButtons[i])
        {
            SendCmd(QString("%1SSQ").arg(i));
            break;
        }
    }
}
