/*
 * AVRPioRemote
 * Copyright (C) 2013  Andreas Müller, Ulrich Mensfeld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
    void ShowOldFavoritesDialog();
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
