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
#include "receiverinterface.h"
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QMoveEvent>

namespace Ui {
class EQDialog;
}


class EQPreset
{
public:
    QString         m_Name;
    QPushButton*    m_Button;
    int             m_Values[11];
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
    bool                m_PositionSet;

    bool ReadFile(const QString& fileName);
    bool SaveFile(const QString& fileName);
    void ResetEQPresets();
    void SelectPreset(int preset);

    void moveEvent(QMoveEvent*event);

public slots:
    void ShowEQDialog();
    void DataReceived(QString data);
signals:
    void SendCmd(QString);
private slots:
    void Timeout();
    void OnSliderValueChanged(int value);
    void on_pushButton_clicked();
    void on_savebutt_clicked();
    void on_restbutt_clicked();
    void on_selectmem_currentIndexChanged(int index);
    void on_eqba_sliderReleased();
    void on_eqtr_sliderReleased();
    void on_bypass_clicked();
    void on_XCurveSlider_sliderReleased();
};
extern const char* eqnames[];
#endif // EQDIALOG_H
