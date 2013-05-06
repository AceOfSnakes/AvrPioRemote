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
    this->setFixedSize(this->size());
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
    SetLanguage();
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
