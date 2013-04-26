#ifndef TUNERDIALOG_H
#define TUNERDIALOG_H

#include <QDialog>
#include <vector>

namespace Ui {
class TunerDialog;
}

class TunerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TunerDialog(QWidget *parent = 0);
    ~TunerDialog();
    
private:
    Ui::TunerDialog *ui;
    std::vector<QPushButton*>   m_ClassButtons;
    std::vector<QPushButton*>   m_PresetButtons;
    QString m_SelectedClassNo;
    QString m_SelectedPresetNo;
    int     m_TunerFrequency;

public slots:
    void ShowTunerDialog();
    void DataReceived(QString data);
    void SelectClassButton(int idx);
    void SelectPresetButton(int idx);
signals:
    void SendCmd(QString);
private slots:
    void ClassButtonClicked(QString Param);
    void PresetButtonClicked(QString Param);
    void on_PresetPlusButton_clicked();
    void on_PresetMinusButton_clicked();
    void on_AMButton_clicked();
    void on_FMButton_clicked();
    void on_FrequencyMinusButton_clicked();
    void on_FrequencyPlusButton_clicked();
    void on_ChoosePresetButton_clicked();
    void on_RenamePresetApplayButton_clicked();
};

#endif // TUNERDIALOG_H
