#include "listeningmodedialog.h"
#include "ui_listeningmodedialog.h"
#include <Defs.h>
#include <QDebug>
#include <receiverinterface.h>

ListeningModeDialog::ListeningModeDialog(QWidget *parent,QSettings &settings,ReceiverInterface &Comm ) :
    QDialog(parent),
    m_Settings(settings),
    m_Comm(Comm),
    ui(new Ui::ListeningModeDialog)

{
    ui->setupUi(this);
    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));
//    connect(&m_Comm,SIGNAL(ListeningModeData(QString)),this,SLOT(LMchanged(QString)));
    connect(&m_Comm,SIGNAL(Listenextended(QString)),this,SLOT(LMchanged(QString)));
            int i=0;
            while (strlen(LISTENING_MODE[i].key) != 0)
            {
                ui->listmodi->addItem(LISTENING_MODE[i].text);
                i++;
                if (LISTENING_MODE[i].key=="0050" && m_Settings.value("TunerCompatibilityMode").toBool())
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


void ListeningModeDialog::ShowListeningDialog()
{
    QString str;
    if (!isVisible())
    {
        QWidget* Parent = dynamic_cast<QWidget*>(parent());
        int x = Parent->pos().x() - Parent->width()-100;
        QPoint pos;
        pos.setX(x);
        pos.setY(Parent->pos().y());
        this->move(pos);
        this->show();
    }
    SendCmd("?L"); //aktueller Wert
    SendCmd("?S");
}


void ListeningModeDialog::on_listmodi_doubleClicked(const QModelIndex &index)
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


void ListeningModeDialog::LMchanged(QString data)
{
    if (data != "---")
        ui->laktuell->setText(data);
}

