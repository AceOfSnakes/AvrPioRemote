#ifndef MCACCPROGRESSDIALOG_H
#define MCACCPROGRESSDIALOG_H

#include <QDialog>
#include "receiver_interface/receiverinterface.h"
#include "QSettings"
#include <QTimer>

namespace Ui {
class MCACCProgressDialog;
}

class MCACCProgressDialog : public QDialog, public ResponseListener
{
    Q_OBJECT

public:
    explicit MCACCProgressDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm);
    ~MCACCProgressDialog();
    // ResponseListener interface
    void ResponseReceived(ReceivedObjectBase *);

private:
    Ui::MCACCProgressDialog*    ui;
    QSettings&                  m_Settings;
    ReceiverInterface&          m_Comm;
    bool                        m_PositionSet;
    QTimer                      m_RefreshTimer;

protected:
    void changeEvent(QEvent *e);
    void moveEvent(QMoveEvent*event);
    void closeEvent(QCloseEvent *event);

public slots:
    void ShowMCACCProgressDialog();
    void RefreshTimeout();
signals:
    void SendCmd(QString);
};

#endif // MCACCPROGRESSDIALOG_H
