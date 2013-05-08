#ifndef EQDIALOG_H
#define EQDIALOG_H

#include <QDialog>
#include "receiverinterface.h"
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <QPushButton>

namespace Ui {
class EQDialog;
}


class EQPreset
{
public:
    QString         m_Name;
    QPushButton*    m_Button;
    int             m_Values[9];
};

class EQDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EQDialog(QWidget *parent, ReceiverInterface& Comm, QSettings& settings);
    ~EQDialog();
    
private:
    Ui::EQDialog *ui;
    ReceiverInterface&  m_Comm;
    QList<QSlider*>     m_Sliders;
    QList<QLabel*>      m_Labels;
    QTimer              m_Timer;
    EQPreset            m_EQPresets[4];
    QSettings&          m_Settings;
    int                 m_SelectedPreset;

    bool ReadFile(const QString& fileName);
    bool SaveFile(const QString& fileName);
    void ResetEQPresets();
    void SelectPreset(int preset);
public slots:
    void ShowEQDialog();
    void DataReceived(QString data);
signals:
    void SendCmd(QString);
private slots:
    void Timeout();
    void OnSliderValueChanged(int value);
//    void OnSliderPressed();
//    void OnSliderReleased();
//   void on_RestoreEq_clicked();
//    void on_CloseEq_clicked();
//    void on_SaveEq_clicked();
    void on_pushButton_clicked();
/*    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();*/
    void on_savebutt_clicked();
    void on_restbutt_clicked();
};
extern const char* eqnames[];
#endif // EQDIALOG_H
