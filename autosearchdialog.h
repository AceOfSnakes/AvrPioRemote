#ifndef AUTOSEARCHDIALOG_H
#define AUTOSEARCHDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QLibrary>
#include <QtNetwork/QTcpSocket>
#include "logger.h"
#include <QUdpSocket>

namespace Ui {
class AutoSearchDialog;
}

class RemoteDevice : public QObject
{
    Q_OBJECT
public:
    RemoteDevice();
    void Connect(QString ip, int port);
    ~RemoteDevice();

    QString ip;
    int port;
    QTcpSocket* socket;

private slots:
    void _DataAvailable(){emit DataAvailable();}
    void _TcpError(QAbstractSocket::SocketError socketError){emit TcpError(socketError);}
    void _TcpConnected(){emit TcpConnected();}
    void _TcpDisconnected(){TcpDisconnected();}
signals:
    void DataAvailable();
    void TcpError(QAbstractSocket::SocketError socketError);
    void TcpConnected();
    void TcpDisconnected();
};

class AutoSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AutoSearchDialog(QWidget *parent = 0);
    ~AutoSearchDialog();

    int                     m_Result;
    //int                     m_SelectedIndex;
    QString                 m_SelectedAddress;
    int                     m_SelectedPort;
    QVector<RemoteDevice*>  m_DeviceInList;

protected:
    void changeEvent(QEvent *e);
    QVector<RemoteDevice*>  m_RemoteDevices;
    QVector<QUdpSocket*>    m_MulticatsSockets;
    QHostAddress            m_GroupAddress;

    void SendMsg();

private slots:
    void NewDevice(QString name, QString url);
    void ReadString();
    void TcpError(QAbstractSocket::SocketError socketError);
    void TcpConnected();
    void TcpDisconnected();
    void ProcessPendingDatagrams();
    void on_CancelButton_clicked();
    void on_continueButton_clicked();
    void on_repeatButton_clicked();
    void on_listWidget_clicked(const QModelIndex &index);
    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::AutoSearchDialog *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // AUTOSEARCHDIALOG_H
