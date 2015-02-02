#include "zonecontroldialog.h"
#include "ui_zonecontroldialog.h"
#include <QDebug>

ZoneControlDialog::ZoneControlDialog(QWidget *parent, QSettings &settings, ReceiverInterface &Comm) :
    QDialog(parent),
    ui(new Ui::ZoneControlDialog),
    m_Settings(settings),
    m_Comm(Comm),
    m_PositionSet(false)
{
    ui->setupUi(this);

    this->setFixedSize(this->size());

    // restore the position of the window
    if (m_Settings.value("SaveZoneControlWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("ZoneControlWindowGeometry").toByteArray());
    }

    ui->Z2ActivateButton->setVisible(false);
    ui->Z3ActivateButton->setVisible(false);

    ui->Z2InputComboBox->setEnabled(false);
    ui->Z2VolumeUpButton->setEnabled(false);
    ui->Z2VolumeDownButton->setEnabled(false);
    ui->Z2MuteButton->setEnabled(false);

    ui->Z3InputComboBox->setEnabled(false);
    ui->Z3VolumeUpButton->setEnabled(false);
    ui->Z3VolumeDownButton->setEnabled(false);
    ui->Z3MuteButton->setEnabled(false);

    ui->Z2InputComboBox->addItem("DVD", 4);
    ui->Z2InputComboBox->addItem("SAT/CBL", 6);
    ui->Z2InputComboBox->addItem("DVR/BDR", 15);
    ui->Z2InputComboBox->addItem("VIDEO", 10);
    ui->Z2InputComboBox->addItem("HMG/NETWORK", 26);
    ui->Z2InputComboBox->addItem("INTERNET RADIO", 38);
    ui->Z2InputComboBox->addItem("MEDIA SERVER", 44);
    ui->Z2InputComboBox->addItem("FAVORITES", 45);
    ui->Z2InputComboBox->addItem("iPod/USB", 17);
    ui->Z2InputComboBox->addItem("TV", 5);
    ui->Z2InputComboBox->addItem("CD", 1);
    ui->Z2InputComboBox->addItem("CD-R/TAPE", 3);
    ui->Z2InputComboBox->addItem("TUNER", 2);
    ui->Z2InputComboBox->addItem("ADAPTER PORT", 33);

    ui->Z3InputComboBox->addItem("DVD", 4);
    ui->Z3InputComboBox->addItem("SAT/CBL", 6);
    ui->Z3InputComboBox->addItem("DVR/BDR", 15);
    ui->Z3InputComboBox->addItem("VIDEO", 10);
    ui->Z3InputComboBox->addItem("HMG/NETWORK", 26);
    ui->Z3InputComboBox->addItem("INTERNET RADIO", 38);
    ui->Z3InputComboBox->addItem("MEDIA SERVER", 44);
    ui->Z3InputComboBox->addItem("FAVORITES", 45);
    ui->Z3InputComboBox->addItem("iPod/USB", 17);
    ui->Z3InputComboBox->addItem("TV", 5);
    ui->Z3InputComboBox->addItem("CD", 1);
    ui->Z3InputComboBox->addItem("CD-R/TAPE", 3);
    ui->Z3InputComboBox->addItem("TUNER", 2);
    ui->Z3InputComboBox->addItem("ADAPTER PORT", 33);

//    ui->Z4InputComboBox->addItem("BD", 25);
//    ui->Z4InputComboBox->addItem("DVD", 4);
//    ui->Z4InputComboBox->addItem("SAT/CBL", 6);
//    ui->Z4InputComboBox->addItem("DVR/BDR", 15);
//    ui->Z4InputComboBox->addItem("VIDEO", 10);
//    ui->Z4InputComboBox->addItem("HDMI 1", 19);
//    ui->Z4InputComboBox->addItem("HDMI 2", 20);
//    ui->Z4InputComboBox->addItem("HDMI 3", 21);
//    ui->Z4InputComboBox->addItem("HDMI 4", 22);
//    ui->Z4InputComboBox->addItem("HDMI 5", 23);
//    ui->Z4InputComboBox->addItem("MHL", 48);

    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));
    connect(&m_Comm,SIGNAL(ZoneInput(int, int)),this,SLOT(ZoneInput(int, int)));

    QStringList responseList;
    responseList << DisplayDataResponse().getResponseID();
    responseList << PowerResponse().getResponseID();
    responseList << MuteResponse().getResponseID();
    responseList << VolumeResponse().getResponseID();
    MsgDistributor::AddResponseListener(this, responseList);
}

ZoneControlDialog::~ZoneControlDialog()
{
    delete ui;
}

void ZoneControlDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("ZoneControlWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}

void ZoneControlDialog::ShowZoneControlDialog()
{
    if (!isVisible())
    {
        if (!m_PositionSet || !m_Settings.value("SaveZoneControlWindowGeometry", false).toBool())
        {
            QWidget* Parent = dynamic_cast<QWidget*>(parent());
            //int x = Parent->pos().x() + Parent->width() + 20;
            int x = Parent->pos().x() - Parent->width() - 100;
            QPoint pos;
            pos.setX(x);
            pos.setY(Parent->pos().y());
            this->move(pos);
        }
        this->show();
        emit SendCmd("?AP"); // z2 power
        emit SendCmd("?ZS"); // z2 input
        emit SendCmd("?ZV"); // z2 volume
        emit SendCmd("?Z2M"); // z2 mute

         if (!m_Settings.value("TunerCompatibilityMode").toBool())
         {
             emit SendCmd("?BP"); // z3 power
             //emit SendCmd("?ZEP"); // z4 power
             emit SendCmd("?ZT"); // z3 input
             //emit SendCmd("?ZEA"); // z4 input
             emit SendCmd("?YV"); // z3 volume
             emit SendCmd("?Z3M"); // z3 mute
         }
    }
    else
    {
        this->hide();
    }

}

// Receiver messages
void ZoneControlDialog::ResponseReceived(ReceivedObjectBase *response)
{
    // display data
    DisplayDataResponse* display = dynamic_cast<DisplayDataResponse*>(response);
    if (display != NULL)
    {
        if (display->getDisplayType() == 2)
        {
            bool zone2active = false;
            bool zone3active = false;
            QString data = display->getDisplayLine().trimmed();
            if (data.startsWith("ZONE ") && data.endsWith(" ON"))
            {
                if (data[5] == QChar('2'))
                    zone2active = true;
                if (data[5] == QChar('3') || data[5] == QChar('3'))
                    zone3active = true;
                ui->MultiZoneButton->setChecked(zone2active || zone3active);
                ui->Z2ActivateButton->setChecked(zone2active);
                ui->Z3ActivateButton->setChecked(zone3active);
            }
            else if (data.startsWith("MULTI ZONE ") && data.endsWith(" OFF"))
            {
                ui->MultiZoneButton->setChecked(false);
                ui->Z2ActivateButton->setChecked(false);
                ui->Z3ActivateButton->setChecked(false);
            }
        }
        return;
    }
    // zone power
    PowerResponse* power = dynamic_cast<PowerResponse*>(response);
    if (power != NULL)
    {
        bool on = power->IsPoweredOn();
        if (power->GetZone() == PowerResponse::Zone2)
        {
            ui->Z2PowerButton->setChecked(on);
            ui->Z2InputComboBox->setEnabled(on);
            ui->Z2VolumeUpButton->setEnabled(on);
            ui->Z2VolumeDownButton->setEnabled(on);
            ui->Z2MuteButton->setEnabled(on);
        }
        else if (power->GetZone() == PowerResponse::Zone3)
        {
            ui->Z3PowerButton->setChecked(on);
            if (!ui->Z3GroupBox->isEnabled())
                ui->Z3GroupBox->setEnabled(true);
            ui->Z3InputComboBox->setEnabled(on);
            ui->Z3VolumeUpButton->setEnabled(on);
            ui->Z3VolumeDownButton->setEnabled(on);
            ui->Z3MuteButton->setEnabled(on);
        }
        return;
    }
    // zone mute
    MuteResponse* mute = dynamic_cast<MuteResponse*>(response);
    if (mute != NULL)
    {
        if (mute->GetZone() == MuteResponse::Zone2)
        {
            ui->Z2MuteButton->setChecked(mute->IsMuted());
            qDebug() << "mute z2" << mute->IsMuted();
        }
        else if (mute->GetZone() == MuteResponse::Zone3)
        {
            ui->Z3MuteButton->setChecked(mute->IsMuted());
            qDebug() << "mute z3" << mute->IsMuted();
        }
        return;
    }
    // zone volume
    VolumeResponse* volume = dynamic_cast<VolumeResponse*>(response);
    if (volume != NULL)
    {
        if (volume->GetZone() == VolumeResponse::Zone2)
        {
            ui->Z2VolumeLineEdit->setText(volume->GetVolumeString());
        }
        else if (volume->GetZone() == VolumeResponse::Zone3)
        {
            ui->Z3VolumeLineEdit->setText(volume->GetVolumeString());
        }
        return;
    }
}


void ZoneControlDialog::ZoneInput (int zone, int input)
{
    QComboBox* box = NULL;
    if (zone == 2)
    {
        box = ui->Z2InputComboBox;
    }
    else if (zone == 3)
    {
        box = ui->Z3InputComboBox;
    }
    else
    {
        return;
    }
    for (int i = 0; i < box->count(); i++)
    {
        if (box->itemData(i).toInt() == input)
        {
            box->setCurrentIndex(i);
            break;
        }
    }
}

// GUI Controls

void ZoneControlDialog::on_MultiZoneButton_clicked()
{
    ui->MultiZoneButton->setChecked(!ui->MultiZoneButton->isChecked());
    emit SendCmd("ZZ");
}

void ZoneControlDialog::on_Z2ActivateButton_clicked()
{
    ui->Z2ActivateButton->setChecked(!ui->Z2ActivateButton->isChecked());
    emit SendCmd("APZ");
}

void ZoneControlDialog::on_Z2PowerButton_clicked()
{
    bool checked = !ui->Z2PowerButton->isChecked();
    ui->Z2PowerButton->setChecked(checked);
    if (checked)
        emit SendCmd("APF");
    else
        emit SendCmd("APO");
}

void ZoneControlDialog::on_Z2InputComboBox_activated(int index)
{
    int input = ui->Z2InputComboBox->itemData(index).toInt();
    emit SendCmd(QString("%1ZS").arg(input, 2, 10, QLatin1Char('0')));
    emit SendCmd("?ZS"); // z2 input
}

void ZoneControlDialog::on_Z2VolumeUpButton_clicked()
{
    emit SendCmd("ZU");
}

void ZoneControlDialog::on_Z2VolumeDownButton_clicked()
{
    emit SendCmd("ZD");
}

void ZoneControlDialog::on_Z2MuteButton_clicked()
{
    bool checked = !ui->Z2MuteButton->isChecked();
    ui->Z2MuteButton->setChecked(checked);
    if (checked)
        emit SendCmd("Z2MF");
    else
        emit SendCmd("Z2MO");
}

void ZoneControlDialog::on_Z3MuteButton_clicked()
{
    bool checked = !ui->Z3MuteButton->isChecked();
    ui->Z3MuteButton->setChecked(checked);
    if (checked)
        emit SendCmd("Z3MF");
    else
        emit SendCmd("Z3MO");
}

void ZoneControlDialog::on_Z3PowerButton_clicked()
{
    bool checked = !ui->Z3PowerButton->isChecked();
    ui->Z3PowerButton->setChecked(checked);
    if (checked)
        emit SendCmd("BPF");
    else
        emit SendCmd("BPO");
}

void ZoneControlDialog::on_Z3InputComboBox_activated(int index)
{
    int input = ui->Z3InputComboBox->itemData(index).toInt();
    emit SendCmd(QString("%1ZT").arg(input, 2, 10, QLatin1Char('0')));
    emit SendCmd("?ZT"); // z3 input
}

void ZoneControlDialog::on_Z3VolumeUpButton_clicked()
{
    emit SendCmd("YU");
}

void ZoneControlDialog::on_Z3VolumeDownButton_clicked()
{
    emit SendCmd("YD");
}
