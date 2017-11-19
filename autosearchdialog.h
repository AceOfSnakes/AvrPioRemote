#ifndef AUTOSEARCHDIALOG_H
#define AUTOSEARCHDIALOG_H

#include <string>
#include <QDialog>
#include <QDebug>
#include <QModelIndex>
#include <QLibrary>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QSslSocket>
#include "logger.h"
#include <QUdpSocket>
#include <QSettings>
#include "receiver_interface/devices/discoverydevice.h"

using namespace std;
namespace Ui {
class AutoSearchDialog;
}

class AutoSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AutoSearchDialog(QSettings& settings, QWidget *parent = 0, bool receiver = true, QString pingCommand="?RGD",
                              QString pingResponseStart="RGD", QString pingResponseStartOff="", int prefferedPort=23, bool crlf=true);
    ~AutoSearchDialog();
    int                     prefferedPort;
    bool                    crlf;
    int                     m_Result;
    QSettings&              m_Settings;
    QString                 m_SelectedAddress;
    int                     m_SelectedPort;
    bool                    m_SelectedPioneer;
    QVector<DiscoveryDevice*> m_DeviceInList;

protected:
    void changeEvent(QEvent *e);
    QString m_pingCommand;
    QString m_pingResponseStart;
    QString m_pingResponseStartOff;

    QMap<QString, DiscoveryDevice*> m_RemoteDevices;
    QVector<QUdpSocket*>    m_MulticatsSockets;
    QHostAddress            m_GroupAddress;
    bool                    m_FindReceivers;
    QString removeDevice(QMap<QString, DiscoveryDevice *> &m_RemoteDevices, DiscoveryDevice* device);
    void SendMsg();
    void reconnect(QString & key, QString & ip, int port, DiscoveryDevice *device);
private slots:
    void NewDevice(QString name, QString ip, QString location);
    void PioneerAnswer(const QString &answer);
    void ReadOnkyoAnswer(const QString &answer);
    void PioneerError(QAbstractSocket::SocketError socketError);
    void OnkyoError(QAbstractSocket::SocketError socketError);
    void TcpConnected();
    void TcpDisconnected();
    void ProcessPendingDatagrams();
    void on_CancelButton_clicked();
    void on_continueButton_clicked();
    void on_repeatButton_clicked();
    void on_listWidget_clicked(const QModelIndex &index);
    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::AutoSearchDialog *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // AUTOSEARCHDIALOG_H
