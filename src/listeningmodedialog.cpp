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
#include "listeningmodedialog.h"
#include "ui_listeningmodedialog.h"
#include "Defs.h"
#include <QDebug>
#include "receiver_interface/receiverinterface.h"


ListeningModeDialog::ListeningModeDialog(QWidget *parent,QSettings &settings,ReceiverInterface &Comm ) :
    QDialog(parent),
    m_Settings(settings),
    m_Comm(Comm),
    ui(new Ui::ListeningModeDialog),
    m_PositionSet(false)
{
    ui->setupUi(this);

    if(m_Settings.value("SaveLMSettingsWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("LMSettingsWindowGeometry").toByteArray());
    }

    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));
//    connect(&m_Comm,SIGNAL(ListeningModeData(QString)),this,SLOT(LMchanged(QString)));
    connect(&m_Comm,SIGNAL(Listenextended(QString, QString)),this,SLOT(LMchanged(QString, QString)));
    int i=0;
    while (strlen(LISTENING_MODE[i].key) != 0)
    {
        ui->listmodi->addItem(LISTENING_MODE[i].text);
        LMUserData* data = new LMUserData();
        data->m_Data = LISTENING_MODE[i].key;
        //////// ui->listmodi->setUserData(i, data);
        i++;
        if ((strcmp(LISTENING_MODE[i].key, "0050") == 0) && m_Settings.value("TunerCompatibilityMode").toBool())
        {
              //für 922 kompatible nur deren gültige werte anzeigen
            break;
        }
    }
}


ListeningModeDialog::~ListeningModeDialog()
{
    delete ui;
}


void ListeningModeDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("LMSettingsWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}


void ListeningModeDialog::ShowListeningDialog()
{
    if (!isVisible())
    {
        if (!m_PositionSet || !m_Settings.value("SaveLMSettingsWindowGeometry", false).toBool())
        {
            QWidget* Parent = dynamic_cast<QWidget*>(parent());
            int x = Parent->pos().x() - Parent->width()-100;
            QPoint pos;
            pos.setX(x);
            pos.setY(Parent->pos().y());
            this->move(pos);
        }
        this->show();
    }
    SendCmd("?L"); //aktueller Wert
    SendCmd("?S");
}


void ListeningModeDialog::on_listmodi_doubleClicked(const QModelIndex &/*index*/)
{
    QString str;
    int i;
    i=ui->listmodi->currentRow(); //ausgewähltes Element, passt zur Füllliste
    str=LISTENING_MODE[i].key;
    str=str+"SR";
    SendCmd(str);
//    str="?S";
//    qDebug() << "?L "
//    qDebug() << "row:" <<ui->listmodi->currentRow() <<"to send:" <<str ;
}


void ListeningModeDialog::LMchanged(QString id, QString data)
{
    // select the listening mode in the list
    for (int i = 0; i < ui->listmodi->count(); i++)
    {
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        if (id == ((LMUserData*)ui->listmodi->userData(i))->m_Data) // selected
        {
            ui->listmodi->item(i)->setBackgroundColor(QColor(0, 0, 255)); // blue background
            ui->listmodi->item(i)->setTextColor(QColor(255, 255, 255)); // white text
        }
        else // normla
        {
            ui->listmodi->item(i)->setBackgroundColor(QColor(255, 255, 255)); // white background
            ui->listmodi->item(i)->setTextColor(QColor(0, 0, 0)); // black text
        }
#else
        // if (id == ((LMUserData*)ui->listmodi->userData(i))->m_Data) // selected
        // {
        //     ui->listmodi->item(i)->setBackground(QColor(0, 0, 255)); // blue background
        //     ui->listmodi->item(i)->setForeground(QColor(255, 255, 255)); // white text
        // }
        // else // normla
        // {
        //     ui->listmodi->item(i)->setBackground(QColor(255, 255, 255)); // white background
        //     ui->listmodi->item(i)->setForeground(QColor(0, 0, 0)); // black text
        // }
#endif
    }
    if (data != "---")
        ui->laktuell->setText(data);
    else
        ui->laktuell->setText("");
}

