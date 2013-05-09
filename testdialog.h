#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include "receiverinterface.h"
#include <QMoveEvent>
#include <QSettings>

namespace Ui {
class TestDialog;
}

class TestDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TestDialog(QWidget *parent, ReceiverInterface& Comm, QSettings& Settings);
    ~TestDialog();
    
private slots:
    void on_ClearButton_clicked();
    void on_SendButton_clicked();
    void on_SaveButton_clicked();

public slots:
    void ShowTestDialog();
    void NewDataReceived(QString data);
    void LogSendCmd(QString data);

private:
    Ui::TestDialog             *ui;
    ReceiverInterface&          m_Comm;
    QSettings&                  m_Settings;
    bool                        m_PositionSet;

    void AddToList(const QString& str);

    void moveEvent(QMoveEvent*event);

signals:
    void SendCmd(QString);

};


#endif // TESTDIALOG_H
