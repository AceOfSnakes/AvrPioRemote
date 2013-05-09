#ifndef TUNERDIALOG_H
#define TUNERDIALOG_H

#include <QDialog>
#include <vector>
#include <QSettings>
#include "actionwithparameter.h"
#include "receiverinterface.h"
#include <QMoveEvent>

namespace Ui {
class TunerDialog;
}

class TunerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TunerDialog(QWidget *parent, ReceiverInterface& Comm, QSettings& settings);
    ~TunerDialog();
    
private:
    QSettings&                  m_Settings;
    Ui::TunerDialog *           ui;
    ReceiverInterface&          m_Comm;
    QList<QPushButton*>         m_ClassButtons;
    QList<QPushButton*>         m_PresetButtons;
    QString                     m_SelectedClassNo;
    QString                     m_SelectedPresetNo;
    int                         m_TunerFrequency;
    bool                        m_CompatibilityMode;
    QString                     m_TempPresetName;
    QList<QAction*>             m_PresetActions;
    QString                     m_DisplayString;
    bool                        m_TunerInputSelected;
    bool                        m_PositionSet;

    bool GetScrolledString(QString input);

    void moveEvent(QMoveEvent*event);

public slots:
    void ShowTunerDialog();
    void DataReceived(QString data);
    void SelectClassButton(int idx);
    void SelectPresetButton(int idx);
    void PresetSelected(QString Param);
    void DisplayData(int no, QString str);
    void InputChanged(int no, QString name);
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
