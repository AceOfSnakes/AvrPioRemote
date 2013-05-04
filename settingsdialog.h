#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QWidget *parent, QSettings& settings);
    ~SettingsDialog();

public slots:
    void ShowSettingsDialog();

private slots:
    void on_TunerVSX922CompatibilityModeCheckBox_stateChanged(int state);

    void on_FavoriteLX83CompatibilityModeCheckBox_stateChanged(int state);

private:
    QSettings&          m_Settings;
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
