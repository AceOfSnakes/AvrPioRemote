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
    responseList << DisplayDataResponse_FL().getResponseID();
    responseList << PowerResponse_PWR_APR_BPR_ZEP().getResponseID();
    responseList << MuteResponse_MUT_Z2MUT_Z3MUT().getResponseID();
    responseList << VolumeResponse_VOL_ZV_YV().getResponseID();
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

        if (m_Comm.IsPioneer())
        {
            ui->MultiZoneButton->setVisible(true);

            ui->Z2InputComboBox->clear();
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

            ui->Z3InputComboBox->clear();
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
            ui->MultiZoneButton->setVisible(false);

            ui->Z2InputComboBox->clear();
            ui->Z2InputComboBox->addItem("CBL/SAT", 0x01);
            ui->Z2InputComboBox->addItem("GAME", 0x02);
            ui->Z2InputComboBox->addItem("AUX", 0x03);
            ui->Z2InputComboBox->addItem("BD/DVD", 0x10);
            ui->Z2InputComboBox->addItem("STR BOX", 0x11);
            ui->Z2InputComboBox->addItem("TV", 0x12);
            ui->Z2InputComboBox->addItem("PHONO", 0x22);
            ui->Z2InputComboBox->addItem("CD", 0x23);
            ui->Z2InputComboBox->addItem("FM", 0x24);
            ui->Z2InputComboBox->addItem("AM", 0x25);
            ui->Z2InputComboBox->addItem("TUNER", 0x26);
            ui->Z2InputComboBox->addItem("USB (Front)", 0x29);
            ui->Z2InputComboBox->addItem("NETWORK", 0x2B);
            ui->Z2InputComboBox->addItem("Bluetooth", 0x2E);
            ui->Z2InputComboBox->addItem("HDMI 5", 0x55);

            ui->Z3InputComboBox->clear();
            ui->Z3InputComboBox->addItem("CBL/SAT", 0x01);
            ui->Z3InputComboBox->addItem("GAME", 0x02);
            //ui->Z3InputComboBox->addItem("AUX", 0x03);
            ui->Z3InputComboBox->addItem("BD/DVD", 0x10);
            ui->Z3InputComboBox->addItem("STR BOX", 0x11);
            ui->Z3InputComboBox->addItem("TV", 0x12);
            ui->Z3InputComboBox->addItem("PHONO", 0x22);
            ui->Z3InputComboBox->addItem("CD", 0x23);
            ui->Z3InputComboBox->addItem("FM", 0x24);
            ui->Z3InputComboBox->addItem("AM", 0x25);
            ui->Z3InputComboBox->addItem("TUNER", 0x26);
            ui->Z3InputComboBox->addItem("USB (Front)", 0x29);
            ui->Z3InputComboBox->addItem("NETWORK", 0x2B);
            ui->Z3InputComboBox->addItem("Bluetooth", 0x2E);

            emit SendCmd("ZPWQSTN"); // z2 power
            emit SendCmd("SLZQSTN"); // z2 input
            emit SendCmd("ZVLQSTN"); // z2 volume
            emit SendCmd("ZMTQSTN"); // z2 mute

            emit SendCmd("PW3QSTN"); // z3 power
            emit SendCmd("SL3QSTN"); // z3 input
            emit SendCmd("VL3QSTN"); // z3 volume
            emit SendCmd("MT3QSTN"); // z3 mute
        }
        this->show();
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
    DisplayDataResponse_FL* display = dynamic_cast<DisplayDataResponse_FL*>(response);
    if (display != NULL)
    {
        if (m_Comm.IsPioneer() && display->getDisplayType() == 2)
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
    PowerResponse_PWR_APR_BPR_ZEP* power = dynamic_cast<PowerResponse_PWR_APR_BPR_ZEP*>(response);
    if (power != NULL)
    {
        bool on = power->IsPoweredOn();
        if (power->GetZone() == PowerResponse_PWR_APR_BPR_ZEP::Zone2)
        {
            ui->Z2PowerButton->setChecked(on);
            ui->Z2InputComboBox->setEnabled(on);
            ui->Z2VolumeUpButton->setEnabled(on);
            ui->Z2VolumeDownButton->setEnabled(on);
            ui->Z2MuteButton->setEnabled(on);
        }
        else if (power->GetZone() == PowerResponse_PWR_APR_BPR_ZEP::Zone3)
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
    MuteResponse_MUT_Z2MUT_Z3MUT* mute = dynamic_cast<MuteResponse_MUT_Z2MUT_Z3MUT*>(response);
    if (mute != NULL)
    {
        if (mute->GetZone() == MuteResponse_MUT_Z2MUT_Z3MUT::Zone2)
        {
            ui->Z2MuteButton->setChecked(mute->IsMuted());
            //qDebug() << "mute z2" << mute->IsMuted();
        }
        else if (mute->GetZone() == MuteResponse_MUT_Z2MUT_Z3MUT::Zone3)
        {
            ui->Z3MuteButton->setChecked(mute->IsMuted());
            //qDebug() << "mute z3" << mute->IsMuted();
        }
        return;
    }
    // zone volume
    VolumeResponse_VOL_ZV_YV* volume = dynamic_cast<VolumeResponse_VOL_ZV_YV*>(response);
    if (volume != NULL)
    {
        if (volume->GetZone() == VolumeResponse_VOL_ZV_YV::Zone2)
        {
            ui->Z2VolumeLineEdit->setText(volume->GetAsString());
        }
        else if (volume->GetZone() == VolumeResponse_VOL_ZV_YV::Zone3)
        {
            ui->Z3VolumeLineEdit->setText(volume->GetAsString());
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
    if (m_Comm.IsPioneer())
    {
        emit SendCmd("ZZ");
    }
}

void ZoneControlDialog::on_Z2ActivateButton_clicked()
{
    ui->Z2ActivateButton->setChecked(!ui->Z2ActivateButton->isChecked());
    if (m_Comm.IsPioneer())
    {
        emit SendCmd("APZ");
    }
}

void ZoneControlDialog::on_Z2PowerButton_clicked()
{
    bool checked = !ui->Z2PowerButton->isChecked();
    ui->Z2PowerButton->setChecked(checked);
    if (m_Comm.IsPioneer())
    {
        if (checked)
            emit SendCmd("APF");
        else
            emit SendCmd("APO");
    }
    else
    {
        if (checked)
            emit SendCmd("ZPW00");
        else
            emit SendCmd("ZPW01");
    }
}

void ZoneControlDialog::on_Z2InputComboBox_activated(int index)
{
    int input = ui->Z2InputComboBox->itemData(index).toInt();
    if (m_Comm.IsPioneer())
    {
        emit SendCmd(QString("%1ZS").arg(input, 2, 10, QLatin1Char('0')));
        emit SendCmd("?ZS"); // z2 input
    }
    else
    {
        emit SendCmd(QString::asprintf("SLZ%02X", input));
    }
}

void ZoneControlDialog::on_Z2VolumeUpButton_clicked()
{
    if (m_Comm.IsPioneer())
    {
        emit SendCmd("ZU");
    }
    else
    {
        emit SendCmd("ZVLUP");
    }
}

void ZoneControlDialog::on_Z2VolumeDownButton_clicked()
{
    if (m_Comm.IsPioneer())
    {
        emit SendCmd("ZD");
    }
    else
    {
        emit SendCmd("ZVLDOWN");
    }
}

void ZoneControlDialog::on_Z2MuteButton_clicked()
{
    bool checked = !ui->Z2MuteButton->isChecked();
    ui->Z2MuteButton->setChecked(checked);
    if (m_Comm.IsPioneer())
    {
        if (checked)
            emit SendCmd("Z2MF");
        else
            emit SendCmd("Z2MO");
    }
    else
    {
        emit SendCmd(checked ? "ZMT00" : "ZMT01");
    }
}

void ZoneControlDialog::on_Z3MuteButton_clicked()
{
    bool checked = !ui->Z3MuteButton->isChecked();
    ui->Z3MuteButton->setChecked(checked);
    if (m_Comm.IsPioneer())
    {
        if (checked)
            emit SendCmd("Z3MF");
        else
            emit SendCmd("Z3MO");
    }
    else
    {
        emit SendCmd(checked ? "MT300" : "MT301");
    }
}

void ZoneControlDialog::on_Z3PowerButton_clicked()
{
    bool checked = !ui->Z3PowerButton->isChecked();
    ui->Z3PowerButton->setChecked(checked);
    if (m_Comm.IsPioneer())
    {
        if (checked)
            emit SendCmd("BPF");
        else
            emit SendCmd("BPO");
    }
    else
    {
        if (checked)
            emit SendCmd("PW300");
        else
            emit SendCmd("PW301");
    }
}

void ZoneControlDialog::on_Z3InputComboBox_activated(int index)
{
    int input = ui->Z3InputComboBox->itemData(index).toInt();
    if (m_Comm.IsPioneer())
    {
        emit SendCmd(QString("%1ZT").arg(input, 2, 10, QLatin1Char('0')));
        emit SendCmd("?ZT"); // z3 input
    }
    else
    {
        emit SendCmd(QString::asprintf("SL3%02X", input));
    }
}

void ZoneControlDialog::on_Z3VolumeUpButton_clicked()
{
    if (m_Comm.IsPioneer())
    {
        emit SendCmd("YU");
    }
    else
    {
        emit SendCmd("VL3UP");
    }
}

void ZoneControlDialog::on_Z3VolumeDownButton_clicked()
{
    if (m_Comm.IsPioneer())
    {
        emit SendCmd("YD");
    }
    else
    {
        emit SendCmd("VL3DOWN");
    }
}
