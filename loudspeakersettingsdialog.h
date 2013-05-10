#ifndef LOUDSPEAKERSETTINGSDIALOG_H
#define LOUDSPEAKERSETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QComboBox>
#include <QByteArray>
#include "receiverinterface.h"
#include <QLabel>
#include <QMoveEvent>
#include <QCheckBox>

namespace Ui {
class LoudspeakerSettingsDialog;
}

class LoudspeakerSettingsDialog : public QDialog
{
    Q_OBJECT


    
public:
    explicit LoudspeakerSettingsDialog(QWidget *parent, QSettings& settings,ReceiverInterface& Comm);
    ~LoudspeakerSettingsDialog();
    int mVal; // Wert der aktuellen Konfig
    int mLSpaar[7]; //Wert des aktuellen LS- 0=small, 1=large,2=NO
    int mchannels[12]; //Wert des aktuellen Channels DB-Preset
    int errflag; //errorflag für SSF Befehl setzen

private:
    QSettings& m_Settings;
    Ui::LoudspeakerSettingsDialog *ui;
    QList<QCheckBox*>   m_buttons;
    ReceiverInterface&  m_Comm;
    QList<QSlider*>     m_Sliders;
    QList<QLabel*>      m_Labels;
    bool                m_PositionSet;

    void moveEvent(QMoveEvent*event);

public slots:

//    void SpeakerReceived(QString data);
    void ShowLoudspeakerSettingsDialog();
    void Speakerinfo(QString data);
    void error(int);

signals:
    void SendCmd(QString data);

private slots:
    void on_SetBut_clicked();
    void on_SetBut2_clicked();
    void on_speaker_currentIndexChanged(int index);
    void on_savebutt_clicked();
    void on_restbutt_clicked();
    void ValueChanged();
    void setslider();
    void on_selectmem_currentIndexChanged(int index);
    void clear_toggles();
    void checkbox();
};

extern const char* LStyp[]; //Lautsprecherkonfig (SB/FH, Zone2 etc)
extern const int LSwert[];  //Wert für Konfig im AVR, beide gesetzt im .cpp
extern const char* LSpaar[];    //Lautsprechercode für small/large
extern const char* channels[];
extern const char* slchannels[];

#endif // LOUDSPEAKERSETTINGSDIALOG_H
