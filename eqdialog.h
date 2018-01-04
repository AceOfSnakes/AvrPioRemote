/*
 * AVRPioRemote
 * Copyright (C) 2013  Andreas Müller, Ulrich Mensfeld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef EQDIALOG_H
#define EQDIALOG_H

#include <QDialog>
#include "receiver_interface/receiverinterface.h"
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QMoveEvent>
#include "emphasisdialog.h"

namespace Ui {
class EQDialog;
}


class EQDialog : public QDialog, public ResponseListener
{
    Q_OBJECT
    
public:
    explicit EQDialog(QWidget *parent, ReceiverInterface& Comm, QSettings& settings);
    ~EQDialog();
    void ResponseReceived(ReceivedObjectBase *);

private:
    Ui::EQDialog *ui;
    ReceiverInterface&  m_Comm;
    QList<QSlider*>     m_EQSliders;
    QList<QLabel*>      m_EQLabels;
    QTimer              m_Timer;
    QSettings&          m_Settings;
    int                 m_SelectedPreset;
    bool                m_PositionSet;
    bool                m_ToneON;
    EmphasisDialog*     m_EmphasisDialog;

    bool ReadFile(const QString& fileName);
    bool SaveFile(const QString& fileName);
    void ResetEQPresets();
    void SelectPreset(int preset);
    void SetEqSlider(int value, QSlider* slider, QLabel* label);
    void SetToneSlider(int value, QSlider* slider, QLabel* label);
    void SetXCurveSlider(int value, QSlider* slider, QLabel* label);
    void SetEmphasisSlider(int value, QSlider* slider, QLabel* label);
    void RestorePreset(int nr, bool eq, bool emph, bool tone, bool xcurve);
    void SavePreset(int nr, bool eq, bool emph, bool tone, bool xcurve);
    void SaveFilter();
    void RestoreFilter();

    void moveEvent(QMoveEvent*event);

public slots:
    void ShowEQDialog();
signals:
    void SendCmd(QString);
private slots:
    void Timeout();
    void OnSliderValueChanged(int value);
    void OnEmphasisSliderReleased();
    void CenterChanged(int);
    void BassChanged(int);
    void onSaveCheckBoxToggled(bool);
    void on_eqFlatPushButton_clicked();
    void on_savebutt_clicked();
    void on_restbutt_clicked();
    void onSelectmemCurrentIndexChanged(int index);
    void on_eqba_sliderReleased();
    void on_eqtr_sliderReleased();
    void on_bypass_clicked();
    void on_XCurveSlider_sliderReleased();
    void on_emphasisPushButton_clicked();
    void on_dfilter_slow_radioButton_clicked();
    void on_dfilter_sharp_radioButton_clicked();
    void on_dfilter_short_radioButton_clicked();
    void on_theater_checkBox_clicked();
    void on_drc_off_radioButton_clicked();
    void on_drc_on_radioButton_clicked();
    void on_drc_auto_radioButton_clicked();
    void on_upsampling_x1_radioButton_clicked();
    void on_upsampling_x2_radioButton_clicked();
    void on_upsampling_x4_radioButton_clicked();
    void on_loudness_checkBox_clicked();
    void on_fixed_pcm_checkBox_clicked();
    void on_audio_scalar_checkBox_clicked();
};
extern const char* eqnames[];
#endif // EQDIALOG_H
