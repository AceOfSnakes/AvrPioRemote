#ifndef HDMICONTROLDIALOG_H
#define HDMICONTROLDIALOG_H

#include <QDialog>
#include <QSettings>
#include "receiverinterface.h"

namespace Ui {
class HdmiControlDialog;
}

class HdmiControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HdmiControlDialog(QWidget *parent, QSettings &settings, ReceiverInterface &Comm);
    ~HdmiControlDialog();
    bool IsLastEnabled();

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
    bool                        m_LastEnabled;

public slots:
    void ShowHdmiControlDialog();

private slots:
    void CommConnected();
    void DataReceived(QString data);
    void on_hdmiControlCheckBox_toggled(bool);
    void on_hdmiControlModeCheckBox_toggled(bool);
    void on_arcCheckBox_toggled(bool);
    void on_standbyPassThroughComboBox_currentIndexChanged(int);

signals:
    void SendCmd(QString);
};

#endif // HDMICONTROLDIALOG_H
