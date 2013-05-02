#ifndef OLDFAVORITESDIALOG_H
#define OLDFAVORITESDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "getoldfavoritelistdialog.h"
#include "receiverinterface.h"
#include <QListWidgetItem>

namespace Ui {
class OldFavoritesDialog;
}

class OldFavoritesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OldFavoritesDialog(QWidget *parent, ReceiverInterface* comm);
    ~OldFavoritesDialog();
    
private slots:
    void on_GetFavoritesButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

public slots:
    void DisplayData(int no, QString data);
    void InputFunctionData(int no, QString name);
    void FavoritesAquired();


private:
    Ui::OldFavoritesDialog *ui;

    QStringList m_FavList;
    ReceiverInterface* m_Comm;
    QString m_SelectedFavorite;
    bool    m_SelectAction;
    enum    Direction
    {
        DirectionNone,
        DirectionUp,
        DirectionDown
    };

    Direction   m_Direction;

    bool ReadFile(const QString& fileName);
    bool SaveFile(const QString& fileName, const QStringList &favs);
    int FindFavorite(const QString& fav);
signals:
    void SendCmd(QString);
};

#endif // OLDFAVORITESDIALOG_H
