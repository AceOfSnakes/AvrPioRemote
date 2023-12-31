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
#include "tunerdialog.h"
#include "ui_tunerdialog.h"
#include <QDebug>
#include <QMenu>


TunerDialog::TunerDialog(QWidget *parent, ReceiverInterface &Comm, QSettings &settings) :
    QDialog(parent),
    m_Settings(settings),
    ui(new Ui::TunerDialog),
    m_Comm(Comm),
    m_TunerInputSelected(false),
    m_PositionSet(false),
    m_TunerStringPosition(-1)
{
    ui->setupUi(this);

    // restore the position of the window
    if (m_Settings.value("SaveTunerWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("TunerWindowGeometry").toByteArray());
    }

    m_SelectedClassNo = "A";
    m_SelectedPresetNo = "00";
    m_TunerFrequency = 0;
    m_CompatibilityMode = false;

    connect(&m_Comm, SIGNAL(DataReceived(const QString&, bool)), this, SLOT(DataReceived(const QString&, bool)));
    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));

    MsgDistributor::AddResponseListener(this, QStringList() << InputFunctionResponse_FN().getResponseID() << DisplayDataResponse_FL().getResponseID());

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

    m_CompatibilityMode = m_Settings.value("TunerCompatibilityMode", false).toBool();
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


void TunerDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("TunerWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}


void TunerDialog::ManualShowTunerDialog()
{
    ShowTunerDialog(false);
}

void TunerDialog::ShowTunerDialog(bool autoShow)
{
    if (m_Comm.IsPioneer() &&((!autoShow) || (m_Settings.value("AutoShowTuner", true).toBool() && !isVisible())))
    {
        if (!m_PositionSet || !m_Settings.value("SaveTunerWindowGeometry", false).toBool())
        {
            QWidget* Parent = dynamic_cast<QWidget*>(parent());
            int x = Parent->pos().x() + Parent->width() + 20;
            QPoint pos;
            pos.setX(x);
            pos.setY(Parent->pos().y());
            this->move(pos);
        }
        this->show();
        EnableControls(true);
        ui->SaveButton->setEnabled(false);
        ui->OkButton->setEnabled(false);
        ui->FrequencyEdit->setReadOnly(true);
        ui->FrequencyEdit->setInputMask("");
        ui->RenamePresetButton->setEnabled(true);
        ui->CancelButton->setEnabled(false);
    }
//    SendCmd("?FR");
    if (m_Comm.IsPioneer())
    {
        SendCmd("?PR");
    }
}


void TunerDialog::ResponseReceived(ReceivedObjectBase *response)
{
    if (!m_Comm.IsPioneer())
    {
        return;
    }

    DisplayDataResponse_FL* display = dynamic_cast<DisplayDataResponse_FL*>(response);
    if (display != NULL)
    {
        if (m_TunerInputSelected && display->getDisplayType() == 2)
        {
            GetScrolledString(display->getDisplayLine());
            ui->DisplayEditBig->setText(m_DisplayString);
        }
        return;
    }
    InputFunctionResponse_FN* inputFunction = dynamic_cast<InputFunctionResponse_FN*>(response);
    if (inputFunction != NULL)
    {
        int no = inputFunction->getNumber();
        if (no == 2) // tuner
        {
            m_TunerInputSelected = true;
        }
        else
        {
            m_TunerInputSelected = true;
            ui->DisplayEditBig->clear();
        }
        return;
    }
}


void TunerDialog::ClassButtonClicked(QString Param)
{
    QString cmd = QString("%1%2PR").arg(Param).arg(m_SelectedPresetNo);
    emit SendCmd(cmd);
    m_ClassButtons[m_SelectedClassNo[0].toLatin1() - 'A']->setChecked(true);
}


void TunerDialog::PresetButtonClicked(QString Param)
{
    QString cmd = QString("%1%2PR").arg(m_SelectedClassNo).arg(Param);
    emit SendCmd(cmd);
    m_PresetButtons[m_SelectedPresetNo.toInt() - 1]->setChecked(true);
}


void TunerDialog::DataReceived(const QString& data, bool is_pioneer)
{
    if (m_Comm.IsPioneer())
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
            n = data[2].toLatin1() - 'A';
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
            // clear the preset actions list
            if (Class == "A" && Preset == "01")
            {
                for(int i = 0; i < m_PresetActions.count(); i++)
                {
                    delete m_PresetActions[i];
                }
                m_PresetActions.clear();
            }

            // if the name is set add the preset to the action list
            if (Name != "")
            {
                QString Param = QString("%1%2").arg(Class).arg(Preset);
                ActionWithParameter* action = new ActionWithParameter(ui->ChoosePresetButton, Name, Param);
                connect(action, SIGNAL(ActionTriggered(QString)), this, SLOT(PresetSelected(QString)));
                m_PresetActions.append(action);
            }
        }
    }
}


void TunerDialog::PresetSelected(QString Param)
{
    QString cmd = QString("%1PR").arg(Param);
    SendCmd(cmd);
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
//    QAction* pAction;
    QMenu MyMenu(ui->ChoosePresetButton);

    MyMenu.addActions(m_PresetActions);
//    pAction = new QAction("Refresh status", this);
//    MyMenu.addAction(pAction);
//    connect(pAction, SIGNAL(triggered()), this, SLOT(RequestStatus()));


    QPoint pos = QCursor::pos();
    MyMenu.exec(pos);
}


void TunerDialog::on_DisplayButton_clicked()
{
    emit SendCmd("06TN");
}


void TunerDialog::on_PTYSearchButton_clicked()
{
    emit SendCmd("07TN");
}


void TunerDialog::on_NoiseCutButton_clicked()
{
    emit SendCmd("05TN");
}


void TunerDialog::on_EditFrequencyButton_clicked()
{
    EnableControls(false);
    ui->CancelButton->setEnabled(true);
    ui->OkButton->setEnabled(true);
//    if (!m_CompatibilityMode)
    ui->SaveButton->setEnabled(true);
    if (ui->FMButton->isChecked())
    {
        // m_TunerFrequency is 12345 --> FM 123.45 MHz
        double f = (double)m_TunerFrequency / 100.0;
        QString text = QString("%1").arg(f, 3, 'f', 2, QChar('0'));
        ui->FrequencyEdit->setText(text);
        ui->FrequencyEdit->setInputMask("000.00;");
    }
    else
    {
        // m_TunerFrequency is 12345 --> AM 12345 kHz
        QString text = QString("%1").arg(m_TunerFrequency, 5, 10, QChar('0'));
        ui->FrequencyEdit->setText(text);
        ui->FrequencyEdit->setInputMask("00000;");
    }
    ui->FrequencyEdit->setReadOnly(false);
}


void TunerDialog::EnableControls(bool enable)
{
    for (int i = 0; i < (int)m_ClassButtons.size(); i++)
    {
        m_ClassButtons[i]->setEnabled(enable);
    }
    for (int i = 0; i < (int)m_PresetButtons.size(); i++)
    {
        m_PresetButtons[i]->setEnabled(enable);
    }
    ui->FMButton->setEnabled(enable);
    ui->AMButton->setEnabled(enable);
    ui->ChoosePresetButton->setEnabled(enable);
    ui->FrequencyMinusButton->setEnabled(enable);
    ui->FrequencyPlusButton->setEnabled(enable);
    ui->SaveButton->setEnabled(enable);
    ui->RenamePresetButton->setEnabled(enable);
    ui->CancelButton->setEnabled(enable);
    ui->EditFrequencyButton->setEnabled(enable);
    ui->PresetPlusButton->setEnabled(enable);
    ui->PresetMinusButton->setEnabled(enable);
    ui->DisplayButton->setEnabled(enable);
    ui->PTYSearchButton->setEnabled(enable);
    ui->NoiseCutButton->setEnabled(enable);
    ui->OkButton->setEnabled(enable);
}


void TunerDialog::on_SaveButton_clicked()
{
    if (!ui->FrequencyEdit->isReadOnly())
    {
        QString cmd;
//        emit SendCmd("02TN"); // edit
//        emit SendCmd("TC"); // edit
//        cmd = QString ("FG%1").arg((int)(m_SelectedPresetNo[1].toAscii()), 1, 16);
//        emit SendCmd(cmd); // enter
//        emit SendCmd("03TN"); // enter
        if (m_CompatibilityMode)
        {
            emit SendCmd("02TN"); // edit
            for(int i = 'A'; i < m_SelectedClassNo[0].toLatin1(); i++)
            {
                emit SendCmd("TC"); // class
            }
            for(int i = 0; i < m_SelectedPresetNo.toInt(); i++)
            {
                emit SendCmd("TPI"); // increment preset
            }
            emit SendCmd("03TN"); // enter
        }
        else
        {
            QString band = "A";
            QString mpx = "0";
            if (ui->FMButton->isChecked())
            {
                band = "F";
                mpx = "1";
            }
            cmd = QString("%1%2%3%4%5TGA").arg(m_SelectedClassNo).arg(m_SelectedPresetNo[1]).arg(band).arg(m_TunerFrequency, 5, 10, QChar('0')).arg(mpx);
            emit SendCmd(cmd);
        }
        emit SendCmd("?FR");
    }
    else if (!ui->PresetEdit->isReadOnly())
    {
        ui->PresetEdit->setReadOnly(true);
        ui->CancelButton->setEnabled(false);
        QString Name = ui->PresetEdit->text().trimmed();
        if (!m_CompatibilityMode)
        {
            Name = QString("%1").arg(Name, -8, QChar(' '));
            QString str = QString("%1%2\"%3\"TQ").arg(m_SelectedClassNo).arg(m_SelectedPresetNo[1]).arg(Name);
            //emit SendCmd("A1\"SWr2.-$&\"TQ");
            emit SendCmd(str);
        }
        else
        {
            emit SendCmd("02TN"); // edit
            for (int i = 0; i < Name.length(); i++)
            {
                QString cmd = QString("FG%1").arg((int)Name[i].toLatin1(), 2, 16, QChar('0'));
                emit SendCmd(cmd);
            }
            emit SendCmd("03TN"); // enter
        }
    }
    ui->FrequencyEdit->setReadOnly(true);
    ui->FrequencyEdit->setInputMask("");
    EnableControls(true);
    ui->CancelButton->setEnabled(false);
    ui->OkButton->setEnabled(false);
    ui->SaveButton->setEnabled(false);
}


void TunerDialog::on_CancelButton_clicked()
{
    // frequency edit
    if (!ui->FrequencyEdit->isReadOnly())
    {
        ui->FrequencyEdit->setReadOnly(true);
        ui->FrequencyEdit->setInputMask("");
        ui->FrequencyEdit->setText("");
        emit SendCmd("?FR");
    }
    // preset name edit
    else if (!ui->PresetEdit->isReadOnly())
    {
        ui->PresetEdit->setReadOnly(true);
        emit SendCmd("?PR");
    }
    EnableControls(true);
    ui->CancelButton->setEnabled(false);
    ui->OkButton->setEnabled(false);
    ui->SaveButton->setEnabled(false);
}


void TunerDialog::on_OkButton_clicked()
{
    if (!ui->FrequencyEdit->isReadOnly())
    {
        ui->FrequencyEdit->setReadOnly(true);
        QString text = ui->FrequencyEdit->text();
        //qDebug() << " FrequencyEdit->text() = <" << ui->FrequencyEdit->text() << ">";
//        QString msg = QString("");
//        Logger::Log(msg);
        if (ui->FMButton->isChecked())
        {
            double f = text.toDouble() * 100.0;
            m_TunerFrequency = f;
            //qDebug() << "f = " << f;
        }
        else
        {
            m_TunerFrequency = text.toDouble();
        }
        ui->FrequencyEdit->setInputMask("");
        QString str = QString("%1").arg(m_TunerFrequency, 5, 10, (QChar)'0');
        //qDebug() << "FREQ " << m_TunerFrequency << " <" << str << ">";
        emit SendCmd("TAC");
        emit SendCmd(QString("%1TP").arg(str[0]));
        emit SendCmd(QString("%1TP").arg(str[1]));
        emit SendCmd(QString("%1TP").arg(str[2]));
        emit SendCmd(QString("%1TP").arg(str[3]));
        emit SendCmd(QString("%1TP").arg(str[4]));
        emit SendCmd("03TN");
//        emit SendCmd("?FR");
    }
    EnableControls(true);
    ui->CancelButton->setEnabled(false);
    ui->OkButton->setEnabled(false);
    ui->SaveButton->setEnabled(false);
}

void TunerDialog::on_RenamePresetButton_clicked()
{
    EnableControls(false);
    ui->CancelButton->setEnabled(true);
    ui->SaveButton->setEnabled(true);
    m_TempPresetName = ui->PresetEdit->text();
    ui->PresetEdit->setReadOnly(false);
}


// a little bit of magic to compose a list of scrolling short strings into
// a long one
bool TunerDialog::GetScrolledString(QString input)
{
//    input = input.mid(1); // ignore the first character, it's a symbol
    // if the first seen string equals to the current one, the string is wrapped
    // and we are done
//    if (m_Firstline == input)
//    {
//        // there is unneded data at the end of the string
//        m_Remembered = m_Remembered.mid(0, m_Remembered.count() - m_Firstline.count() + 1);
//        m_Done = true;
//        return true;
//    }
    // remove the end of the string, so the indexOf is working
    QString tmp = input.mid(0, input.count() - 2);
    // find the current string in the saved
    int pos = m_DisplayString.lastIndexOf(tmp);
//    qDebug() << "|" << input << "| |" << tmp << "| " << pos;
    if ((pos == -1) && (m_TunerStringPosition != (pos + 1)))
    {
        // nothing is found, so take the current string as the beginning
        // of our new string
        m_DisplayString = input;
        m_TunerStringPosition = -1;
        //m_Firstline = input;
    }
    else
    {
        // integrate the current string in to our saved string
        // it get only one character longer
        m_DisplayString.replace(pos, input.length(), input);
        m_TunerStringPosition = pos;
    }
    return false;
}

