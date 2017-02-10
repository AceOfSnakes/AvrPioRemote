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
#ifndef LOUDSPEAKERSETTINGSDIALOG_H
#define LOUDSPEAKERSETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QComboBox>
#include <QByteArray>
#include "receiver_interface/receiverinterface.h"
#include <QLabel>
#include <QMoveEvent>
#include <QCheckBox>
#include <QVector>
#include <QPair>
#include <QRadioButton>

namespace Ui {
class LoudspeakerSettingsDialog;
}

class LoudspeakerSettingsDialog : public QDialog, public ResponseListener
{
    Q_OBJECT
    
public:
    explicit LoudspeakerSettingsDialog(QWidget *parent, QSettings& settings,ReceiverInterface& Comm);
    ~LoudspeakerSettingsDialog();
    // ResponseListener interface
    void ResponseReceived(ReceivedObjectBase *);

    QString mCurrentSpeakerSetting; // Wert der aktuellen Konfig
    QVector<int> mchannels; //Wert des aktuellen Channels DB-Preset
    int errflag; //errorflag für SSF Befehl setzen

private:
    QSettings& m_Settings;
    Ui::LoudspeakerSettingsDialog *ui;
    QList<QCheckBox*>       m_MCACCButtons;
    ReceiverInterface&      m_Comm;
    QList<QSlider*>         m_Sliders;
    QList<QLabel*>          m_Labels;
    bool                    m_PositionSet;

    QVector<QComboBox*>     m_SpeakerSettings;
    bool                    m_RefreshSpeakerSettings;
    QVector<QRadioButton*>  m_XOverButtons;
    QVector<QPair<QString, QString>> m_SpeakerConfigurations;

    void moveEvent(QMoveEvent*event);
    void requestSpeakerSettings();
    void enableSlider(bool);
    void enableSpeakerSettings(bool);
    void disableControls();
    void requestData();
    void initSpeakerConfiguration();
    void addSpkConf(const QString&, const QString&);

public slots:

//    void SpeakerReceived(QString data);
    void ShowLoudspeakerSettingsDialog();

signals:
    void SendCmd(QString data);

private slots:
    void on_savebutt_clicked();
    void on_restbutt_clicked();
    void ValueChanged();
    void setslider();
    void setslider(int idx, int value);
    void on_selectmem_currentIndexChanged(int index);
    void clear_toggles();
    void checkbox();
    void on_spa_clicked();
    void on_spb_clicked();
    void on_spab_clicked();
    void on_spoff_clicked();
    void speakerSettingsComboBoxValueChanged(int index);
    void on_radioButtonSurOnSide_clicked();
    void on_radioButtonSurBehind_clicked();
    void XOver_selected();
    void on_LSsystem_currentIndexChanged(int index);
};

#endif // LOUDSPEAKERSETTINGSDIALOG_H
