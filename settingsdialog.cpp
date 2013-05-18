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
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent, QSettings& settings) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->TunerVSX922CompatibilityModeCheckBox->setChecked(m_Settings.value("TunerCompatibilityMode", false).toBool());
    ui->FavoriteLX83CompatibilityModeCheckBox->setChecked(m_Settings.value("FavoritesCompatibilityMode", false).toBool());
    // vorerst auf eis gelegt
    m_Settings.setValue("FavoritesCompatibilityMode", false);
    ui->FavoriteLX83CompatibilityModeCheckBox->setVisible(false);

    //this->setFixedSize(this->size());
    this->setModal(true);

    QString lang = m_Settings.value("Language", "auto").toString();
    if (lang == "auto")
    {
        ui->LanguageAutoRadioButton->setChecked(true);
    }
    else if (lang.startsWith("de"))
    {
        ui->LanguageGermanRadioButton->setChecked(true);
    }
    else
    {
        ui->LanguageEnglishRadioButton->setChecked(true);
    }

    ui->ShowNetRadioCheckBox->setChecked(m_Settings.value("AutoShowNetRadio", true).toBool());
    ui->ShowTunerCheckBox->setChecked(m_Settings.value("AutoShowTuner", true).toBool());
    ui->ShowTunerCheckBox->setChecked(m_Settings.value("AutoShowUSB", true).toBool());
    SetLanguage();

    ui->StartLoggingInTestWindowCheckBox->setChecked(m_Settings.value("StartLoggingInTestWindow", false).toBool());

    ui->RestoreMainWindowCheckBox->setChecked(m_Settings.value("SaveMainWindowGeometry", true).toBool());
    ui->RestoreEQWindowCheckBox->setChecked(m_Settings.value("SaveEQWindowGeometry", false).toBool());
    ui->RestoreTunerWindowCheckBox->setChecked(m_Settings.value("SaveTunerWindowGeometry", false).toBool());
    ui->RestoreNetRadioWindowCheckBox->setChecked(m_Settings.value("SaveNetRadioWindowGeometry", false).toBool());
    ui->RestoreSpeakerSettingsWindowCheckBox->setChecked(m_Settings.value("SaveLSSettingsWindowGeometry", false).toBool());
    ui->RestoreTestWindowCheckBox->setChecked(m_Settings.value("SaveTestWindowGeometry", false).toBool());
    ui->RestoreLMSettingsWindowCheckBox->setChecked(m_Settings.value("SaveLMSettingsWindowGeometry", false).toBool());
    ui->RestoreUSBWindowCheckBox->setChecked(m_Settings.value("SaveUsbWindowGeometry", false).toBool());

    ui->ShowReceiverNameInTitleCheckBox->setChecked(m_Settings.value("ShowReceiverNameInTitle", true).toBool());

    ui->ShowDefaultInputNameCheckBox->setChecked(m_Settings.value("ShowDefaultInputName", false).toBool());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}


void SettingsDialog::ShowSettingsDialog()
{
    if (!this->isVisible())
    {
//        QWidget* Parent = dynamic_cast<QWidget*>(parent());
//        int x = this->pos().x() + this->width() + 20;
//        QPoint pos;
//        pos.setX(x);
//        pos.setY(this->pos().y());
//        m_SettingsDialog->move(pos);
        this->show();
    }
}


void SettingsDialog::on_TunerVSX922CompatibilityModeCheckBox_stateChanged(int state)
{
    if (Qt::Checked == state)
    {
        m_Settings.setValue("TunerCompatibilityMode", true);
    }
    else
    {
        m_Settings.setValue("TunerCompatibilityMode", false);
    }
}


void SettingsDialog::on_FavoriteLX83CompatibilityModeCheckBox_stateChanged(int state)
{
    if (Qt::Checked == state)
    {
        m_Settings.setValue("FavoritesCompatibilityMode", true);
    }
    else
    {
        m_Settings.setValue("FavoritesCompatibilityMode", false);
    }
}


void SettingsDialog::on_LanguageAutoRadioButton_clicked(bool checked)
{
    if (checked)
    {
        m_Settings.setValue("Language", "auto");
        SetLanguage();
    }
}


void SettingsDialog::on_LanguageEnglishRadioButton_clicked(bool checked)
{
    if (checked)
    {
        m_Settings.setValue("Language", "en");
        SetLanguage();
    }
}


void SettingsDialog::on_LanguageGermanRadioButton_clicked(bool checked)
{
    if (checked)
    {
        m_Settings.setValue("Language", "de");
        SetLanguage();
    }
}


void SettingsDialog::SetLanguage()
{
/*    QString lang = m_Settings.value("Language", "auto").toString();
    if (lang == "auto")
    {
        lang = QLocale::system().name();
    }
    if (lang.startsWith("de"))
    {
        m_Translater.load("avrpioremote_de");
    }
    else
    {
        m_Translater.load("avrpioremote_en");
    }
    QCoreApplication::installTranslator(&m_Translater);*/
}


void SettingsDialog::on_pushButton_clicked()
{
    close();
}


void SettingsDialog::on_ShowTunerCheckBox_clicked()
{
    m_Settings.setValue("AutoShowTuner", ui->ShowTunerCheckBox->isChecked());
}

void SettingsDialog::on_ShowNetRadioCheckBox_clicked()
{
    m_Settings.setValue("AutoShowNetRadio", ui->ShowNetRadioCheckBox->isChecked());
}

void SettingsDialog::on_RestoreMainWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveMainWindowGeometry", ui->RestoreMainWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreEQWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveEQWindowGeometry", ui->RestoreEQWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreTunerWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveTunerWindowGeometry", ui->RestoreTunerWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreNetRadioWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveNetRadioWindowGeometry", ui->RestoreNetRadioWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreSpeakerSettingsWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveLSSettingsWindowGeometry", ui->RestoreSpeakerSettingsWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreTestWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveTestWindowGeometry", ui->RestoreTestWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreLMSettingsWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveLMSettingsWindowGeometry", ui->RestoreLMSettingsWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreUSBWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveUsbWindowGeometry", ui->RestoreUSBWindowCheckBox->isChecked());
}

void SettingsDialog::on_ShowUSBCheckBox_clicked()
{
    m_Settings.setValue("AutoShowUSB", ui->ShowUSBCheckBox->isChecked());
}

void SettingsDialog::on_StartLoggingInTestWindowCheckBox_clicked()
{
    m_Settings.setValue("StartLoggingInTestWindow", ui->StartLoggingInTestWindowCheckBox->isChecked());
}

void SettingsDialog::on_ShowReceiverNameInTitleCheckBox_clicked()
{
    m_Settings.setValue("ShowReceiverNameInTitle", ui->ShowReceiverNameInTitleCheckBox->isChecked());
}

void SettingsDialog::on_ShowDefaultInputNameCheckBox_clicked()
{
    m_Settings.setValue("ShowDefaultInputName", ui->ShowDefaultInputNameCheckBox->isChecked());
}
