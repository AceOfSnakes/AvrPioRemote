#ifndef MCACCEQDIALOG_H
#define MCACCEQDIALOG_H

#include <QDialog>
#include "receiverinterface.h"
#include <QSettings>
#include <QRadioButton>
#include <QSlider>
#include <QLabel>

namespace Ui {
class MCACCEQDialog;
}

class MCACCEQDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MCACCEQDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm);
    ~MCACCEQDialog();
    
private:
    Ui::MCACCEQDialog *ui;
    QSettings&              m_Settings;
    ReceiverInterface&      m_Comm;
    bool                    m_PositionSet;
    int                     m_CurrentMcacc;
    int                     m_SelectedChannel;

    QVector< QVector<int> > m_EQData;
    QVector<QRadioButton*>  m_Speakers;
    QVector<QSlider*>       m_Slider;
    QVector<QLabel*>        m_Labels;

    void moveEvent(QMoveEvent*event);
    void RefreshSpeakerEq(QString speaker);
    void EnableSlider(bool enabled);
    QString GetDBString(int dB);

public slots:
    void ShowMCACCEQDialog();
    void MCACCEQData(int mcacc, QString speaker, int eqidx, int value);
    void MCACC(int mcacc);
    void SliderValueChanged();
    void SpeakerClicked();

signals:
    void SendCmd(QString data);
private slots:
    void on_MCACCcomboBox_activated(int index);
    void on_SaveToFilePushButton_clicked();
    void on_RestoreFromFilePushButton_clicked();
};

#endif // MCACCEQDIALOG_H
