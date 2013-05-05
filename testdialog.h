#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include "receiverinterface.h"

namespace Ui {
class TestDialog;
}

class TestDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TestDialog(QWidget *parent, ReceiverInterface& Comm);
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
    Ui::TestDialog *ui;
    ReceiverInterface&          m_Comm;

    void AddToList(const QString& str);

    signals:
    void SendCmd(QString);
};

#endif // TESTDIALOG_H
