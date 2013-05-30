#ifndef ZONECONTROLDIALOG_H
#define ZONECONTROLDIALOG_H

#include <QDialog>
#include "receiverinterface.h"
#include <QSettings>

namespace Ui {
class ZoneControlDialog;
}

class ZoneControlDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ZoneControlDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm);
    ~ZoneControlDialog();
    
private:
    Ui::ZoneControlDialog *ui;
    QSettings&              m_Settings;
    ReceiverInterface&      m_Comm;
    bool                    m_PositionSet;

    void moveEvent(QMoveEvent*event);

public slots:
    void ShowZoneControlDialog();

signals:
    void SendCmd(QString data);

private slots:
    // Receiver messages
    void ZonePower (int zone, bool on);
    void ZoneInput (int zone, int input);
    void ZoneVolume (int zone, int volume, QString asString);
    void ZoneMute (int zone, bool muted);
    void DisplayData(int DispNo, QString data);
    // gui controls
    void on_MultiZoneButton_clicked();
    void on_Z2ActivateButton_clicked();
    void on_Z2PowerButton_clicked();
    void on_Z2InputComboBox_activated(int index);
    void on_Z2VolumeUpButton_clicked();
    void on_Z2VolumeDownButton_clicked();
    void on_Z2MuteButton_clicked();
    void on_Z3MuteButton_clicked();
    void on_Z3PowerButton_clicked();
    void on_Z3InputComboBox_activated(int index);
    void on_Z3VolumeUpButton_clicked();
    void on_Z3VolumeDownButton_clicked();
};

#endif // ZONECONTROLDIALOG_H
