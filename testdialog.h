#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class TestDialog;
}

class TestDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TestDialog(QWidget *parent = 0);
    ~TestDialog();
    
private slots:
    void Timeout();
    void on_ClearButton_clicked();

public slots:
    void DisplayData(int no, QString data);
    void InputFunctionData(int no, QString name);
private:
    Ui::TestDialog *ui;

    QString m_Remembered;
    QString m_Firstline;
    bool    m_Done;
    bool    m_Recording;
    QTimer  m_Timer;
    QStringList m_FavList;

    bool GetScrolledString(QString input);
signals:
    void SendCmd(QString);
};

#endif // TESTDIALOG_H
