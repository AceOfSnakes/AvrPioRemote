#ifndef USBDIALOG_H
#define USBDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QListWidgetItem>
#include "receiverinterface.h"
#include <QTimer>
#include <QMoveEvent>
#include <QIcon>

namespace Ui {
class usbDialog;
}

class usbDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit usbDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm);
    ~usbDialog();
    
private:
    QSettings&          m_Settings;
    Ui::usbDialog *ui;
    int                 m_IndexOfLine1;
    int                 m_IndexOfLastLine;
    int                 m_TotalNumberOfItems;
    int                 m_SelectedItemIndex;
    QTimer              m_Timer;
    int                 m_VisibleListSize;
    QStringList         m_StationList;
    ReceiverInterface&  m_Comm;
    bool                m_PositionSet;
    int                 m_ScreenType;
    QMap<int, QIcon>    m_Icons;

    void moveEvent(QMoveEvent*event);

public slots:
    void usbrecData(QString data);
    void ShowusbDialog();
    void randrepeattest(int dispNo, QString data);

private slots:
    void on_CursorUpButton_clicked();
    void on_CursorLeftButton_clicked();
    void on_CursorEnterButton_clicked();
    void on_CursorRightButton_clicked();
    void on_CursorDownButton_clicked();
    void on_CursorReturnButton_clicked();
    void on_NetPauseButton_clicked();
    void Timeout();
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

    void on_bicontrol_clicked();

    void on_bdirectctl_clicked();

    void on_brandom_clicked();

    void on_brepeat_clicked();

signals:
    void SendCmd(QString data);
};

#endif // usbDialog_H
