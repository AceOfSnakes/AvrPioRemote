#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, QSettings& settings) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->TunerVSX922CompatibilityModeCheckBox->setChecked(m_Settings.value("TunerCompatibilityMode", false).toBool());
    ui->FavoriteLX83CompatibilityModeCheckBox->setChecked(m_Settings.value("FavoritesCompatibilityMode", false).toBool());
    this->setFixedSize(this->size());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
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
