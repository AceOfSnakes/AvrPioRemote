#ifndef WIRINGDIALOG_H
#define WIRINGDIALOG_H

#include <QDialog>
#include <QSettings>
#include "receiver_interface/receiverinterface.h"
#include "Defs.h"
#include <QVector>
#include "wiringmodel.h"
#include "comboboxdelegate.h"
#include "checkboxdelegate.h"

namespace Ui {
class WiringDialog;
}

class WiringDialog : public QDialog, public ResponseListener
{
    Q_OBJECT

public:
    explicit WiringDialog(QWidget *parent, QSettings& settings, ReceiverInterface& Comm);
    ~WiringDialog();
    // ResponseListener interface
    void ResponseReceived(ReceivedObjectBase *);

protected:
    void changeEvent(QEvent *e);
    void moveEvent(QMoveEvent*event);
    void closeEvent(QCloseEvent *event);

private:
    QSettings&                  m_Settings;
    Ui::WiringDialog *ui;
    ReceiverInterface&          m_Comm;
    bool                        m_PositionSet;
    int                         m_ParameterErrorCount;
    int                         m_InputAnswersCount;
    WiringModel                 m_WiringModel;

    enum ACTION
    {
        ACTION_GET_NAME,
        ACTION_GET_AUDIO_ASSIGNMENT,
        ACTION_GET_HDMI_ASSIGNMENT,
        ACTION_GET_VIDEO_ASSIGNMENT,
        ACTION_GET_SKIP,
        ACTION_NOTHING,
    };
    ACTION                      m_CurrentAction;
    QVector<InputContainer>     m_Inputs;
    QVector<InputContainer>::Iterator m_CurrentInput;

    void AquireData();

public slots:
    void ShowWiringDialog();
    void DataReceived(const QString &data, bool is_pioneer);

signals:
    void SendCmd(QString);
private slots:
};

#endif // WIRINGDIALOG_H
