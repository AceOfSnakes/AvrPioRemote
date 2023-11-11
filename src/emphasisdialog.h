#ifndef EMPHASISDIALOG_H
#define EMPHASISDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QComboBox>
#include <QSettings>
#include "labeledslider.h"
#include "receiver_interface/receiverinterface.h"

namespace Ui {
class EmphasisDialog;
}

class EmphasisDialog : public QDialog, public ResponseListener
{
    Q_OBJECT

public:
    explicit EmphasisDialog(QWidget *parent, QSettings& settings);
    ~EmphasisDialog();
    void ResponseReceived(ReceivedObjectBase *);

    void SetBass(int n);
    void SetCenter(int n);
    int GetBass();
    int GetCenter();
    QString GetChannelString();
    QString GetOnkyoChannelString();
    void SetChannelString(QString str);
    void SetIsPioneer(bool isPioneer);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EmphasisDialog *ui;
    QList<LabeledSlider*> m_EmphasisSliders;
    QSettings&          m_Settings;
    int                 m_BassCh1;
    int                 m_BassCh2;
    bool                m_IsPioneer;

    void ReadBassChannels();
    void SaveBassChannels();
    void CreateEmphasisSlider(int count);

public slots:
    void OnSliderReleased();
    void BassCh1ComboBoxIndexChanged(int);
    void BassCh2ComboBoxIndexChanged(int);

private slots:
    void on_flatPushButton_clicked();
    void on_resetPushButton_clicked();

signals:
    void SendCmd(QString);
    void BassChanged(int);
    void CenterChanged(int);
};

#endif // EMPHASISDIALOG_H
