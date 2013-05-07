#ifndef LISTENINGMODEDIALOG_H
#define LISTENINGMODEDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QComboBox>
#include <QByteArray>
#include "receiverinterface.h"
#include <QLabel>

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
    Ui::ListeningModeDialog *ui;
    QSettings& m_Settings;
    ReceiverInterface&  m_Comm;

signals:
    void SendCmd(QString data);

public slots:
    void ShowListeningDialog();

private slots:
    void on_listmodi_doubleClicked(const QModelIndex &index);
    void LMchanged(QString name);
};

#endif // LISTENINGMODEDIALOG_H
