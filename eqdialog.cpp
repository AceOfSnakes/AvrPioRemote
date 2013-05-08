#include "eqdialog.h"
#include "ui_eqdialog.h"
#include <QDebug>

const char* eqnames[] = {
    "Eq63",
    "Eq125",
    "Eq250",
    "Eq500",
    "Eq1k",
    "Eq2k",
    "Eq4k",
    "Eq8k",
    "Eq16k",
    "Eqbass",
    "Eqtreble",
};


EQDialog::EQDialog(QWidget *parent, ReceiverInterface &Comm,QSettings &settings) :
    QDialog(parent),
    ui(new Ui::EQDialog),
    m_Comm(Comm),
    m_Settings(settings),
    m_Timer(this),
    m_SelectedPreset(-1) // flat
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    // communication
    connect(&m_Comm, SIGNAL(DataReceived(QString)), this, SLOT(DataReceived(QString)));
    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));

    // slider
    connect(ui->eq63,  SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq125, SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq250, SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq500, SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq1k,  SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq2k,  SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq4k,  SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq8k,  SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    connect(ui->eq16k, SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));


    // save sliders in a list
    m_Sliders.append(ui->eq63);
    m_Sliders.append(ui->eq125);
    m_Sliders.append(ui->eq250);
    m_Sliders.append(ui->eq500);
    m_Sliders.append(ui->eq1k);
    m_Sliders.append(ui->eq2k);
    m_Sliders.append(ui->eq4k);
    m_Sliders.append(ui->eq8k);    
    m_Sliders.append(ui->eq16k);
    m_Sliders.append(ui->eqba);
    m_Sliders.append(ui->eqtr);


    // save dB value labels in a list
    m_Labels.append(ui->wert63);
    m_Labels.append(ui->wert125);
    m_Labels.append(ui->wert250);
    m_Labels.append(ui->wert500);
    m_Labels.append(ui->wert1k);
    m_Labels.append(ui->wert2k);
    m_Labels.append(ui->wert4k);
    m_Labels.append(ui->wert8k);
    m_Labels.append(ui->wert16k);
    m_Labels.append(ui->wertbass);
    m_Labels.append(ui->werttreble);

    // configure the timer
    connect((&m_Timer), SIGNAL(timeout()), this, SLOT(Timeout()));
    m_Timer.setSingleShot(true);
    m_Timer.setInterval(200);

  //  QString path = QDir::currentPath() + "/" + "ATBEQPresets.xml";
  //  ReadFile(path);

    QStringList mstr1;
    mstr1 << "Memory 1"  << "Memory 2" << "Memory 3" << "Memory 4" << "Memory 5";
    ui->selectmem->addItems(mstr1);

//    SelectPreset(-1);
}


EQDialog::~EQDialog()
{
    delete ui;
}


void EQDialog::SelectPreset(int preset)
{
    m_SelectedPreset = preset;
    if (preset >= 0 && preset <= 4)
    {
        // set the sliders
        for (int i = 0; i < 11; i++)
        {
            m_Sliders[i]->setValue(m_EQPresets[preset].m_Values[i]);
            if (i>8) //bass+treble
                 m_Sliders[i]->setValue(m_EQPresets[preset].m_Values[i]*-1);
        }
    }
    else // FLAT
    {
        // set sliders to flat
        for (int i = 0; i < 9; i++)
        {
            m_Sliders[i]->setValue(50);
        }
             m_Sliders[9]->setValue(-6); //Bass
             m_Sliders[10]->setValue(-6); //Treble
        // select the FLAT button
        ui->pushButton->setChecked(false);
    }
    m_Timer.start();
}



void EQDialog::ShowEQDialog()
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
    SendCmd("?ATB");
    SendCmd("?BA");
    SendCmd("?TR");
    SendCmd("?TO");
}


void EQDialog::DataReceived(QString data)
{
   int wert;
    double eqValue = 0;
    QString str;
    if (data.startsWith("ATB"))
    {
        for (int i = 0; i < 9; i++)
        {
            eqValue = data.mid(5 + i * 2, 2).toInt();
            m_Sliders[i]->setSliderPosition(eqValue);
            m_EQPresets[0].m_Values[i]=eqValue;

            eqValue = (eqValue - 50.0) / 2.0;

            if (eqValue == 0.0)//eqValue > -0.1 && eqValue <= 0.1)
            {
                str = "0.0";
            }
            else if (eqValue < 0.0)
                str = QString("%1").arg(eqValue, 3, 'f', 1);
            else
                str = QString("+%1").arg(eqValue, 3, 'f', 1);
            m_Labels[i]->setText(str);
        }
    }
    if (data.startsWith("BA") || data.startsWith("TR"))
    {
          str=data.mid(2,2);
          wert=str.toInt();
          if (data.startsWith("BA"))
        {
            m_EQPresets[0].m_Values[9]=wert;
            m_Sliders[9]->setSliderPosition(wert*-1);
            wert=6-wert;
            str=QString("%1dB").arg(wert);
            m_Labels[9]->setText(str);
        }
        else
        {
            m_EQPresets[0].m_Values[10]=wert;
            m_Sliders[10]->setSliderPosition(wert*-1);
            wert=6-wert;
            str=QString("%1dB").arg(wert);
            m_Labels[10]->setText(str);
        }
    }
    if (data.startsWith("TO"))
    {
        wert=data.mid(2,1).toInt();
        if (wert==0)
            ui->bypass->setText("Bypass");
        else
            ui->bypass->setText("Tone");
    }
}



void EQDialog::Timeout()
{
    // send the eq settings to the receiver, zuerst Mainbass+Treble
    QString str;
    str=QString("%1BA").arg(m_Sliders[9]->value()*-1);
    if (str.size() <4)
            str="0"+str;
//     qDebug() <<"bass" <<str;
     SendCmd(str);
     str=QString("%1TR").arg(m_Sliders[10]->value()*-1);
     if (str.size() <4)
             str="0"+str;
//      qDebug() <<"treble" <<str;
      SendCmd(str);


    QString cmd = "00";
    for (int i = 0; i < 9; i++)
    {
        QString str = QString("%1").arg(m_Sliders[i]->value(), 2, 10, QChar('0'));
        cmd.append(str);
    }
    cmd.append("50ATB");
    emit SendCmd(cmd);


}


void EQDialog::OnSliderValueChanged(int value)
{
    // don't change the eq settings yet, because the receiver is somewhat slow
    // and get irritated if the next command is sent to early
    // so wait till the value is not changing anymore
    m_Timer.start();
}



void EQDialog::on_pushButton_clicked() // FLAT
{
    SelectPreset(-1);
}


void EQDialog::on_savebutt_clicked()
{
      //Channel-Level aus public-Speicher in Memory x sichern, gem. Auswahl Combobox,
        QString str;
        int str1;
        str1=m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever

        for (int i=0;i<11;i++)
        {
            str=QString("mem%1-%2/%3").arg(ui->selectmem->currentIndex()).arg(str1).arg(eqnames[i]);

           m_Settings.setValue(str,m_EQPresets[0].m_Values[i]);
//            qDebug() <<str <<m_EQPresets[0].m_Values[i];
        }
       str=QString("mem%1-%2/EQset").arg(ui->selectmem->currentIndex()).arg(str1);
        m_Settings.setValue(str,ui->meminf->text());
}



void EQDialog::on_restbutt_clicked()
{
    //Channel-Level aus public-Speicher in Memory x sichern, gem. Auswahl Combobox,
      QString str;
      int str1;
      str1=m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever

      for (int i=0;i<11;i++)
      {
          str=QString("mem%1-%2/%3").arg(ui->selectmem->currentIndex()).arg(str1).arg(eqnames[i]);

          m_EQPresets[0].m_Values[i]=m_Settings.value(str).toInt();
//          qDebug() <<str <<m_EQPresets[0].m_Values[i];
      }
      SelectPreset(0);
}


void EQDialog::on_selectmem_currentIndexChanged(int index)
{
    QString str;
    int str1;
    str1=m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever
    str=QString("mem%1-%2/EQset").arg(index).arg(str1);
    str=m_Settings.value(str).toString();
    ui->meminf->setText(str);
}



void EQDialog::on_eqba_sliderReleased()
{
    QString str;
    int i=ui->eqba->value()*-1;
    str=QString("%1BA").arg(i);
    if (str.size() <4)
            str="0"+str;
     //qDebug() <<"bass" <<str;
     SendCmd(str);
}

void EQDialog::on_eqtr_sliderReleased()
{
    QString str;
    int i=ui->eqtr->value()*-1;
    str=QString("%1TR").arg(i);
    if (str.size() <4)
            str="0"+str;
     //qDebug() <<"treble" <<str;
    SendCmd(str);
}

void EQDialog::on_bypass_clicked()
{
    if (ui->bypass->text()=="Tone")
    {
        SendCmd("0TO");
        ui->bypass->setText("Bypass");
    }
    else
    {
        SendCmd("1TO");
        ui->bypass->setText("Tone");
        SelectPreset(0);
    }

}
