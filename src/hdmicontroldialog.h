#ifndef HDMICONTROLDIALOG_H
#define HDMICONTROLDIALOG_H

#include <QDialog>
#include <QSettings>
#include "receiver_interface/receiverinterface.h"


namespace Ui {
class HdmiControlDialog;
}

class HdmiControlDialog : public QDialog, public ResponseListener
{
    Q_OBJECT

public:
    explicit HdmiControlDialog(QWidget *parent, QSettings &settings, ReceiverInterface &Comm);
    ~HdmiControlDialog();
    // ResponseListener interface
    void ResponseReceived(ReceivedObjectBase *);

protected:
    void changeEvent(QEvent *e);
    void moveEvent(QMoveEvent*event);
    void closeEvent(QCloseEvent *event);

private:
    QSettings&                  m_Settings;
    Ui::HdmiControlDialog *ui;
    ReceiverInterface&          m_Comm;
    bool                        m_PositionSet;
    bool                        m_PowerOn;

public slots:
    void ShowHdmiControlDialog();

private slots:
    void CommConnected();
    void on_hdmiControlCheckBox_toggled(bool);
    void on_hdmiControlModeCheckBox_toggled(bool);
    void on_arcCheckBox_toggled(bool);
    void on_standbyPassThroughComboBox_currentIndexChanged(int);

signals:
    void SendCmd(QString);
};

#endif // HDMICONTROLDIALOG_H
