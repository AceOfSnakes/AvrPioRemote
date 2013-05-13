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
#include "oldfavoritesdialog.h"
#include "ui_oldfavoritesdialog.h"
#include <QDebug>
#include <QFile>
#include <QDomDocument>
#include <QDir>

OldFavoritesDialog::OldFavoritesDialog(QWidget *parent, ReceiverInterface *comm) :
    QDialog(parent),
    ui(new Ui::OldFavoritesDialog),
    m_Comm(comm),
    m_SelectAction(false),
    m_Direction(DirectionNone)
{
    ui->setupUi(this);

    this->setFixedSize(this->size());

    connect((comm), SIGNAL(DisplayData(int, QString)),       this,   SLOT(DisplayData(int, QString)));
    connect((this), SIGNAL(SendCmd(QString)),                parent, SLOT(SendCmd(QString)));
    connect((comm), SIGNAL(InputFunctionData(int, QString)), this,   SLOT(InputFunctionData(int, QString)));

//    QString path = QDir::currentPath() + "/" + "Favorites.xml";
//    ReadFile(path);
}

OldFavoritesDialog::~OldFavoritesDialog()
{
    delete ui;
}


void OldFavoritesDialog::ShowOldFavoritesDialog()
{
    if (!this ->isVisible())
    {
        QWidget* Parent = dynamic_cast<QWidget*>(parent());
        int x = Parent->pos().x() + Parent->width() + 20;
        QPoint pos;
        pos.setX(x);
        pos.setY(Parent->pos().y());
        this->move(pos);
        this->show();
    }
}


void OldFavoritesDialog::InputFunctionData(int no, QString/* name*/)
{
    if (no == 45)
    {
    }
}


void OldFavoritesDialog::DisplayData(int no, QString data_)
{
    if (m_SelectAction && no == 3)
    {
        QString data = data_.mid(1);
//        qDebug() << "<" << data << "> <" <<m_SelectedFavorite << ">";
        if (m_SelectedFavorite.startsWith(data))
        {
            emit SendCmd("10NW"); // Favorite found, press play
            m_SelectAction = false;
            m_Direction = DirectionNone;
            return;
        }

        if (m_Direction == DirectionNone)
        {
            int idxCurrent = FindFavorite(data);
            int idxWanted = FindFavorite(m_SelectedFavorite);
            if (idxWanted > idxCurrent)
            {
                int down = idxWanted - idxCurrent;
                int up = m_FavList.count() - idxWanted + idxCurrent;
                if (down <= up)
                {
                    m_Direction = DirectionDown;
                }
                else
                {
                    m_Direction = DirectionUp;
                }
                return;
            }
            else
            {
                int up = idxCurrent - idxWanted;
                int down = m_FavList.count() - idxCurrent + idxWanted;
                if (down <= up)
                {
                    m_Direction = DirectionDown;
                }
                else
                {
                    m_Direction = DirectionUp;
                }
                return;
            }
        }

        if (m_Direction == DirectionDown)
        {
            emit SendCmd("27NW");
        }
        else if (m_Direction == DirectionUp)
        {
            emit SendCmd("26NW");
        }
    }
}


int OldFavoritesDialog::FindFavorite(const QString& fav)
{
    QString str = fav.trimmed();
    for(int i = 0; i < m_FavList.count(); i++)
    {
        if (m_FavList[i].startsWith(str))
        {
            //qDebug() << "FOUND FAVORITE";
            return i;
        }
    }
    return -1;
}


void OldFavoritesDialog::on_GetFavoritesButton_clicked()
{
    int result = QMessageBox::warning(this, tr("AVRPioRemote"),
                                      tr("This will erase privious data and take some time to aquire the new.\nDo you want to continue?"),
                                      QMessageBox::Ok | QMessageBox::Cancel);

    if (result == QMessageBox::Ok)
    {
        GetOldFavoriteListDialog* dialog = new GetOldFavoriteListDialog(this, m_Comm);
        connect(dialog, SIGNAL(SendCmd(QString)), parent(), SLOT(SendCmd(QString)));
        connect(dialog, SIGNAL(FavoritesAquired()), this, SLOT(FavoritesAquired()));
        dialog->setModal(true);
        dialog->ShowDialog();
    }

}

bool OldFavoritesDialog::ReadFile(const QString& fileName)
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
    if (root.tagName() != "OldFavoritesList") {
        QString msg = QString("Error: Not the favorites list (OldFavoritesList root element missing)");
        Logger::Log(msg);
        return false;
    }
    QDomNodeList nodes = root.childNodes();
    for (int i = 0; i < nodes.count(); i++)
    {
        if (nodes.at(i).isElement() && nodes.at(i).nodeName() == "List")
        {
            QDomNodeList FavList = nodes.at(i).childNodes();
            for (int j = 0; j < FavList.count(); j++)
            {
                if (FavList.at(j).isElement() && FavList.at(j).nodeName() == "Favorite")
                {
                    //qDebug() << FavList.at(j).attributes().namedItem("Name").nodeValue();
                    QString name = FavList.at(j).attributes().namedItem("Name").nodeValue();
                    ui->listWidget->addItem(name);
                    m_FavList.append(name);
                }
            }
        }
    }

    //parseBookindexElement(root);
    file.close();
    return true;
}

bool OldFavoritesDialog::SaveFile(const QString& fileName, const QStringList& favs)
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

//    QDomElement translation = doc.createElement("translation");
//    QDomText latin = doc.createTextNode("Ars longa vita brevis");
//    QDomText english = doc.createTextNode("Art is long, life is short");
    doc.appendChild(Root);
    Root.appendChild(List);
//    root.appendChild(translation);
//    quote.appendChild(latin);
//    translation.appendChild(english);

//    QDomNode xmlNode = doc.createProcessingInstruction("xml",
//                               "version=\"1.0\" encoding=\"ISO-8859-1\"");
//    doc.insertBefore(xmlNode, doc.firstChild());

    QTextStream out(&file);
    doc.save(out, Indent);
    file.close();
    return true;
}

void OldFavoritesDialog::FavoritesAquired()
{
    ui->listWidget->clear();
    QString path = QDir::currentPath() + "/" + "Favorites.xml";
    ReadFile(path);
}

void OldFavoritesDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    m_SelectedFavorite = item->text();
    m_SelectAction = true;
    m_Direction = DirectionNone;
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        QBrush brush(QColor(0, 0, 0));
        ui->listWidget->item(i)->setForeground(brush);
    }
    QBrush brush(QColor(0, 0, 255));
    item->setForeground(brush);
    emit SendCmd("45FN"); // switch to favorites
    emit SendCmd("31NW"); // go back to favorite menu if we are in the playing window
    emit SendCmd("27NW"); // go to next favorite and trigger the display
}
