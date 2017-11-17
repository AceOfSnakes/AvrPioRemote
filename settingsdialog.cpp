/*
 * AVRPioRemote
 * Copyright (C) 2013  Andreas Müller, Ulrich Mensfeld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent, QSettings& settings, ReceiverInterface &Comm, PlayerInterface &CommBD) :
    QDialog(parent),
    m_Settings(settings),
    m_IpValidator(0, 255, this),
    m_IpPortValidator(0, 99999, this),
    m_Comm(Comm),
    m_CommBD(CommBD),
    ui(new Ui::SettingsDialog),
    m_AutoSearchDialog(NULL)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);

    ui->TunerVSX922CompatibilityModeCheckBox->setChecked(m_Settings.value("TunerCompatibilityMode", false).toBool());
    ui->VSX5xxCompatibilityModeCheckBox->setChecked(m_Settings.value("VSX5xxCompatibilityMode", false).toBool());

    //this->setFixedSize(this->size());
    this->setModal(true);

    QString lang = m_Settings.value("Language", "auto").toString();
    if (lang == "auto")
    {
        ui->LanguageAutoRadioButton->setChecked(true);
    }
    else if (lang.startsWith("de"))
    {
        ui->LanguageGermanRadioButton->setChecked(true);
    }
    else if (lang.startsWith("ru"))
    {
        ui->LanguageRussianRadioButton->setChecked(true);
    }
    else
    {
        ui->LanguageEnglishRadioButton->setChecked(true);
    }
    ui->ShowNetRadioCheckBox->setChecked(m_Settings.value("AutoShowNetRadio", true).toBool());
    ui->ShowTunerCheckBox->setChecked(m_Settings.value("AutoShowTuner", true).toBool());
    ui->ShowUSBCheckBox->setChecked(m_Settings.value("AutoShowUSB", true).toBool());
    ui->ShowBlueRayWindowCheckBox->setChecked(m_Settings.value("AutoShowBlueRay", false).toBool());

    //ui->StartLoggingInTestWindowCheckBox->setChecked(m_Settings.value("StartLoggingInTestWindow", false).toBool());

    ui->RestoreMainWindowCheckBox->setChecked(m_Settings.value("SaveMainWindowGeometry", true).toBool());
    ui->RestoreEQWindowCheckBox->setChecked(m_Settings.value("SaveEQWindowGeometry", false).toBool());
    ui->RestoreTunerWindowCheckBox->setChecked(m_Settings.value("SaveTunerWindowGeometry", false).toBool());
    ui->RestoreNetRadioWindowCheckBox->setChecked(m_Settings.value("SaveNetRadioWindowGeometry", false).toBool());
    ui->RestoreSpeakerSettingsWindowCheckBox->setChecked(m_Settings.value("SaveLSSettingsWindowGeometry", false).toBool());
    //ui->RestoreTestWindowCheckBox->setChecked(m_Settings.value("SaveTestWindowGeometry", false).toBool());
    ui->RestoreLMSettingsWindowCheckBox->setChecked(m_Settings.value("SaveLMSettingsWindowGeometry", false).toBool());
    ui->RestoreUSBWindowCheckBox->setChecked(m_Settings.value("SaveUsbWindowGeometry", false).toBool());
    ui->RestoreZoneControlWindowCheckBox->setChecked(m_Settings.value("SaveZoneControlWindowGeometry", false).toBool());
    ui->RestoreBlueRayWindowCheckBox->setChecked(m_Settings.value("SaveBlueRayWindowGeometry", false).toBool());
    ui->RestoreAVSettingsWindowCheckBox->setChecked(m_Settings.value("SaveAVSettingsWindowGeometry", false).toBool());
    ui->RestoreMCACCEQWindowCheckBox->setChecked(m_Settings.value("SaveMCACCEQWindowGeometry", false).toBool());
    ui->MinimizeToTrayCheckBox->setChecked(m_Settings.value("MinimizeToTrayCheckBox", false).toBool());

    ui->ShowReceiverNameInTitleCheckBox->setChecked(m_Settings.value("ShowReceiverNameInTitle", true).toBool());
    ui->ShowDefaultInputNameCheckBox->setChecked(m_Settings.value("ShowDefaultInputName", false).toBool());
    ui->BlackThemeCheckBox->setChecked(m_Settings.value("UseBlackTheme", true).toBool());

    // configure ip adress edit input
    ui->lineEditIP1->setValidator(&m_IpValidator);
    ui->lineEditIP2->setValidator(&m_IpValidator);
    ui->lineEditIP3->setValidator(&m_IpValidator);
    ui->lineEditIP4->setValidator(&m_IpValidator);
    ui->lineEditIPPort->setValidator(&m_IpPortValidator);
    // configure player ip adress edit input
    ui->lineEditIP1_BD->setValidator(&m_IpValidator);
    ui->lineEditIP2_BD->setValidator(&m_IpValidator);
    ui->lineEditIP3_BD->setValidator(&m_IpValidator);
    ui->lineEditIP4_BD->setValidator(&m_IpValidator);
    ui->lineEditIPPort_BD->setValidator(&m_IpPortValidator);
    // get the saved ip address data
    ui->lineEditIP1->setText(m_Settings.value("IP/1", "192").toString());
    ui->lineEditIP2->setText(m_Settings.value("IP/2", "168").toString());
    ui->lineEditIP3->setText(m_Settings.value("IP/3", "1").toString());
    ui->lineEditIP4->setText(m_Settings.value("IP/4", "1").toString());
    ui->lineEditIPPort->setText(m_Settings.value("IP/PORT", "8102").toString());
    ((m_Settings.value("IP/IsPioneer", true).toBool()) ? ui->radioButtonVendorPioneer : ui->radioButtonVendorOnkyo)->setChecked(true);
    // get the saved player ip address data
    ui->lineEditIP1_BD->setText(m_Settings.value("Player_IP/1", "192").toString());
    ui->lineEditIP2_BD->setText(m_Settings.value("Player_IP/2", "168").toString());
    ui->lineEditIP3_BD->setText(m_Settings.value("Player_IP/3", "1").toString());
    ui->lineEditIP4_BD->setText(m_Settings.value("Player_IP/4", "1").toString());
    ui->lineEditIPPort_BD->setText(m_Settings.value("Player_IP/PORT", "8102").toString());


    connect((&m_Comm), SIGNAL(Connected()), this, SLOT(CommConnected()));
    connect((&m_Comm), SIGNAL(Disconnected()), this, SLOT(CommDisconnected()));
    connect((&m_Comm), SIGNAL(CommError(QString)), this,  SLOT(CommError(QString)));
    connect((this), SIGNAL(onConnect()), parent,  SLOT(onConnect()));

    connect((&m_CommBD), SIGNAL(Connected()), this, SLOT(CommConnectedBD()));
    connect((&m_CommBD), SIGNAL(Disconnected()), this, SLOT(CommDisconnectedBD()));
    connect((&m_CommBD), SIGNAL(CommError(QString)), this,  SLOT(CommErrorBD(QString)));
    connect((this), SIGNAL(onConnectBD()), parent,  SLOT(onConnectBD()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}


void SettingsDialog::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void SettingsDialog::ShowSettingsDialog()
{
    if (!this->isVisible())
    {
        //        QWidget* Parent = dynamic_cast<QWidget*>(parent());
        //        int x = this->pos().x() + this->width() + 20;
        //        QPoint pos;
        //        pos.setX(x);
        //        pos.setY(this->pos().y());
        //        m_SettingsDialog->move(pos);
        this->show();
    }
}

void SettingsDialog::GetIpAddress(QString& ip1, QString& ip2, QString& ip3, QString& ip4, QString& port, bool& is_pioneer)
{
    ip1 = ui->lineEditIP1->text().trimmed();
    ip2 = ui->lineEditIP2->text().trimmed();
    ip3 = ui->lineEditIP3->text().trimmed();
    ip4 = ui->lineEditIP4->text().trimmed();
    port = ui->lineEditIPPort->text().trimmed();
    is_pioneer = ui->radioButtonVendorPioneer->isChecked();
}

void SettingsDialog::GetIpAddressBD(QString& ip1, QString& ip2, QString& ip3, QString& ip4, QString& port)
{
    ip1 = ui->lineEditIP1_BD->text().trimmed();
    ip2 = ui->lineEditIP2_BD->text().trimmed();
    ip3 = ui->lineEditIP3_BD->text().trimmed();
    ip4 = ui->lineEditIP4_BD->text().trimmed();
    port = ui->lineEditIPPort_BD->text().trimmed();
}

void SettingsDialog::SetIpAddress(QString ip1, QString ip2, QString ip3, QString ip4, QString port, bool is_pioneer)
{
    ui->lineEditIP1->setText(ip1);
    ui->lineEditIP2->setText(ip2);
    ui->lineEditIP3->setText(ip3);
    ui->lineEditIP4->setText(ip4);
    ui->lineEditIPPort->setText(port);
    ((is_pioneer) ? ui->radioButtonVendorPioneer : ui->radioButtonVendorOnkyo)->setChecked(true);
}

void SettingsDialog::SetIpAddressBD(QString ip1, QString ip2, QString ip3, QString ip4, QString port)
{
    ui->lineEditIP1_BD->setText(ip1);
    ui->lineEditIP2_BD->setText(ip2);
    ui->lineEditIP3_BD->setText(ip3);
    ui->lineEditIP4_BD->setText(ip4);
    ui->lineEditIPPort_BD->setText(port);
}

void SettingsDialog::EnableIPInput(bool enable)
{
    ui->lineEditIP1->setEnabled(enable);
    ui->lineEditIP2->setEnabled(enable);
    ui->lineEditIP3->setEnabled(enable);
    ui->lineEditIP4->setEnabled(enable);
    ui->lineEditIPPort->setEnabled(enable);
}
void SettingsDialog::EnableIPInputBD(bool enable)
{
    ui->lineEditIP1_BD->setEnabled(enable);
    ui->lineEditIP2_BD->setEnabled(enable);
    ui->lineEditIP3_BD->setEnabled(enable);
    ui->lineEditIP4_BD->setEnabled(enable);
    ui->lineEditIPPort_BD->setEnabled(enable);
}

void SettingsDialog::CommError(QString/* socketError*/)
{
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonAuto->setEnabled(true);
    ui->pushButtonConnect->setText(tr("Connect"));
}

void SettingsDialog::CommConnected()
{
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonAuto->setEnabled(false);
    ui->pushButtonConnect->setText(tr("Disconnect"));
    ui->pushButtonConnect->setChecked(true);
}

void SettingsDialog::CommDisconnected()
{
    ui->pushButtonConnect->setText(tr("Connect"));
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonAuto->setEnabled(true);
}
void SettingsDialog::CommErrorBD(QString/* socketError*/)
{
    ui->pushButtonConnect_BD->setEnabled(true);
    ui->pushButtonAuto_BD->setEnabled(true);
    ui->pushButtonConnect_BD->setText(tr("Connect to Player"));
}

void SettingsDialog::CommConnectedBD()
{
    ui->pushButtonConnect_BD->setEnabled(true);
    ui->pushButtonAuto_BD->setEnabled(false);
    ui->pushButtonConnect_BD->setText(tr("Disconnect"));
    ui->pushButtonConnect_BD->setChecked(true);
}

void SettingsDialog::CommDisconnectedBD()
{
    ui->pushButtonConnect_BD->setText(tr("Connect to Player"));
    ui->pushButtonConnect_BD->setEnabled(true);
    ui->pushButtonAuto_BD->setEnabled(true);
}
void SettingsDialog::on_TunerVSX922CompatibilityModeCheckBox_stateChanged(int state)
{
    m_Settings.setValue("TunerCompatibilityMode", (Qt::Checked == state));
}


void SettingsDialog::on_VSX5xxCompatibilityModeCheckBox_stateChanged(int state)
{
    m_Settings.setValue("VSX5xxCompatibilityMode", (Qt::Checked == state));
}


void SettingsDialog::on_LanguageAutoRadioButton_clicked(bool checked)
{
    if (checked)
    {
        m_Settings.setValue("Language", "auto");
        SetLanguage();
    }
}


void SettingsDialog::on_LanguageEnglishRadioButton_clicked(bool checked)
{
    if (checked)
    {
        m_Settings.setValue("Language", "en");
        SetLanguage();
    }
}


void SettingsDialog::on_LanguageGermanRadioButton_clicked(bool checked)
{
    if (checked)
    {
        m_Settings.setValue("Language", "de");
        SetLanguage();
    }
}


void SettingsDialog::SetLanguage()
{
    QString lang = m_Settings.value("Language", "auto").toString();
    if (lang == "auto")
    {
        lang = QLocale::system().name();
    }
    if (lang.startsWith("de"))
    {
        m_Translater.load(QString::fromUtf8(":/new/prefix1/avrpioremote_de"));
    }
    else if (lang.startsWith("ru"))
    {
        m_Translater.load(QString::fromUtf8(":/new/prefix1/avrpioremote_ru"));
    }
    else
    {
        m_Translater.load(QString::fromUtf8(":/new/prefix1/avrpioremote_en"));
    }
    QCoreApplication::installTranslator(&m_Translater);

}

void SettingsDialog::on_radioButtonVendorPioneer_clicked()
{
    m_Settings.setValue("Player_IP/TypePioneer", true);
}

void SettingsDialog::on_radioButtonVendorOnkyo_clicked()
{
    m_Settings.setValue("Player_IP/TypePioneer", false);
}

void SettingsDialog::on_pushButton_clicked()
{
    close();
}


void SettingsDialog::on_ShowTunerCheckBox_clicked()
{
    m_Settings.setValue("AutoShowTuner", ui->ShowTunerCheckBox->isChecked());
}

void SettingsDialog::on_ShowNetRadioCheckBox_clicked()
{
    m_Settings.setValue("AutoShowNetRadio", ui->ShowNetRadioCheckBox->isChecked());
}

void SettingsDialog::on_RestoreMainWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveMainWindowGeometry", ui->RestoreMainWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreEQWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveEQWindowGeometry", ui->RestoreEQWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreTunerWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveTunerWindowGeometry", ui->RestoreTunerWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreNetRadioWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveNetRadioWindowGeometry", ui->RestoreNetRadioWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreSpeakerSettingsWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveLSSettingsWindowGeometry", ui->RestoreSpeakerSettingsWindowCheckBox->isChecked());
}
/*
void SettingsDialog::on_RestoreTestWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveTestWindowGeometry", ui->RestoreTestWindowCheckBox->isChecked());
}
*/
void SettingsDialog::on_RestoreLMSettingsWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveLMSettingsWindowGeometry", ui->RestoreLMSettingsWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreUSBWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveUsbWindowGeometry", ui->RestoreUSBWindowCheckBox->isChecked());
}

void SettingsDialog::on_ShowUSBCheckBox_clicked()
{
    m_Settings.setValue("AutoShowUSB", ui->ShowUSBCheckBox->isChecked());
}
/*
void SettingsDialog::on_StartLoggingInTestWindowCheckBox_clicked()
{
    m_Settings.setValue("StartLoggingInTestWindow", ui->StartLoggingInTestWindowCheckBox->isChecked());
}
*/
void SettingsDialog::on_ShowReceiverNameInTitleCheckBox_clicked()
{
    m_Settings.setValue("ShowReceiverNameInTitle", ui->ShowReceiverNameInTitleCheckBox->isChecked());
}

void SettingsDialog::on_ShowDefaultInputNameCheckBox_clicked()
{
    m_Settings.setValue("ShowDefaultInputName", ui->ShowDefaultInputNameCheckBox->isChecked());
}

void SettingsDialog::on_BlackThemeCheckBox_clicked()
{
    m_Settings.setValue("UseBlackTheme", ui->BlackThemeCheckBox->isChecked());
}

void SettingsDialog::on_RestoreZoneControlWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveZoneControlWindowGeometry", ui->RestoreZoneControlWindowCheckBox->isChecked());
}

void SettingsDialog::on_pushButtonConnect_clicked()
{
    bool checked = !ui->pushButtonConnect->isChecked();
    ui->pushButtonConnect->setChecked(checked);
    if (!checked)
    {
        ui->pushButtonConnect->setEnabled(false);
        ui->pushButtonAuto->setEnabled(true);
    }
    emit onConnect();
}

void SettingsDialog::on_ShowBlueRayWindowCheckBox_clicked()
{
    m_Settings.setValue("AutoShowBlueRay", ui->ShowBlueRayWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreBlueRayWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveBlueRayWindowGeometry", ui->RestoreBlueRayWindowCheckBox->isChecked());
}

void SettingsDialog::on_RestoreAVSettingsWindowCheckBox_clicked()
{
    m_Settings.setValue("SaveAVSettingsWindowGeometry", ui->RestoreAVSettingsWindowCheckBox->isChecked());
}

void SettingsDialog::on_LanguageRussianRadioButton_clicked(bool checked)
{
    if (checked)
    {
        m_Settings.setValue("Language", "ru");
        SetLanguage();
    }
}

void SettingsDialog::on_RestoreMCACCEQWindowCheckBox_clicked(bool checked)
{
    m_Settings.setValue("SaveMCACCEQWindowGeometry", checked);
}

void SettingsDialog::on_pushButtonAuto_clicked()
{
    do
    {
        delete m_AutoSearchDialog;
        m_AutoSearchDialog = new AutoSearchDialog(m_Settings, this);
        m_AutoSearchDialog->exec();
    } while(m_AutoSearchDialog->m_Result == 2);
    if (m_AutoSearchDialog->m_Result == 1)
    {
        QString ip = m_AutoSearchDialog->m_SelectedAddress;
        int port = m_AutoSearchDialog->m_SelectedPort;
        QStringList l = ip.split(QRegExp("[.]"), QString::SkipEmptyParts);
        if (l.size() == 4)
        {
            SetIpAddress(l[0], l[1], l[2], l[3], QString("%1").arg(port), m_AutoSearchDialog->m_SelectedPioneer);
        }
        //m_Settings.setValue("Player_IP/TypePioneer", m_AutoSearchDialog->m_SelectedPioneer);
    }
    delete m_AutoSearchDialog;
    m_AutoSearchDialog = NULL;
}

void SettingsDialog::on_pushButtonAuto_BD_clicked()
{
//    qDebug()<<"m_CommBD.m_ping_commands"<<m_CommBD.m_ping_commands;
    do
    {
        delete m_AutoSearchDialog;
        m_AutoSearchDialog = new AutoSearchDialog(m_Settings, this, false,m_CommBD.m_ping_commands.at(0).toString(),
                                                  m_CommBD.m_PlayerSettings.value("pingResponseOk").toString(),
                                                  m_CommBD.m_PlayerSettings.value("pingResponseErr").toString(),
                                                  m_CommBD.m_PlayerSettings.value("prefferedPort",8102).toInt(),
                                                  m_CommBD.m_PlayerSettings.value("crlf",true).toBool());

        m_AutoSearchDialog->exec();
    } while(m_AutoSearchDialog->m_Result == 2);
    if (m_AutoSearchDialog->m_Result == 1)
    {
        QString ip = m_AutoSearchDialog->m_SelectedAddress;
        int port = m_AutoSearchDialog->m_SelectedPort;
        QStringList l = ip.split(QRegExp("[.]"), QString::SkipEmptyParts);
        if (l.size() == 4)
        {
            SetIpAddressBD(l[0], l[1], l[2], l[3], QString("%1").arg(port));
        }

    }
    delete m_AutoSearchDialog;
    m_AutoSearchDialog = NULL;
}

void SettingsDialog::on_pushButtonConnect_BD_clicked()
{
    bool checked = !ui->pushButtonConnect_BD->isChecked();
    ui->pushButtonConnect_BD->setChecked(checked);
    if (!checked)
    {
        ui->pushButtonConnect_BD->setEnabled(false);
        ui->pushButtonAuto_BD->setEnabled(true);
    }
    emit onConnectBD();
}


void SettingsDialog::on_MinimizeToTrayCheckBox_clicked()
{
    m_Settings.setValue("MinimizeToTrayCheckBox", ui->MinimizeToTrayCheckBox->isChecked());
    emit minimizeToTrayChanged();
}

void SettingsDialog::on_pushBDSettingsLoadButton_clicked()
{
    QString xfile = QFileDialog::getOpenFileName(this, tr("Load Player Settings"),
                                                 NULL, tr("Json Files").append(" (*.json);;").append(tr(
                                                                                                         "All files")).append(" (* *.*)"));

    if (!xfile.isEmpty()) {
        QFile file(xfile);
        QByteArray array;
        if (file.open(QIODevice::ReadOnly)) {
            array = file.readAll();
            QJsonDocument document = QJsonDocument::fromJson(array);
            file.close();
            m_CommBD.reloadPlayerSettings(QJsonObject::fromVariantMap(document.toVariant().toMap()).toVariantMap());
        }
    }
}
