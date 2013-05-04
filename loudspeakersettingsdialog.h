#ifndef LOUDSPEAKERSETTINGSDIALOG_H
#define LOUDSPEAKERSETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QComboBox>
#include <QByteArray>
#include "receiverinterface.h"

namespace Ui {
class LoudspeakerSettingsDialog;
}

class LoudspeakerSettingsDialog : public QDialog
{
    Q_OBJECT


    
public:
    explicit LoudspeakerSettingsDialog(QWidget *parent, QSettings& settings,ReceiverInterface& Comm);
    ~LoudspeakerSettingsDialog();
    int mVal;

private:
    QSettings& m_Settings;
    Ui::LoudspeakerSettingsDialog *ui;
    ReceiverInterface&  m_Comm;

    void RequestEQ(const QString& Memory, const QString& Loudspeaker, const QString& Frequency);

public slots:
    void Data(QString& data);
    void DataReceived(QString data);
    void ShowLoudspeakerSettingsDialog();
    void error(int);

signals:
    void SendCmd(QString data);

private slots:
        void on_LSsystem_currentIndexChanged(int index);
    void on_SetBut_clicked();
    void on_SetBut2_clicked();
};

extern const char* LStyp[]; //Lautsprecherkonfig (SB/FH, Zone2 etc)
extern const int LSwert[];  //Wert für Konfig im AVR, beide gesetzt im .cpp


#endif // LOUDSPEAKERSETTINGSDIALOG_H
