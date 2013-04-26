#ifndef LOUDSPEAKERSETTINGSDIALOG_H
#define LOUDSPEAKERSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class LoudspeakerSettingsDialog;
}

class LoudspeakerSettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LoudspeakerSettingsDialog(QWidget *parent = 0);
    ~LoudspeakerSettingsDialog();
    
private:
    Ui::LoudspeakerSettingsDialog *ui;

    void RequestEQ(const QString& Memory, const QString& Loudspeaker, const QString& Frequency);
public slots:
    void Data(QString& data);
signals:
    void SendCmd(QString data);

private slots:
    void on_GetButton_clicked();
};

#endif // LOUDSPEAKERSETTINGSDIALOG_H
