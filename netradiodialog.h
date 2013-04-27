#ifndef NETRADIODIALOG_H
#define NETRADIODIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class NetRadioDialog;
}

class NetRadioDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NetRadioDialog(QWidget *parent, QSettings& settings);
    ~NetRadioDialog();
    
private:
    QSettings& m_Settings;
    Ui::NetRadioDialog *ui;
    int m_IndexOfLine1;
    int m_IndexOfLastLine;
    int m_TotalNumberOfItems;
    int m_SelectedItemIndex;

    int m_VisibleListSize;
    QStringList m_StationList;

public slots:
    void NetData(QString data);
private slots:
    void on_CursorUpButton_clicked();
    void on_CursorLeftButton_clicked();
    void on_CursorEnterButton_clicked();
    void on_CursorRightButton_clicked();
    void on_CursorDownButton_clicked();
    void on_CursorReturnButton_clicked();
    void on_NetPauseButton_clicked();

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

signals:
    void SendCmd(QString data);
};

#endif // NETRADIODIALOG_H
