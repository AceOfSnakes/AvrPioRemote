#ifndef TUNERDIALOG_H
#define TUNERDIALOG_H

#include <QDialog>
#include <vector>
#include <QSettings>
#include "actionwithparameter.h"

namespace Ui {
class TunerDialog;
}

class TunerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TunerDialog(QWidget *parent, QSettings& settings);
    ~TunerDialog();
    
private:
    QSettings&                  m_Settings;
    Ui::TunerDialog *           ui;
    QList<QPushButton*>         m_ClassButtons;
    QList<QPushButton*>         m_PresetButtons;
    QString                     m_SelectedClassNo;
    QString                     m_SelectedPresetNo;
    int                         m_TunerFrequency;
    bool                        m_CompatibilityMode;
    QString                     m_TempPresetName;
    QList<QAction*>             m_PresetActions;

public slots:
    void ShowTunerDialog();
    void DataReceived(QString data);
    void SelectClassButton(int idx);
    void SelectPresetButton(int idx);
    void PresetSelected(QString Param);
signals:
    void SendCmd(QString);
private slots:
    void ClassButtonClicked(QString Param);
    void PresetButtonClicked(QString Param);
    void EnableControls(bool enable);
    void on_PresetPlusButton_clicked();
    void on_PresetMinusButton_clicked();
    void on_AMButton_clicked();
    void on_FMButton_clicked();
    void on_FrequencyMinusButton_clicked();
    void on_FrequencyPlusButton_clicked();
    void on_ChoosePresetButton_clicked();
    void on_DisplayButton_clicked();
    void on_PTYSearchButton_clicked();
    void on_NoiseCutButton_clicked();
    void on_EditFrequencyButton_clicked();
    void on_SaveButton_clicked();
    void on_CancelButton_clicked();
    void on_OkButton_clicked();
    void on_RenamePresetButton_clicked();
};

#endif // TUNERDIALOG_H
