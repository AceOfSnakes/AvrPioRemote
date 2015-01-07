#ifndef EMPHASISDIALOG_H
#define EMPHASISDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QComboBox>
#include <QSettings>
#include "labeledslider.h"

namespace Ui {
class EmphasisDialog;
}

class EmphasisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmphasisDialog(QWidget *parent, QSettings& settings);
    ~EmphasisDialog();

    void SetBass(int n);
    void SetCenter(int n);
    int GetBass();
    int GetCenter();
    QString GetChannelString();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EmphasisDialog *ui;
    QList<LabeledSlider*> m_EmphasisSliders;
    QSettings&          m_Settings;
    int                 m_BassCh1;
    int                 m_BassCh2;

    void ReadBassChannels();
    void SaveBassChannels();
    void CreateEmphasisSlider(int count);

public slots:
    void DataReceived(QString data);
    void OnSliderReleased();
    void BassCh1ComboBoxIndexChanged(int);
    void BassCh2ComboBoxIndexChanged(int);

private slots:
    void on_flatPushButton_clicked();

signals:
    void SendCmd(QString);
};

#endif // EMPHASISDIALOG_H
