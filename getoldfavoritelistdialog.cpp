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

    connect((&m_Timer), SIGNAL(timeout()), this, SLOT(Timeout()));
    m_Timer.setSingleShot(true);
    m_Timer.setInterval(2000);

    connect((comm), SIGNAL(DisplayData(int, QString)),       this,   SLOT(DisplayData(int, QString)));
//    connect((this), SIGNAL(SendCmd(QString)),                parent, SLOT(SendCmd(QString)));
    connect((comm), SIGNAL(InputFunctionData(int, QString)), this,   SLOT(InputFunctionData(int, QString)));

    //StartAquire();
}


GetOldFavoriteListDialog::~GetOldFavoriteListDialog()
{
    delete ui;
}


void GetOldFavoriteListDialog::FinishAquire()
{
    m_Recording = false;
    m_Done = true;
    ui->lineEdit->setText("");
    QMessageBox::information(this, tr("AVRPioRemote"),
                             tr("The favorites list ist aquired."),
                             QMessageBox::Ok);
    QString path = QDir::currentPath() + "/" + "Favorites.xml";
    SaveFile(path, m_FavList);
    emit FavoritesAquired();
    this->close();
}


void GetOldFavoriteListDialog::Timeout()
{
   //m_Done = true;
    QString str = m_Remembered.trimmed();
    if (str == "")
    {
        m_Remembered = "";
        m_Firstline = "";
        emit SendCmd("27NW");
        return;
    }
    if (m_FavList.contains(m_Remembered))
    {
        FinishAquire();
    }
    else
    {
        m_FavList.append(m_Remembered);
        ui->listWidget->addItem(m_Remembered);
        m_Remembered = "";
        m_Firstline = "";
        emit SendCmd("27NW");
    }
}


void GetOldFavoriteListDialog::InputFunctionData(int no, QString name)
{
//    qDebug() << "INPUT " << no << " " << name;
    if (no == 45 && !m_Done)
    {
        m_Recording = true;
    }
}


void GetOldFavoriteListDialog::DisplayData(int no, QString data)
{
    if (data == "  FAVORITES   ")
    {
        m_Timer.start();
        return;
    }

    if (no == 3 && m_Recording)
    {
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

                if (m_FavList.contains(m_Remembered))
                {
                    FinishAquire();
                    return;
                }
                m_Done = false;
                ui->listWidget->addItem(m_Remembered);
                m_FavList.append(m_Remembered);
            }
            m_Remembered = "";
            m_Firstline = "";
            emit SendCmd("27NW"); // cursor down
        }
        //emit SendCmd("29NW"); // don't go back to play window
        m_Timer.start();
    }
}

bool GetOldFavoriteListDialog::GetScrolledString(QString input)
{
    input = input.mid(1);
    if (m_Firstline == input)
    {
        m_Remembered = m_Remembered.mid(0, m_Remembered.count() - m_Firstline.count() + 1);
        m_Done = true;
        return true;
    }
    QString tmp = input.mid(0, input.count() - 2);
    int pos = m_Remembered.indexOf(tmp);
//    qDebug() << "|" << input << "| |" << tmp << "| " << pos;
    if (pos == -1)
    {
        m_Remembered = input;
        m_Firstline = input;
    }
    else
    {
        m_Remembered.replace(pos, input.length(), input);
    }
    return false;
}


void GetOldFavoriteListDialog::StartAquire()
{
//    int result = QMessageBox::warning(this, tr("AVRPioRemote"),
//                                      tr("This will erase privious data and take some time to aquire the new.\nDo you want to continue?"),
//                                      QMessageBox::Ok | QMessageBox::Cancel);

//    qDebug() << "Dialog result " << result;

    ui->lineEdit->clear();
    ui->listWidget->clear();
    m_Remembered = "";
    m_Firstline = "";
    m_Done = false;
    //m_Recording = true;
    m_FavList.clear();
   // m_Recording = true;
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

    //parseBookindexElement(root);
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
