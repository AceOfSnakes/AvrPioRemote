#ifndef GETOLDFAVORITELISTDIALOG_H
#define GETOLDFAVORITELISTDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QMessageBox>
#include "receiverinterface.h"

namespace Ui {
class GetOldFavoriteListDialog;
}

class GetOldFavoriteListDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GetOldFavoriteListDialog(QWidget *parent, ReceiverInterface *comm);
    ~GetOldFavoriteListDialog();

    
private:
    Ui::GetOldFavoriteListDialog *ui;

private slots:
    void Timeout();

    void on_CancelButton_clicked();

public slots:
    void DisplayData(int no, QString data);
    void InputFunctionData(int no, QString name);
    void ShowDialog();

private:
    QString m_Remembered;
    QString m_Firstline;
    bool    m_Done;
    bool    m_Recording;
    QTimer  m_Timer;
    ReceiverInterface* m_Comm;
    QStringList m_FavList;
    enum AquireAction
    {
        AquireActionAquire,
        AquireActionSetFirst
    };
    AquireAction m_CurrentAction;

    void StartAquire();
    bool GetScrolledString(QString input);
    void FinishAquire();
    bool ReadFile(const QString& fileName);
    bool SaveFile(const QString& fileName, const QStringList &favs);
signals:
    void SendCmd(QString);
    void FavoritesAquired();
};

#endif // GETOLDFAVORITELISTDIALOG_H
