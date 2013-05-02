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

    QString path = QDir::currentPath() + "/" + "Favorites.xml";
    ReadFile(path);
}

OldFavoritesDialog::~OldFavoritesDialog()
{
    delete ui;
}


void OldFavoritesDialog::InputFunctionData(int no, QString name)
{
    qDebug() << "INPUT " << no << " " << name;
    if (no == 45)
    {
    }
}


void OldFavoritesDialog::DisplayData(int no, QString data_)
{
    if (m_SelectAction && no == 3)
    {
        QString data = data_.mid(1);
        qDebug() << "<" << data << "> <" <<m_SelectedFavorite << ">";
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
                qDebug() << "1DOWN " << down << ", UP " << up;
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
                qDebug() << "1DOWN " << down << ", UP " << up;
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

    qDebug() << "Dialog result " << result;
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
        qDebug() << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString());
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine,
                        &errorColumn)) {
        qDebug() << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr);
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "OldFavoritesList") {
        qDebug() << "Error: Not an OldFavoritesList file";
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
        qDebug() << "Error: Cannot write file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString());
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
