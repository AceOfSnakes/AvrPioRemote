#include "eqdialog.h"
#include "ui_eqdialog.h"
//#include <QFile>
//#include <QDomDocument>
//#include <QDir>
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

    // configure the timer
    connect((&m_Timer), SIGNAL(timeout()), this, SLOT(Timeout()));
    m_Timer.setSingleShot(true);
    m_Timer.setInterval(200);

  //  QString path = QDir::currentPath() + "/" + "ATBEQPresets.xml";
  //  ReadFile(path);

    QStringList mstr1;
    mstr1 << "Memory 1"  << "Memory 2" << "Memory 3" << "Memory 4" << "Memory 5";
    ui->selectmem->addItems(mstr1);

    SelectPreset(0);
}

EQDialog::~EQDialog()
{
    delete ui;
}


/*
void EQDialog::ResetEQPresets()
{
    for (int i = 0; i < 4; i++)
    {
        m_EQPresets[i].m_Name = QString("USER %1").arg(i + 1);
        for (int j = 0; j < 9; j++)
        {
            m_EQPresets[i].m_Values[j] = 50;
        }
    }
    m_EQPresets[0].m_Button = ui->pushButton_2;
    m_EQPresets[1].m_Button = ui->pushButton_3;
    m_EQPresets[2].m_Button = ui->pushButton_4;
    m_EQPresets[3].m_Button = ui->pushButton_5;

}

*/

void EQDialog::SelectPreset(int preset)
{
    m_SelectedPreset = preset;
    // select the pressed preset button
    for (int i = 0; i < 4; i++)
    {
//        m_EQPresets[i].m_Button->setChecked(i == preset);
    }
    if (preset >= 0 && preset <= 4)
    {
        // set the sliders
        for (int i = 0; i < 9; i++)
        {
            m_Sliders[i]->setValue(m_EQPresets[preset].m_Values[i]);
        }
        // deselect the FLAT button
//        ui->pushButton->setChecked(false);
    }
    else // FLAT
    {
        // set sliders to flat
        for (int i = 0; i < 9; i++)
        {
            m_Sliders[i]->setValue(50);
        }
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
}


void EQDialog::DataReceived(QString data)
{
    double eqValue = 0;
    QString str;
    if (data.startsWith("ATB"))
    {
        for (int i = 0; i < m_Sliders.count(); i++)
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
}


void EQDialog::Timeout()
{
    // send the eq settings to the receiver
    QString cmd = "00";
    for (int i = 0; i < m_Sliders.count(); i++)
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

/*
void EQDialog::on_RestoreEq_clicked()
{
    for (int i = 0; i < m_Sliders.count(); i++)
    {
        if (m_SelectedPreset >= 0 && m_SelectedPreset <= 4)
        {
            m_Sliders[i]->setSliderPosition(m_EQPresets[m_SelectedPreset].m_Values[i]);
        }
        else // flat
        {
            m_Sliders[i]->setSliderPosition(50);
        }
    }
    m_Timer.start();
}
*/

/*
void EQDialog::on_CloseEq_clicked()
{
    close();
}
*/



bool EQDialog::ReadFile(const QString& fileName)
{
    return true;
}

/*
    ResetEQPresets();
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QString msg = QString("Error: Cannot read file <%1>: %2").arg(fileName).arg(file.errorString());
        Logger::Log(msg);
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine,
                        &errorColumn)) {
        QString msg = QString("Error: Parse error at line %1, %2: %3").arg(errorLine).arg(errorColumn).arg(errorStr);
        Logger::Log(msg);
        file.close();
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if (root.tagName() != "ATBEQ") {
        QString msg = QString("Error: Not the favorites list");
        Logger::Log(msg);
        return false;
    }

    QDomNodeList nodes = root.childNodes();
    int count = 0;
    for (int i = 0; i < nodes.count(); i++)
    {
        if (nodes.at(i).isElement() && nodes.at(i).nodeName() == "Preset")
        {
            m_EQPresets[i].m_Name = nodes.at(i).attributes().namedItem("Name").nodeValue();
            m_EQPresets[i].m_Button->setText(m_EQPresets[i].m_Name);
            m_EQPresets[i].m_Values[0] = nodes.at(i).attributes().namedItem("Eq63").nodeValue().toInt();
            m_EQPresets[i].m_Values[1] = nodes.at(i).attributes().namedItem("Eq125").nodeValue().toInt();
            m_EQPresets[i].m_Values[2] = nodes.at(i).attributes().namedItem("Eq250").nodeValue().toInt();
            m_EQPresets[i].m_Values[3] = nodes.at(i).attributes().namedItem("Eq500").nodeValue().toInt();
            m_EQPresets[i].m_Values[4] = nodes.at(i).attributes().namedItem("Eq1k").nodeValue().toInt();
            m_EQPresets[i].m_Values[5] = nodes.at(i).attributes().namedItem("Eq2k").nodeValue().toInt();
            m_EQPresets[i].m_Values[6] = nodes.at(i).attributes().namedItem("Eq4k").nodeValue().toInt();
            m_EQPresets[i].m_Values[7] = nodes.at(i).attributes().namedItem("Eq8k").nodeValue().toInt();
            m_EQPresets[i].m_Values[8] = nodes.at(i).attributes().namedItem("Eq16k").nodeValue().toInt();
            count++;
        }
        if (count >= 4)
            break;
    }

    return true;
}


bool EQDialog::SaveFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QString msg = QString("Error: Cannot write file <%1>: %2").arg(fileName).arg(file.errorString());
        Logger::Log(msg);
        return false;
    }

    const int Indent = 4;
    QDomDocument doc;
    QDomElement Root = doc.createElement("ATBEQ");
    for (int i = 0; i < 4; i++)
    {
        QDomElement Preset = doc.createElement("Preset");
        Preset.setAttribute("Name",  m_EQPresets[i].m_Name);
        Preset.setAttribute("Eq63",  m_EQPresets[i].m_Values[0]);
        Preset.setAttribute("Eq125", m_EQPresets[i].m_Values[1]);
        Preset.setAttribute("Eq250", m_EQPresets[i].m_Values[2]);
        Preset.setAttribute("Eq500", m_EQPresets[i].m_Values[3]);
        Preset.setAttribute("Eq1k",  m_EQPresets[i].m_Values[4]);
        Preset.setAttribute("Eq2k",  m_EQPresets[i].m_Values[5]);
        Preset.setAttribute("Eq4k",  m_EQPresets[i].m_Values[6]);
        Preset.setAttribute("Eq8k",  m_EQPresets[i].m_Values[7]);
        Preset.setAttribute("Eq16k", m_EQPresets[i].m_Values[8]);
        Root.appendChild(Preset);
    }

    doc.appendChild(Root);

    QTextStream out(&file);
    doc.save(out, Indent);
    file.close();
    return true;
}

*/
/*
void EQDialog::on_SaveEq_clicked()
{
    if (m_SelectedPreset >= 0 && m_SelectedPreset <= 4) // not flat
    {
        for (int i = 0; i < 9; i++)
        {
            m_EQPresets[m_SelectedPreset].m_Values[i] = m_Sliders[i]->value();
        }
    }

    QString path = QDir::currentPath() + "/" + "ATBEQPresets.xml";
    SaveFile(path);
}
*/

void EQDialog::on_pushButton_clicked() // FLAT
{
    SelectPreset(-1);
}

/*
void EQDialog::on_pushButton_2_clicked() // USER 1
{
    SelectPreset(0);
}


void EQDialog::on_pushButton_3_clicked() // USER 2
{
    SelectPreset(1);
}


void EQDialog::on_pushButton_4_clicked() // USER 3
{
    SelectPreset(2);
}


void EQDialog::on_pushButton_5_clicked() // USER 4
{
    SelectPreset(3);
}
*/

void EQDialog::on_savebutt_clicked()
{
      //Channel-Level aus public-Speicher in Memory x sichern, gem. Auswahl Combobox,
        QString str;
        int str1;
        str1=m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever

        for (int i=0;i<9;i++)
        {
            str=QString("mem%1-%2/%3").arg(ui->selectmem->currentIndex()).arg(str1).arg(eqnames[i]);

           m_Settings.setValue(str,m_EQPresets[0].m_Values[i]);
            qDebug() <<str <<m_EQPresets[0].m_Values[i];
        }

}

void EQDialog::on_restbutt_clicked()
{
    //Channel-Level aus public-Speicher in Memory x sichern, gem. Auswahl Combobox,
      QString str;
      int str1;
      str1=m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever

      for (int i=0;i<9;i++)
      {
          str=QString("mem%1-%2/%3").arg(ui->selectmem->currentIndex()).arg(str1).arg(eqnames[i]);

          m_EQPresets[0].m_Values[i]=m_Settings.value(str).toInt();
          qDebug() <<str <<m_EQPresets[0].m_Values[i];

      }
      SelectPreset(0);
}
