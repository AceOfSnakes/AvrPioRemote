#include "tunerdialog.h"
#include "ui_tunerdialog.h"
#include <QDebug>
#include "actionwithparameter.h"
#include <QMenu>

TunerDialog::TunerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TunerDialog)
{
    ui->setupUi(this);

    m_SelectedClassNo = "A";
    m_SelectedPresetNo = "00";
    m_TunerFrequency = 0;

    connect(parent, SIGNAL(DataReceived(QString)), this, SLOT(DataReceived(QString)));
    connect(this, SIGNAL(SendCmd(QString)), parent, SLOT(SendCmd(QString)));

    m_ClassButtons.push_back(ui->ClassAButton);
    m_ClassButtons.push_back(ui->ClassBButton);
    m_ClassButtons.push_back(ui->ClassCButton);
    m_ClassButtons.push_back(ui->ClassDButton);
    m_ClassButtons.push_back(ui->ClassEButton);
    m_ClassButtons.push_back(ui->ClassFButton);
    m_ClassButtons.push_back(ui->ClassGButton);

    for (int i = 0; i < (int)m_ClassButtons.size(); i++)
    {
        QString Param = "";
        Param += (QChar)(i + 'A');
        ActionWithParameter* action = new ActionWithParameter(m_ClassButtons[i], Param);
        connect(m_ClassButtons[i], SIGNAL(clicked()), action, SLOT(trigger()));
        connect(action, SIGNAL(ActionTriggered(QString)), this, SLOT(ClassButtonClicked(QString)));
        m_ClassButtons[i]->addAction(action);
    }

    m_PresetButtons.push_back(ui->Preset1Button);
    m_PresetButtons.push_back(ui->Preset2Button);
    m_PresetButtons.push_back(ui->Preset3Button);
    m_PresetButtons.push_back(ui->Preset4Button);
    m_PresetButtons.push_back(ui->Preset5Button);
    m_PresetButtons.push_back(ui->Preset6Button);
    m_PresetButtons.push_back(ui->Preset7Button);
    m_PresetButtons.push_back(ui->Preset8Button);
    m_PresetButtons.push_back(ui->Preset9Button);

    for (int i = 0; i < (int)m_PresetButtons.size(); i++)
    {
        QString Param = QString("%1").arg(i + 1, 2, 10, (QChar)'0');
        ActionWithParameter* action = new ActionWithParameter(m_PresetButtons[i], Param);
        connect(m_PresetButtons[i], SIGNAL(clicked()), action, SLOT(trigger()));
        connect(action, SIGNAL(ActionTriggered(QString)), this, SLOT(PresetButtonClicked(QString)));
        m_PresetButtons[i]->addAction(action);
    }

    //QIcon icon;
    //icon.addFile ( "images/pen.png", QSize(16, 16));
    //ui->RenamePresetApplayButton->setIcon(icon);

    //ui->RenamePresetApplayButton->setStyleSheet(QString::fromUtf8("background-image: url(:/images/pen.png);"));
    //ui->RenamePresetApplayButton->setStyleSheet("background: transparent");

    this->setFixedSize(this->size());
}

TunerDialog::~TunerDialog()
{
    delete ui;
}

void TunerDialog::ShowTunerDialog()
{
    QWidget* Parent = dynamic_cast<QWidget*>(parent());
    int x = Parent->pos().x() + Parent->width() + 20;
    QPoint pos;
    pos.setX(x);
    pos.setY(Parent->pos().y());
    this->move(pos);
    this->show();
//    SendCmd("?FR");
    SendCmd("?PR");
}

void TunerDialog::ClassButtonClicked(QString Param)
{
    QString cmd = QString("%1%2PR").arg(Param).arg(m_SelectedPresetNo);
    emit SendCmd(cmd);
}

void TunerDialog::PresetButtonClicked(QString Param)
{
    QString cmd = QString("%1%2PR").arg(m_SelectedClassNo).arg(Param);
    emit SendCmd(cmd);
}

void TunerDialog::DataReceived(QString data)
{
    if (data.startsWith("FRA"))
    {
        m_TunerFrequency = data.mid(3).toInt();
        QString str = QString("%1 kHz").arg(data.mid(3).toInt());
        ui->FrequencyEdit->setText(str);
        ui->FMButton->setChecked(false);
        ui->AMButton->setChecked(true);
    }
    else if (data.startsWith("FRF"))
    {
        m_TunerFrequency = data.mid(3).toInt();
        QString str = QString("%1 MHz").arg(data.mid(3).toDouble() / 100.0);
        ui->FrequencyEdit->setText(str);
        ui->FMButton->setChecked(true);
        ui->AMButton->setChecked(false);
    }
    else if (data.startsWith("PR"))
    {
        int n = 0;
        m_SelectedClassNo = data[2];
        n = data[2].toAscii() - 'A';
        SelectClassButton(n);

        m_SelectedPresetNo = data.mid(3, 2);
        n = data.mid(3).toInt() - 1;
        SelectPresetButton(n);

        SendCmd("?FR");
        SendCmd("?TQ");
    }
    else if (data.startsWith("TQ"))
    {
        QString Class = QString("%1").arg(data[2]);
        QString Preset = "0" + data.mid(3, 1);
        QString Name = data.mid(4);
        Name = Name.trimmed();
        while (Name.startsWith("\""))
            Name.remove(0, 1);
        while (Name.endsWith("\""))
            Name.chop(1);
        Name = Name.trimmed();
        if (m_SelectedClassNo == Class && m_SelectedPresetNo == Preset)
        {
            ui->PresetEdit->setText(Name);
        }
    }
}

void TunerDialog::SelectClassButton(int idx)
{
    for(int i = 0; i < (int)m_ClassButtons.size(); i++)
    {
        if(i != idx)
            m_ClassButtons[i]->setChecked(false);
        else
            m_ClassButtons[i]->setChecked(true);
    }
}

void TunerDialog::SelectPresetButton(int idx)
{
    for(int i = 0; i < (int)m_PresetButtons.size(); i++)
    {
        if(i != idx)
            m_PresetButtons[i]->setChecked(false);
        else
            m_PresetButtons[i]->setChecked(true);
    }
}

void TunerDialog::on_PresetPlusButton_clicked()
{
    emit SendCmd("TPI");
}

void TunerDialog::on_PresetMinusButton_clicked()
{
    emit SendCmd("TPD");
}

void TunerDialog::on_AMButton_clicked()
{
    emit SendCmd("01TN");
}

void TunerDialog::on_FMButton_clicked()
{
    emit SendCmd("00TN");
}

void TunerDialog::on_FrequencyMinusButton_clicked()
{
    m_TunerFrequency -= 5;
    QString str = QString("%1").arg(m_TunerFrequency, 5, 10, (QChar)'0');
    emit SendCmd("TAC");
    emit SendCmd(QString("%1TP").arg(str[0]));
    emit SendCmd(QString("%1TP").arg(str[1]));
    emit SendCmd(QString("%1TP").arg(str[2]));
    emit SendCmd(QString("%1TP").arg(str[3]));
    emit SendCmd(QString("%1TP").arg(str[4]));
    emit SendCmd("03TN");
}

void TunerDialog::on_FrequencyPlusButton_clicked()
{
    m_TunerFrequency += 5;
    QString str = QString("%1").arg(m_TunerFrequency, 5, 10, (QChar)'0');
    emit SendCmd("TAC");
    emit SendCmd(QString("%1TP").arg(str[0]));
    emit SendCmd(QString("%1TP").arg(str[1]));
    emit SendCmd(QString("%1TP").arg(str[2]));
    emit SendCmd(QString("%1TP").arg(str[3]));
    emit SendCmd(QString("%1TP").arg(str[4]));
    emit SendCmd("03TN");
}

void TunerDialog::on_ChoosePresetButton_clicked()
{
    QAction* pAction;
    QMenu MyMenu(this);

    pAction = new QAction("Refresh status", this);
    MyMenu.addAction(pAction);
    connect(pAction, SIGNAL(triggered()), this, SLOT(RequestStatus()));


    QPoint pos = QCursor::pos();
    MyMenu.exec(pos);
}

void TunerDialog::on_RenamePresetApplayButton_clicked()
{
    if (ui->PresetEdit->isReadOnly())
    {
        m_TempPresetName = ui->PresetEdit->text();
        ui->PresetEdit->setReadOnly(false);
    }
    else
    {
        ui->PresetEdit->setReadOnly(true);
        QString Name = ui->PresetEdit->text().trimmed();
        Name = QString("%1").arg(Name, -8, QChar(' '));
        QString str = QString("%1%2\"%3\"TQ").arg(m_SelectedClassNo).arg(m_SelectedPresetNo[1]).arg(Name);
        //emit SendCmd("A1\"SWr2.-$&\"TQ");
        emit SendCmd(str);
    }
}

void TunerDialog::on_RenamePresetCancelButton_clicked()
{
    ui->RenamePresetApplayButton->setChecked(false);
    ui->PresetEdit->setReadOnly(true);
    ui->PresetEdit->setText(m_TempPresetName);
}
