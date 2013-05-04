#include "loudspeakersettingsdialog.h"
#include "ui_loudspeakersettingsdialog.h"
#include "QDebug"
#include <receiverinterface.h>
#include <QFile>
#include <QDomDocument>
#include <QDir>

const char* LStyp[] = {
    "Normal(SB/FH)", //0
    "Normal(SB/FW)",
    "Speaker B",
    "Front Bi-Amp",
    "Zone 2",
    "9.1ch FH/FW",
    "7.1ch+Speaker B",
    "7.1ch Front Bi-Amp",
    "7.1ch+ZONE2",
    "7.1ch FH/FW+ZONE2",
    "5.1ch Bi-Amp+ZONE2",
    "5.1ch+ZONE 2+3",
    "5.1ch+SP-B Bi-Amp",
    "5.1ch F+Surr Bi-Amp",
    "5.1ch F+C Bi-Amp", //14
    };

const int LSwert[] = {    //codewert des LSTyps im AVR
    0,1,2,3,4,10,11,12,13,14,15,16,17,18,19,
    };




LoudspeakerSettingsDialog::LoudspeakerSettingsDialog(QWidget *parent, QSettings &settings,ReceiverInterface &Comm ) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::LoudspeakerSettingsDialog),
    m_Comm(Comm)


{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    // communication
    connect(&m_Comm, SIGNAL(DataReceived(QString)), this, SLOT(DataReceived(QString)));
    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));
    connect(&m_Comm,SIGNAL(ErrorData(int)),this,SLOT(error(int))); //Fehler abfangen beim setzen der LSConfig


    for(int i=0;i<14;i++) //LS-Konfig belegen
        ui->LSsystem->addItem(LStyp[i]);

    QStringList spmode; //LS-Modus
    spmode << "Large" << "Small" << "No";
    ui->speakermode->addItems(spmode);

    QStringList mspeaker; //LS-Paare vorbelegen
    mspeaker << "Front" << "Center" << "FrontHigh" << "Surround" << "Surr. Back" << "Subwoofer";
    ui->speaker->addItems(mspeaker);

}



LoudspeakerSettingsDialog::~LoudspeakerSettingsDialog()
{
    delete ui;
}

void LoudspeakerSettingsDialog::Data(QString& data)
{
//    qDebug() << "in data";
}



void LoudspeakerSettingsDialog::RequestEQ(const QString& Memory, const QString& Loudspeaker, const QString& Frequency)
{
//    emit SendCmd("");
}

void LoudspeakerSettingsDialog::on_LSsystem_currentIndexChanged(int index)
{
//    qDebug() << index;
//    qDebug() << "in indexchanged";

}




void LoudspeakerSettingsDialog::DataReceived(QString data)
{
   int sysValue = 0;
   if (data.startsWith("SSF"))
    {
        sysValue=data.mid(3,2).toInt();
        ui->LSsystem->setCurrentIndex(sysValue);//aktuelle Konfig
        mVal=sysValue;  //in public speichern zum Vergleich beim setzen
//        qDebug() << "ssfdaten:" <<data;
   }

}


void LoudspeakerSettingsDialog::ShowLoudspeakerSettingsDialog()
{
    if (!isVisible())
    {
        QWidget* Parent = dynamic_cast<QWidget*>(parent());
        int x = Parent->pos().x() + Parent->width() + 20;
        QPoint pos;
        pos.setX(x);
        pos.setY(Parent->pos().y());
        this->move(pos);
        this->show();
    }
    SendCmd("?SSF");
//    qDebug()<< "sending ssf";
}

void LoudspeakerSettingsDialog::on_SetBut_clicked()
{
    QString str;
    int i=LSwert[ui->LSsystem->currentIndex()];
    if (i != mVal)
    {
      str = QString("%1SSF").arg(i);
      if (str.size()<5)
          str="0"+str;
      SendCmd(str);
      mVal=i;
    }

//    qDebug() <<str;
}

void LoudspeakerSettingsDialog::error(int i)
{
    SendCmd("?SSF");   //Wert nicht angenommen, korrigieren in DataReceived(QString data)
}


void LoudspeakerSettingsDialog::on_SetBut2_clicked()
{

}
