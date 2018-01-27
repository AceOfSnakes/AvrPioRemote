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
#ifndef LOUDSPEAKERSETTINGSONKYODIALOG_H
#define LOUDSPEAKERSETTINGSONKYODIALOG_H

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
class LoudspeakerSettingsOnkyoDialog;
}

class LoudspeakerSettingsOnkyoDialog : public QDialog, public ResponseListener
{
    Q_OBJECT
    
public:
    explicit LoudspeakerSettingsOnkyoDialog(QWidget *parent, QSettings& settings,ReceiverInterface& Comm);
    ~LoudspeakerSettingsOnkyoDialog();
    // ResponseListener interface
    void ResponseReceived(ReceivedObjectBase *);

    QString mCurrentSpeakerSetting; // Wert der aktuellen Konfig
    //QVector<int> mchannels; //Wert des aktuellen Channels DB-Preset
    //int errflag; //errorflag für SSF Befehl setzen

private:
    QSettings& m_Settings;
    Ui::LoudspeakerSettingsOnkyoDialog *ui;
    QList<QCheckBox*>       m_MCACCButtons;
    ReceiverInterface&      m_Comm;
    QList<QSlider*>         m_Sliders;
    QList<QLabel*>          m_Labels;
    bool                    m_PositionSet;

    QVector<QComboBox*>     m_SpeakerCombos;
    bool                    m_RefreshSpeakerSettings;
    QVector<QPair<QString, QString>> m_SpeakerConfigurations;

    SpeakerInformationResponse_SPI m_SpeakerSettings;

    void moveEvent(QMoveEvent*event);
    void enableSlider(bool);
    void enableSpeakerSettings(bool);
    void disableControls();
    void requestData();
    void initSpeakerConfiguration();
    void addSpkConf(const QString&, const QString&);
    QString GetOnkyoChannelString();

public slots:

//    void SpeakerReceived(QString data);
    void ShowLoudspeakerSettingsOnkyoDialog();

signals:
    void SendCmd(QString data);

private slots:
    void ValueChanged();
    void setslider();
    void setslider(int idx, int value);
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

#endif // LOUDSPEAKERSETTINGSONKYODIALOG_H
