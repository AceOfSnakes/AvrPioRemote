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
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QTranslator>
#include <QIntValidator>
#include "receiver_interface/receiverinterface.h"
#include "autosearchdialog.h"
#include "playerinterface.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm, PlayerInterface &CommBD);
    ~SettingsDialog();

    void SetLanguage();
    void GetIpAddress(QString& ip1, QString& ip2, QString& ip3, QString& ip4, QString &port);
    void SetIpAddress(QString ip1, QString ip2, QString ip3, QString ip4, QString port);
    void GetIpAddressBD(QString& ip1, QString& ip2, QString& ip3, QString& ip4, QString &port);
    void SetIpAddressBD(QString ip1, QString ip2, QString ip3, QString ip4, QString port);

public slots:
    void ShowSettingsDialog();
    void EnableIPInput(bool enable);
    void EnableIPInputBD(bool enable);

private slots:
    void CommError(QString socketError);
    void CommConnected();
    void CommDisconnected();
    void CommErrorBD(QString socketError);
    void CommConnectedBD();
    void CommDisconnectedBD();
    void on_TunerVSX922CompatibilityModeCheckBox_stateChanged(int state);
    void on_LanguageAutoRadioButton_clicked(bool checked);
    void on_LanguageEnglishRadioButton_clicked(bool checked);
    void on_LanguageGermanRadioButton_clicked(bool checked);
    void on_pushButton_clicked();
    void on_ShowTunerCheckBox_clicked();
    void on_ShowNetRadioCheckBox_clicked();
    void on_RestoreMainWindowCheckBox_clicked();
    void on_RestoreEQWindowCheckBox_clicked();
    void on_RestoreTunerWindowCheckBox_clicked();
    void on_RestoreNetRadioWindowCheckBox_clicked();
    void on_RestoreSpeakerSettingsWindowCheckBox_clicked();
//    void on_RestoreTestWindowCheckBox_clicked();
    void on_RestoreLMSettingsWindowCheckBox_clicked();
    void on_RestoreUSBWindowCheckBox_clicked();
    void on_ShowUSBCheckBox_clicked();
//    void on_StartLoggingInTestWindowCheckBox_clicked();
    void on_ShowReceiverNameInTitleCheckBox_clicked();
    void on_ShowDefaultInputNameCheckBox_clicked();
    void on_RestoreZoneControlWindowCheckBox_clicked();
    void on_pushButtonConnect_clicked();
    void on_ShowBlueRayWindowCheckBox_clicked();
    void on_RestoreBlueRayWindowCheckBox_clicked();
    void on_RestoreAVSettingsWindowCheckBox_clicked();
    void on_LanguageRussianRadioButton_clicked(bool checked);
    void on_RestoreMCACCEQWindowCheckBox_clicked(bool checked);
    void on_pushButtonAuto_clicked();
    void on_pushButtonAuto_BD_clicked();

    void on_pushButtonConnect_BD_clicked();

    void on_ShowPlayerNameInTitle_clicked();

signals:
    void onConnect();
    void onConnectBD();

private:
    QSettings&          m_Settings;
    QTranslator         m_Translater;
    QIntValidator       m_IpValidator;
    QIntValidator       m_IpPortValidator;
    ReceiverInterface&  m_Comm;
    PlayerInterface&    m_CommBD;
    Ui::SettingsDialog *ui;
    AutoSearchDialog*   m_AutoSearchDialog;

protected:
    void changeEvent(QEvent *e);
};

#endif // SETTINGSDIALOG_H
