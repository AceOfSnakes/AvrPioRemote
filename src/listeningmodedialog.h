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
#ifndef LISTENINGMODEDIALOG_H
#define LISTENINGMODEDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QComboBox>
#include <QByteArray>
#include "receiver_interface/receiverinterface.h"
#include <QLabel>
#include <QMoveEvent>

namespace Ui {
class ListeningModeDialog;
}

class ListeningModeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ListeningModeDialog(QWidget *parent , QSettings& settings, ReceiverInterface& Comm);
    ~ListeningModeDialog();


private:
    QSettings&              m_Settings;
    ReceiverInterface&      m_Comm;
    Ui::ListeningModeDialog *ui;
    bool                    m_PositionSet;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    class LMUserData: public QObjectUserData
#else
    class LMUserData: public QObjectData
#endif
    {
    public:
        QString m_Data;
    };

    void moveEvent(QMoveEvent*event);

signals:
    void SendCmd(QString data);

public slots:
    void ShowListeningDialog();

private slots:
    void on_listmodi_doubleClicked(const QModelIndex &index);
    void LMchanged(QString id, QString name);
};

#endif // LISTENINGMODEDIALOG_H
