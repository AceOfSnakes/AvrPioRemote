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
#include "getoldfavoritelistdialog.h"
#include "ui_getoldfavoritelistdialog.h"
#include <QDebug>
#include <QFile>
#include <QDomDocument>
#include <QDir>

GetOldFavoriteListDialog::GetOldFavoriteListDialog(QWidget *parent, ReceiverInterface* comm) :
    QDialog(parent),
    ui(new Ui::GetOldFavoriteListDialog),
    m_Timer(this),
    m_Comm(comm),
    m_CurrentAction(AquireActionAquire)
{
    ui->setupUi(this);

    this->setFixedSize(this->size());

    m_Done = true;
    m_Recording = false;

    // this timer is used to recognize the short text that is not scrolling
    // if the timer is fired we have a short text
    connect((&m_Timer), SIGNAL(timeout()), this, SLOT(Timeout()));
    m_Timer.setSingleShot(true);
    m_Timer.setInterval(2000);

    connect((comm), SIGNAL(DisplayData(int, QString)),       this,   SLOT(DisplayData(int, QString)));
    connect((comm), SIGNAL(InputFunctionData(int, QString)), this,   SLOT(InputFunctionData(int, QString)));
}


GetOldFavoriteListDialog::~GetOldFavoriteListDialog()
{
    delete ui;
}


void GetOldFavoriteListDialog::FinishAquire()
{
    // the download of the favorite list is ended
    m_Recording = false;
    m_Done = true;
    ui->lineEdit->setText("");
    // save the favorite list
    QString path = QDir::currentPath() + "/" + "Favorites.xml";
    SaveFile(path, m_FavList);
    emit FavoritesAquired();
    // notify the user
    QMessageBox::information(this, tr("AVRPioRemote"),
                             tr("The favorites list is aquired."),
                             QMessageBox::Ok);
    this->close();
}


void GetOldFavoriteListDialog::Timeout()
{
    // the timer was fired, that means the shown string is short and is not scrolled
    QString str = m_Remembered.trimmed();
    if (str == "")
    {
        m_Remembered = "";
        m_Firstline = "";
        emit SendCmd("27NW");
        return;
    }
    // if the string is already in the list, we are ready (list is wrapped)
    if (m_FavList.contains(m_Remembered))
    {
        FinishAquire();
    }
    else
    {
        // save the string in the list and go down
        m_FavList.append(m_Remembered);
        ui->listWidget->addItem(m_Remembered);
        m_Remembered = "";
        m_Firstline = "";
        emit SendCmd("27NW"); // cursor down
    }
}


void GetOldFavoriteListDialog::InputFunctionData(int no, QString/* name*/)
{
    // only if the favorite input is selected, save the strings
    if (no == 45 && !m_Done)
    {
        m_Recording = true;
    }
}


void GetOldFavoriteListDialog::DisplayData(int no, QString data)
{
    // ignore this string
    if (data == "  FAVORITES   ")
    {
        m_Timer.start();
        return;
    }

    // 3 is the normal display type
    if (no == 3 && m_Recording)
    {
        // disarm the timer
        m_Timer.stop();
        if (!m_Done)
        {
            GetScrolledString(data);
            ui->lineEdit->setText(m_Remembered + "...");
        }
        else
        {
            ui->lineEdit->setText("");
            QString str = m_Remembered.trimmed();
            if (str != "")
            {
                // if the string is already in the list, we are ready (list is wrapped)
                if (m_FavList.contains(m_Remembered))
                {
                    FinishAquire();
                    return;
                }
                // otherwise save the string
                m_Done = false;
                ui->listWidget->addItem(m_Remembered);
                m_FavList.append(m_Remembered);
            }
            m_Remembered = "";
            m_Firstline = "";
            emit SendCmd("27NW"); // cursor down
        }
        //emit SendCmd("29NW"); // don't go back to play window
        // rearm the timer for short not scrolling strings
        m_Timer.start();
    }
}

// a little bit of magic to compose a list of scrolling shorter strings into
// a long one
bool GetOldFavoriteListDialog::GetScrolledString(QString input)
{
    input = input.mid(1); // ignore the first character, it's a symbol
    // if the first seen string equals to the current one, the string is wrapped
    // and we are done
    if (m_Firstline == input)
    {
        // there is unneded data at the end of the string
        m_Remembered = m_Remembered.mid(0, m_Remembered.count() - m_Firstline.count() + 1);
        m_Done = true;
        return true;
    }
    // remove the end of the string, so the indexOf is working
    QString tmp = input.mid(0, input.count() - 2);
    // find the current string in the saved
    int pos = m_Remembered.indexOf(tmp);
//    qDebug() << "|" << input << "| |" << tmp << "| " << pos;
    if (pos == -1)
    {
        // nothing is found, so take the current string as the beginning
        // of our new string
        m_Remembered = input;
        m_Firstline = input;
    }
    else
    {
        // integrate the current string in to our saved string
        // it get only one character longer
        m_Remembered.replace(pos, input.length(), input);
    }
    return false;
}


void GetOldFavoriteListDialog::StartAquire()
{
    // start favorite list download
    ui->lineEdit->clear();
    ui->listWidget->clear();
    m_Remembered = "";
    m_Firstline = "";
    m_Done = false;
    m_FavList.clear();
    emit SendCmd("20NW"); // stop
    emit SendCmd("45FN"); // switch to favorites
    emit SendCmd("?F"); // switch to favorites
//    emit SendCmd("27NW");
    m_Timer.start();
}


bool GetOldFavoriteListDialog::ReadFile(const QString& fileName)
{
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
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "bookindex") {
        QString msg = QString("Error: Not the favorites list");
        Logger::Log(msg);
        return false;
    }

    file.close();
    return true;
}

bool GetOldFavoriteListDialog::SaveFile(const QString& fileName, const QStringList& favs)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QString msg = QString("Error: Cannot write file <%1>: %2").arg(fileName).arg(file.errorString());
        Logger::Log(msg);
        return false;
    }


    const int Indent = 4;
    QDomDocument doc;
    QDomElement Root = doc.createElement("OldFavoritesList");
    QDomElement List = doc.createElement("List");
    for (int i = 0; i < favs.count(); i++)
    {
        QDomElement Fav = doc.createElement("Favorite");
        Fav.setAttribute("Name", favs[i]);
        List.appendChild(Fav);
    }

    doc.appendChild(Root);
    Root.appendChild(List);

    QTextStream out(&file);
    doc.save(out, Indent);
    file.close();
    return true;
}

void GetOldFavoriteListDialog::on_CancelButton_clicked()
{
    this->close();
}

void GetOldFavoriteListDialog::ShowDialog()
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
    StartAquire();
}
