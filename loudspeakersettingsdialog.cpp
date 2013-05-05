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

const char* LSpaar[] = {    //Codierung der Speaker für Modus Large/Small/NO setzen
    "00",   //Front         // dazu korrespondiert mLSpaar mit den aktuellen Moduswerten
    "01",   //Center        // problem: SW und SB haben Sonderbedingungen, noch abfangen
    "02",   //FH
    "03",   //FW
    "04",   //surr
    "05",   //SB
    "06",   //SW
    };

const char* channels[]={
    "L__",
    "R__",
    "C__",
    "SL_",
    "SR_",
    "SBL",
    "SBR",
    "SW_",
    "LH_",
    "RH_",
    "LW_",
    "RW_",
};

LoudspeakerSettingsDialog::LoudspeakerSettingsDialog(QWidget *parent, QSettings &settings,ReceiverInterface &Comm ) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::LoudspeakerSettingsDialog),
    m_Comm(Comm)


{
    bool x922;
    int lsanz=14;

    ui->setupUi(this);
    this->setFixedSize(this->size());

    // communication
//    connect(&m_Comm, SIGNAL(DataReceived(QString)), this, SLOT(SpeakerReceived(QString)));
    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));
    connect(&m_Comm,SIGNAL(ErrorData(int)),this,SLOT(error(int))); //Fehler abfangen beim setzen der LSConfig
    connect(&m_Comm,SIGNAL(SpeakerData(QString)),this,SLOT(Speakerinfo(QString))); //Fehler abfangen beim setzen der LSConfig

    x922=m_Settings.value("TunerCompatibilityMode").toBool();
    if (x922)   //Im Kompatmodus nur verfügbare Typen anzeigen
        lsanz=5;

    for(int i=0;i<lsanz;i++) //LS-Konfig belegen
        ui->LSsystem->addItem(LStyp[i]);

/*    QStringList spmode; //LS-Modus
    spmode << "Small" << "Large" << "No";
    ui->speakermode->addItems(spmode);
*/

    QStringList mspeaker; //LS-Paare vorbelegen
    mspeaker << "Front" << "Center" << "FrontHigh" << "FrontWide" << "Surround" << "Surr. Back" << "Subwoofer";
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



void LoudspeakerSettingsDialog::Speakerinfo(QString data)

{
    QString str;
    int wert=0;
    int sysValue = 0;
   if (data.startsWith("SSF"))
    {
        sysValue=data.mid(3,2).toInt();
        ui->LSsystem->setCurrentIndex(sysValue);//aktuelle Konfig
        mVal=sysValue;  //in public speichern zum Vergleich beim setzen
     }
   if (data.startsWith("SSG"))
    {
        sysValue=data.mid(3,2).toInt();
        wert=data.mid(5,1).toInt();
        mLSpaar[sysValue]=wert; //Werte der einzelnen Speaker (Large, NO etc)
//       qDebug() << "wertepaar" << sysValue <<"array" <<mLSpaar[sysValue];
        wert=ui->speaker->currentIndex();
        ui->speakermode->setCurrentIndex(mLSpaar[wert]);
   }
   if (data.startsWith("CLV"))
    {
      sysValue=data.mid(6,2).toInt();
      str=data.mid(3,3);
      for (int i=0;i<12;i++)
      {
          if(str==channels[i])
              mchannels[i]=sysValue;
//          qDebug() <<"arraywert mchannels: " << channels[i] << mchannels[i];
      }
   }
//  qDebug() << "speakerdaten:" <<data  <<sysValue <<wert;
}



/*void LoudspeakerSettingsDialog::SpeakerReceived(QString data)
{
//    qDebug() << "Aus speakerinfo: " <<data;
}*/


void LoudspeakerSettingsDialog::ShowLoudspeakerSettingsDialog()
{
    QString str;
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
    for (int i=0;i<7;i++)
    {
        str=LSpaar[i];
        str="?SSG"+str;
        SendCmd(str);
//    qDebug()<< "sending init:" << str;
    }

// hier aktuellen Channel Level anfordern

    for (int i=0;i<12;i++)
    {

            str=QString("?%1CLV").arg(channels[i]);
//            qDebug() << str <<"  CLVDaten";
            SendCmd(str);
    }

}


void LoudspeakerSettingsDialog::on_SetBut_clicked()
{
    QString str;
    int i=LSwert[ui->LSsystem->currentIndex()];
    errflag=0;
    if (i != mVal)
    {
      str = QString("%1SSF").arg(i);
      if (str.size()<5)
          str="0"+str;
      SendCmd(str);
      mVal=i;
      errflag=1;
    }

//    qDebug() <<str;
}

void LoudspeakerSettingsDialog::error(int i)
{
// da keine weitere Unterscheidung hier möglich, generell setzen
    if (errflag>0)
    {
        SendCmd("?SSF");   //Wert nicht angenommen, korrigieren in DataReceived(QString data)
        errflag--;
    }
        ui->speakermode->setCurrentIndex(mLSpaar[ui->speaker->currentIndex()]);

}


void LoudspeakerSettingsDialog::on_SetBut2_clicked()
{
    QString str;
    str=LSpaar[ui->speaker->currentIndex()];
    str=str+QString("%1SSG").arg(ui->speakermode->currentIndex());
//  qDebug() <<"Settings new" <<str;
    SendCmd(str);
}

void LoudspeakerSettingsDialog::on_speaker_currentIndexChanged(int index)
{
    int mindex=index;

    QStringList spmode;
   for (int i=4; i<=0;i--)
       ui->speakermode->removeItem(i);

    if (mindex==6)
        {

        spmode << "Small" << "Plus" << "No" <<"" <<"";
        ui->speakermode->insertItems(0,spmode);
        }
    else if (mindex==5)
        {
            spmode << "Small*2" << "Large*1" << "Large*2" <<"No" <<"Small*1";
            ui->speakermode->insertItems(0,spmode);
        }

    else if (mindex <5)
        {
        spmode << "Small" << "Large" << "No" <<"" << "";
            ui->speakermode->insertItems(0,spmode);
        }
    else if (mindex ==0)
        {
        spmode << "Small" << "Large" << "" <<"" << "";
            ui->speakermode->insertItems(0,spmode);
        }
    ui->speakermode->setCurrentIndex(mLSpaar[mindex]); //Modus nach Anzeige LS ändern aus Array


    //   qDebug() << "changed" <<index;
}
