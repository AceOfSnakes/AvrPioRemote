#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QTranslator>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QWidget *parent, QSettings& settings);
    ~SettingsDialog();

    void SetLanguage();

public slots:
    void ShowSettingsDialog();

private slots:
    void on_TunerVSX922CompatibilityModeCheckBox_stateChanged(int state);
    void on_FavoriteLX83CompatibilityModeCheckBox_stateChanged(int state);
    void on_LanguageAutoRadioButton_clicked(bool checked);
    void on_LanguageEnglishRadioButton_clicked(bool checked);
    void on_LanguageGermanRadioButton_clicked(bool checked);
    void on_pushButton_clicked();
    void on_ShowTunerCheckBox_clicked();
    void on_ShowNetRadioCheckBox_clicked();
    void on_RestoreMainWindowCheckBox_clicked();
    void on_RestoreEQWindowCheckBox_clicked();
    void on_RestoreTunerWindowCheckBox_clicked();
    void on_RestoreNetRadioWindowCheckBox_clicked();
    void on_RestoreSpeakerSettingsWindowCheckBox_clicked();
    void on_RestoreTestWindowCheckBox_clicked();
    void on_RestoreLMSettingsWindowCheckBox_clicked();
    void on_RestoreUSBWindowCheckBox_clicked();
    void on_ShowUSBCheckBox_clicked();
    void on_StartLoggingInTestWindowCheckBox_clicked();
    void on_ShowReceiverNameInTitleCheckBox_clicked();

private:
    QSettings&          m_Settings;
    Ui::SettingsDialog *ui;
    QTranslator         m_Translater;
};

#endif // SETTINGSDIALOG_H
