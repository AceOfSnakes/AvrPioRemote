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
#ifndef NETRADIODIALOG_H
#define NETRADIODIALOG_H

#include <QDialog>
#include <QSettings>
#include <QListWidgetItem>
#include "receiver_interface/receiverinterface.h"
#include <QTimer>
#include <QMoveEvent>
#include <QIcon>
#include <QMap>
#include <inttypes.h>

namespace Ui {
class NetRadioDialog;
}

class NetRadioDialog : public QDialog, public ResponseListener
{
    Q_OBJECT
    
public:
    explicit NetRadioDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm);
    ~NetRadioDialog();
    // ResponseListener interface
    void ResponseReceived(ReceivedObjectBase *);

private:
    QSettings&          m_Settings;
    Ui::NetRadioDialog *ui;
    int                 m_IndexOfLine1;
    int                 m_IndexOfLastLine;
    int                 m_TotalNumberOfItems;
    int                 m_SelectedItemIndex;
    QTimer              m_Timer;
    int                 m_VisibleListSize;
    QStringList         m_StationList;
    ReceiverInterface&  m_Comm;
    bool                m_PositionSet;
    QMap<int, QIcon>    m_Icons;
    int                 m_ScreenType;
    int                 m_ListUpdateFlag;
    uint64_t            m_PlayTime;
    QTimer              m_PlayTimeTimer;
    QString             m_PlayFormat;
    QString             m_PlayBitrate;
    bool                m_NewDataFormat;
    QString             m_BitsPerSample;

    void moveEvent(QMoveEvent*event);
    void handleTime(QString str);
    void showAudioInfoLine();


public slots:
    void NetData(QString data);
    void ManualShowNetDialog();
    void ShowNetDialog(bool autoShow);
private slots:
    void on_CursorUpButton_clicked();
    void on_CursorLeftButton_clicked();
    void on_CursorEnterButton_clicked();
    void on_CursorRightButton_clicked();
    void on_CursorDownButton_clicked();
    void on_CursorReturnButton_clicked();
    void on_NetPauseButton_clicked();
    void Timeout();
    void RefreshPlayTime();
    void on_NetPlayButton_clicked();
    void on_NetStopButton_clicked();
    void on_NetPrevButton_clicked();
    void on_NetNextButton_clicked();
    void on_NetRevButton_clicked();
    void on_NetFwdButton_clicked();
    void on_listWidget_currentRowChanged(int currentRow);
    void on_PageUpButton_clicked();
    void on_PageDownButton_clicked();
    void on_PageUpButton_2_clicked();
    void on_PageDownButton_2_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_NetAddFavButton_clicked();
    void on_NetRemoveFavButton_clicked();
    void on_NetSwitchToNetRadioButton_clicked();
    void on_NetSwitchToMediaServerButton_clicked();
    void on_NetSwitchToFavoritesButton_clicked();
    void on_pushButton_clicked();
    void on_RepeatButton_clicked();
    void on_RandomButton_clicked();

signals:
    void SendCmd(QString data);
};

#endif // NETRADIODIALOG_H
