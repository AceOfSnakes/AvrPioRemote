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
#ifndef TUNERDIALOG_H
#define TUNERDIALOG_H

#include <QDialog>
#include <vector>
#include <QSettings>
#include "actionwithparameter.h"
#include "receiver_interface/receiverinterface.h"
#include <QMoveEvent>

namespace Ui {
class TunerDialog;
}

class TunerDialog : public QDialog, public ResponseListener
{
    Q_OBJECT
    
public:
    explicit TunerDialog(QWidget *parent, ReceiverInterface& Comm, QSettings& settings);
    ~TunerDialog();
    // ResponseListener interface
    void ResponseReceived(ReceivedObjectBase *);

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
    int                         m_TunerStringPosition;

    bool GetScrolledString(QString input);

    void moveEvent(QMoveEvent*event);

public slots:
    void ManualShowTunerDialog();
    void ShowTunerDialog(bool autoShow);
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
