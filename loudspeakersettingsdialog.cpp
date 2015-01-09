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
#include "loudspeakersettingsdialog.h"
#include "ui_loudspeakersettingsdialog.h"
#include "QDebug"
#include "receiverinterface.h"


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
    "01",   //Center
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

const char* slchannels[]={
    "ui->sfl",
    "ui->sfr",
    "ui->sc",
    "ui->ssl",
    "ui->ssr",
    "ui->ssbl",
    "ui->ssbr",
    "ui->ssw",
    "ui->sfhl",
    "ui->sfhr",
    "ui->sfwl",
    "ui->sfwr",
};

LoudspeakerSettingsDialog::LoudspeakerSettingsDialog(QWidget *parent, QSettings &settings,ReceiverInterface &Comm ) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::LoudspeakerSettingsDialog),
    m_Comm(Comm),
    m_PositionSet(false)
{
    errflag=0;
    bool x922;
    int lsanz=14;

    ui->setupUi(this);
    this->setFixedSize(this->size());

    // restore the position of the window
    if (m_Settings.value("SaveLSSettingsWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("LSSettingsWindowGeometry").toByteArray());
    }

    // communication
//    connect(&m_Comm, SIGNAL(DataReceived(QString)), this, SLOT(SpeakerReceived(QString)));
    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));
    connect(&m_Comm,SIGNAL(ErrorData(int)),this,SLOT(error(int))); //Fehler abfangen beim setzen der LSConfig
    connect(&m_Comm,SIGNAL(SpeakerData(QString)),this,SLOT(Speakerinfo(QString)));
    connect(&m_Comm,SIGNAL(MCACC(int)),this,SLOT(MCACC(int)));

    connect(ui->sfl,SIGNAL(sliderReleased()), this, SLOT(ValueChanged()));
    connect(ui->sfr,SIGNAL(sliderReleased()), this, SLOT(ValueChanged()));
    connect(ui->sc,SIGNAL(sliderReleased()), this, SLOT(ValueChanged()));
    connect(ui->ssl,SIGNAL(sliderReleased()), this, SLOT(ValueChanged()));
    connect(ui->ssr,SIGNAL(sliderReleased()), this, SLOT(ValueChanged()));
    connect(ui->ssbl,SIGNAL(sliderReleased()), this, SLOT(ValueChanged()));
    connect(ui->ssbr,SIGNAL(sliderReleased()), this, SLOT(ValueChanged()));
    connect(ui->ssw,SIGNAL(sliderReleased()), this, SLOT(ValueChanged()));
    connect(ui->sfhl,SIGNAL(sliderReleased()), this, SLOT(ValueChanged()));
    connect(ui->sfhr,SIGNAL(sliderReleased()), this, SLOT(ValueChanged()));
    connect(ui->sfwl,SIGNAL(sliderReleased()), this, SLOT(ValueChanged()));
    connect(ui->sfwr,SIGNAL(sliderReleased()), this, SLOT(ValueChanged()));

    connect(ui->mc1,SIGNAL(clicked()),this,SLOT(checkbox()));
    connect(ui->mc2,SIGNAL(clicked()),this,SLOT(checkbox()));
    connect(ui->mc3,SIGNAL(clicked()),this,SLOT(checkbox()));
    connect(ui->mc4,SIGNAL(clicked()),this,SLOT(checkbox()));
    connect(ui->mc5,SIGNAL(clicked()),this,SLOT(checkbox()));
    connect(ui->mc6,SIGNAL(clicked()),this,SLOT(checkbox()));



    // save sliders in a list
    m_Sliders.append(ui->sfl);
    m_Sliders.append(ui->sfr);
    m_Sliders.append(ui->sc);
    m_Sliders.append(ui->ssl);
    m_Sliders.append(ui->ssr);
    m_Sliders.append(ui->ssbl);
    m_Sliders.append(ui->ssbr);
    m_Sliders.append(ui->ssw);
    m_Sliders.append(ui->sfhl);
    m_Sliders.append(ui->sfhr);
    m_Sliders.append(ui->sfwl);
    m_Sliders.append(ui->sfwr);


    // save dB value labels in a list
    m_Labels.append(ui->lfl);
    m_Labels.append(ui->lfr);
    m_Labels.append(ui->lc);
    m_Labels.append(ui->lsl);
    m_Labels.append(ui->lsr);
    m_Labels.append(ui->lsbl);
    m_Labels.append(ui->lsbr);
    m_Labels.append(ui->lsw);
    m_Labels.append(ui->lfhl);
    m_Labels.append(ui->lfhr);
    m_Labels.append(ui->lfwl);
    m_Labels.append(ui->lfwr);

    m_buttons.append(ui->mc1);
    m_buttons.append(ui->mc2);
    m_buttons.append(ui->mc3);
    m_buttons.append(ui->mc4);
    m_buttons.append(ui->mc5);
    m_buttons.append(ui->mc6);

    x922=m_Settings.value("TunerCompatibilityMode").toBool();
    if (x922)   //Im Kompatmodus nur verfügbare Typen anzeigen
        lsanz=5;

    for(int i=0;i<lsanz;i++) //LS-Konfig belegen
        ui->LSsystem->addItem(LStyp[i]);

    for (int i=0;i<12;i++)
        mchannels[i]=50;

    QStringList mstr; //LS-Paare vorbelegen
    mstr << "Front" << "Center" << "FrontHigh" << "FrontWide" << "Surround" << "Surr. Back" << "Subwoofer";
    ui->speaker->addItems(mstr);

    QStringList mstr1;
    mstr1 << "Memory 1"  << "Memory 2" << "Memory 3" << "Memory 4" << "Memory 5" << "Memory 6";
    ui->selectmem->addItems(mstr1);
}



LoudspeakerSettingsDialog::~LoudspeakerSettingsDialog()
{
    delete ui;
}


void LoudspeakerSettingsDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("LSSettingsWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}


void LoudspeakerSettingsDialog::MCACC(int mcacc)
{
    //      qDebug() <<"MC wert: " << wert;
          clear_toggles();
          m_buttons[mcacc - 1]->setChecked(true);
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
        qDebug() << "wertepaar" << sysValue <<"array" <<mLSpaar[sysValue];
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
      setslider();
   }
//  qDebug() << "speakerdaten:" <<data  <<sysValue <<wert;
   if (data.startsWith("SPK"))
     {
       wert=data.mid(3,1).toInt();
       ui->spa->setChecked(false);
       ui->spb->setChecked(false);
       ui->spab->setChecked(false);
       ui->spoff->setChecked(false);
       if (wert==1)
           ui->spa->setChecked(true);
       else if (wert==2)
          ui->spb->setChecked(true);
       else if (wert==3)
          ui->spab->setChecked(true);
       else if (wert==0)
          ui->spoff->setChecked(true);
     }
}



/*void LoudspeakerSettingsDialog::SpeakerReceived(QString data)
{
//    qDebug() << "Aus speakerreceived: " <<data;
}*/


void LoudspeakerSettingsDialog::ShowLoudspeakerSettingsDialog()
{
    QString str;
    if (!isVisible())
    {
        if (!m_PositionSet || !m_Settings.value("SaveLSSettingsWindowGeometry", false).toBool())
        {
            QWidget* Parent = dynamic_cast<QWidget*>(parent());
            int x = Parent->pos().x() + Parent->width() + 20;
            QPoint pos;
            pos.setX(x);
            pos.setY(Parent->pos().y());
            this->move(pos);
        }
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
    SendCmd("?SPK");
    SendCmd("?MC");
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
}


void LoudspeakerSettingsDialog::error(int)
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

        spmode << "Yes" << "Plus" << "No" <<"" <<"";
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


void LoudspeakerSettingsDialog::on_savebutt_clicked()
{   //Channel-Level aus public-Speicher in Memory x sichern, gem. Auswahl Combobox,
    QString str;
    int str1;
    str1=m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever

    for (int i=0;i<12;i++)
    {
        str=QString("mem%1-%2/%3").arg(ui->selectmem->currentIndex()).arg(str1).arg(channels[i]);
          m_Settings.setValue(str,mchannels[i]);
    }

    str=QString("mem%1-%2/LSCONFIG").arg(ui->selectmem->currentIndex()).arg(str1);
    m_Settings.setValue(str,mVal);
    for (int i=0;i<7;i++)
    {
          str=QString("mem%1-%2/%3").arg(ui->selectmem->currentIndex()).arg(str1).arg(LSpaar[i]);
          m_Settings.setValue(str,mLSpaar[i]);
    }
    str=QString("mem%1-%2/LSset").arg(ui->selectmem->currentIndex()).arg(str1);
     m_Settings.setValue(str,ui->meminf->text());

/*     for (int i=0;i<6;i++) //mcacc nicht speichern, da LS-Settings dort auch drin sind
     {
         if (m_buttons[i]->isChecked())
         {
             str=QString("mem%1-%2/mcacc").arg(ui->selectmem->currentIndex()).arg(str1);
              m_Settings.setValue(str,i);
              break;
         }
     }*/
}


void LoudspeakerSettingsDialog::on_restbutt_clicked()
{   //Channel-Level aus Speicher 1-5 zurücklesen und public speichern
    int j;
    QString str;
    int str1;
    str1=m_Settings.value("IP/4").toInt();//letztes Oktett IP anhängen, falls mehrere Reciever

/*    ui->speaker->setCurrentIndex(0);  //Speicherung entfernt, da LS-Settings auch in mcacc gespeichert werden--Konflikte
    ui->speakermode->setCurrentIndex(mLSpaar[0]);
    str=QString("mem%1-%2/mcacc").arg(ui->selectmem->currentIndex()).arg(str1);
    str1=m_Settings.value(str).toInt();   //Nummer des Buttons aus der m_button Liste (0-5)
    if (!m_buttons[str1]->isChecked())
        m_buttons[str1]->click();
*/
//    str1=m_Settings.value("IP/4").toInt();//letztes Oktett IP anhängen, falls mehrere Reciever

    for (int i=0;i<12;i++)
    {
          str=QString("mem%1-%2/%3").arg(ui->selectmem->currentIndex()).arg(str1).arg(channels[i]);
          j=m_Settings.value(str).toInt();
          if (j!=0)
          {
              mchannels[i]=j;
              str= (QString("%1%2CLV").arg(channels[i]).arg(j));
              //qDebug() <<str;
              SendCmd(str);
          }
          setslider();
    }
    str=QString("mem%1-%2/LSCONFIG").arg(ui->selectmem->currentIndex()).arg(str1);
    mVal=m_Settings.value(str).toInt();
    str = QString("%1SSF").arg(mVal);
    if (str.size()<5)
        str="0"+str;
    SendCmd(str);

    for (int i=0;i<7;i++)
    {
          str=QString("mem%1-%2/%3").arg(ui->selectmem->currentIndex()).arg(str1).arg(LSpaar[i]);
          mLSpaar[i]=m_Settings.value(str).toInt();
          str=LSpaar[i];
          str=str+QString("%1SSG").arg(mLSpaar[i]);
//          qDebug() << str;
          SendCmd(str);
    }

 }



void LoudspeakerSettingsDialog::ValueChanged()
{
    int i,j;
    QString str;
    QObject* sender =QObject::sender();
    str="ui->"+sender->objectName();
    for (j=0;j<m_Sliders.count();j++)
    {
        if (str==slchannels[j])
        {
            i=m_Sliders[j]->value();
            str= (QString("%1%2CLV").arg(channels[j]).arg(i));
            //qDebug() <<str;
            SendCmd(str);
        }
    }
    setslider();
}


void LoudspeakerSettingsDialog::setslider()
{
    QString str;
    double j;
    for (int i = 0; i < m_Sliders.count(); i++)
    {
       if (mchannels[i]>25 && mchannels[i]<75)
        {
        m_Sliders[i]->setValue(mchannels[i]);
        m_Sliders[i]->setSliderPosition(mchannels[i]);
        }
        else
        {
            mchannels[i]=50;
        }
        j= (double) mchannels[i];
        j=j/2-25;
        str=QString("%1").arg(j,0,'f',1);
//        qDebug() <<str;
        m_Labels[i]->setText(str);
    }
}

void LoudspeakerSettingsDialog::on_selectmem_currentIndexChanged(int index)
{
    QString str;
    int str1;
    str1=m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever
    str=QString("mem%1-%2/LSset").arg(index).arg(str1);
    str=m_Settings.value(str).toString();
    ui->meminf->setText(str);
}



void LoudspeakerSettingsDialog::checkbox()
{
    int i;
    QString str;
    QObject* sender =QObject::sender();
    str="ui->"+sender->objectName();
    i=str.mid(6,1).toInt()-1;
//      qDebug()  << "buttonchecked="  <<str <<"I=" <<i << m_buttons[i]->isChecked();
    if (m_buttons[i]->isChecked())
        {
            clear_toggles();
            str=QString("%1MC").arg(i+1);
            SendCmd(str);
            ShowLoudspeakerSettingsDialog();
        }
        m_buttons[i]->setChecked(true);
}


void LoudspeakerSettingsDialog::clear_toggles()
{
    for (int i=0;i<6;i++)
    m_buttons[i]->setChecked(false);
}

void LoudspeakerSettingsDialog::on_spa_clicked()
{
        SendCmd("1SPK");
}

void LoudspeakerSettingsDialog::on_spb_clicked()
{
        SendCmd("2SPK");
}

void LoudspeakerSettingsDialog::on_spab_clicked()
{
        SendCmd("3SPK");
}

void LoudspeakerSettingsDialog::on_spoff_clicked()
{
        SendCmd("0SPK");
}
