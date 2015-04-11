#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>
#include "receiver_interface/receiverinterface.h"
#include "QSettings"

class GraphicLSWidget;

namespace Ui {
class InfoDialog;
}

class InfoDialog : public QDialog, public ResponseListener
{
    Q_OBJECT

public:
    explicit InfoDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm);
    ~InfoDialog();
    // ResponseListener interface
    void ResponseReceived(ReceivedObjectBase *);

private:
    Ui::InfoDialog*             ui;
    GraphicLSWidget*            inputLSConfiguration;
    GraphicLSWidget*            outputLSConfiguration;
    QSettings&                  m_Settings;
    ReceiverInterface&          m_Comm;
    bool                        m_PositionSet;

protected:
    void changeEvent(QEvent *e);
    void moveEvent(QMoveEvent*event);
    void closeEvent(QCloseEvent *event);

public slots:
    void ShowInfoDialog();

signals:
//    void SendNewData(QString);
};

#endif // INFODIALOG_H
