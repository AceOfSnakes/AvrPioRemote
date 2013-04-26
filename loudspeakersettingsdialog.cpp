#include "loudspeakersettingsdialog.h"
#include "ui_loudspeakersettingsdialog.h"

LoudspeakerSettingsDialog::LoudspeakerSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoudspeakerSettingsDialog)
{
    ui->setupUi(this);

    connect(this, SIGNAL(SendCmd(QString)), parent, SLOT(SendCmd(QString)));
}

LoudspeakerSettingsDialog::~LoudspeakerSettingsDialog()
{
    delete ui;
}

void LoudspeakerSettingsDialog::Data(QString& data)
{

}

void LoudspeakerSettingsDialog::on_GetButton_clicked()
{
//    ?SUW
//    (data1)
//    (data2)
//    (data3)
//    <CR>

//     data0
//    MCACC Memory.
//    00:Current MEMORY
//    01:MEMORY 1
//    02:MEMORY 2
//    03:MEMORY 3
//    04:MEMORY 4
//    05:MEMORY 5

//    data2
//    L__:Front Left
//    R__:Front Right
//    C__:Center
//    SL_:Surround Left
//    SR_:Surround Right
//    SBL:Surround Back Left
//    SBR:Surround Back Right
//    LH_:Front Height Left
//    RH_:Front Height Right
//    LW_:Front Wide Left
//    RW_:Front Wide Right

//    data3
//    Frequency,Trim
//     00:63Hz
//     01:125Hz
//     02:250Hz
//     03:500Hz
//     04:1kHz
//     05:2kHz
//     06:4kHz
//     07:8kHz
//     08:16kHz
//     09:EQ Wide Trim
    emit SendCmd("?SUW00L__00");
}

void LoudspeakerSettingsDialog::RequestEQ(const QString& Memory, const QString& Loudspeaker, const QString& Frequency)
{
    emit SendCmd("");
}
