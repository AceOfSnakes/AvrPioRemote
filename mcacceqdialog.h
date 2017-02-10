#ifndef MCACCEQDIALOG_H
#define MCACCEQDIALOG_H

#include <QDialog>
#include "receiver_interface/receiverinterface.h"
#include <QSettings>
#include <QRadioButton>
#include <QSlider>
#include <QLabel>
#include <QPair>

namespace Ui {
class MCACCEQDialog;
}

class MCACCEQDialog : public QDialog, public ResponseListener
{
    Q_OBJECT
    
public:
    explicit MCACCEQDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm);
    ~MCACCEQDialog();
    // ResponseListener interface
    void ResponseReceived(ReceivedObjectBase *);

private:
    struct EQData {
        int value;
        bool enabled;
        QSlider* slider;
        QLabel* label;
        EQData() {
            value = 50;
            enabled = false;
            slider = NULL;
            label = NULL;
        }
    };
    struct ChannelData {
        double distance;
        QVector<EQData> eqData;
        QString channel;
        QRadioButton* radio;
        ChannelData() {
            distance = 0;
            radio = NULL;
        }
    };

    Ui::MCACCEQDialog *ui;
    QSettings&              m_Settings;
    ReceiverInterface&      m_Comm;
    bool                    m_PositionSet;
    int                     m_CurrentMcacc;
    int                     m_SelectedChannel;

    QVector<ChannelData>    m_Channels;
    //QVector< QVector<int> > m_EQData;
    //QVector<double>         m_Distance;
    //QVector<QPair<QString, QRadioButton*> >  m_Speakers;
    QVector<QSlider*>       m_Slider;
    QVector<QLabel*>        m_Labels;
    QVector<QRadioButton*>  m_MCACCButtons;

    void moveEvent(QMoveEvent*event);
    void RefreshSpeakerEq(QString speaker);
    void EnableSlider(bool enabled);
    void SendDistance();
    void DisableControls();
    void addChannel(const QString&, QRadioButton*);

public slots:
    void ShowMCACCEQDialog();
    void SliderValueChanged();
    void ChannelClicked();

signals:
    void SendCmd(QString data);
private slots:
    void MCACC_selected();
    void on_SaveToFilePushButton_clicked();
    void on_RestoreFromFilePushButton_clicked();
    void on_ImmediatelyCheckBox_stateChanged(int state);
    void on_DistanceSpinBox_valueChanged(double);
    void on_SetDistancePushButton_clicked();
};

#endif // MCACCEQDIALOG_H
