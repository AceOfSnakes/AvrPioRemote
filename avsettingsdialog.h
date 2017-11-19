#ifndef AVSETTINGSDIALOG_H
#define AVSETTINGSDIALOG_H

#include <QDialog>
#include "receiver_interface/receiverinterface.h"
#include <QSettings>
#include <QTimer>

namespace Ui {
class AVSettingsDialog;
}

class AVSettingsDialog : public QDialog, public ResponseListener
{
    Q_OBJECT
    
public:
    explicit AVSettingsDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm);
    ~AVSettingsDialog();
    void ResponseReceived(ReceivedObjectBase *);

private:
    Ui::AVSettingsDialog *ui;
    QSettings&              m_Settings;
    ReceiverInterface&      m_Comm;
    bool                    m_PositionSet;
    QString                 m_LastCmd;
    int                     m_CmdRepeatCounter;
    QTimer                  m_CmdRepeatTimer;

    void moveEvent(QMoveEvent*event);

    void Refresh();
    void SendCmd(QString cmd);
    void EnableAVControls(bool enable);
    void InputChanged(int, QString);

public slots:
    void ShowAVSettingsDialog();
    void NewDataReceived(const QString &data, bool is_pioneer);
    void CmdRepeatTimeout();

signals:
    void SendCmdSignal(QString data);

private slots:
    void on_VideoConvCheckBox_clicked(bool checked);
    void on_ResolutionComboBox_activated(int index);
    void on_PureCinemaComboBox_activated(int index);
    void on_ProgMotionSlider_valueChanged(int value);
    void on_StreamSmootherComboBox_activated(int index);
    void on_AdvancedVideoAdjustComboBox_activated(int index);
    void on_YNRSlider_valueChanged(int value);
    void on_CNRSlider_valueChanged(int value);
    void on_BNRSlider_valueChanged(int value);
    void on_MNRSlider_valueChanged(int value);
    void on_DetailSlider_valueChanged(int value);
    void on_SharpnessSlider_valueChanged(int value);
    void on_BrightnessSlider_valueChanged(int value);
    void on_HueSlider_valueChanged(int value);
    void on_ContrastSlider_valueChanged(int value);
    void on_ChromaLevelSlider_valueChanged(int value);
    void on_BlackSetupComboBox_activated(int index);
    void on_AspectComboBox_activated(int index);
    void on_MemSetNoComboBox_activated(int index);
    void on_PhaseControlComboBox_activated(int index);
    void on_VirtualSBCheckBox_clicked(bool checked);
    void on_VirtualHeightCheckBox_clicked(bool checked);
    void on_VirtualWideCheckBox_clicked(bool checked);
    void on_VirtualDepthCheckBox_clicked(bool checked);
    void on_SoundRetrieverCheckBox_clicked(bool checked);
    void on_SignalSelectComboBox_activated(int index);
    void on_AnalogInputATTCheckBox_clicked(bool checked);
    void on_MCACCEQCheckBox_clicked(bool checked);
    void on_StandingWaveCheckBox_clicked(bool checked);
    void on_SoundDelaySlider_valueChanged(int value);
    void on_DigitalNoiseReductionCheckBox_clicked(bool checked);
    void on_DialogEnhancementComboBox_activated(int index);
    void on_HiBitCheckBox_clicked(bool checked);
    void on_DualMonoComboBox_activated(int index);
    void on_FixedPCMCheckBox_clicked(bool checked);
    void on_DRCComboBox_activated(int index);
    void on_LFEATTComboBox_activated(int index);
    void on_SACDGainCheckBox_clicked(bool checked);
    void on_AutoDelayCheckBox_clicked(bool checked);
    void on_EffectStrengthSlider_valueChanged(int value);
    void on_LoudnessCheckBox_clicked(bool checked);
    void on_HDMIOutputComboBox_activated(int index);
    void on_HDMIAudioComboBox_activated(int index);
    void on_PQLSComboBox_activated(int index);
    void on_DimmerComboBox_activated(int index);
    void on_SleepComboBox_activated(int index);
};

#endif // AVSETTINGSDIALOG_H
