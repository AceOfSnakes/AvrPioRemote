#include "infodialog.h"
#include "ui_infodialog.h"
#include "graphiclswidget.h"
#include <QLayout>
#include <QDebug>

InfoDialog::InfoDialog(QWidget *parent, QSettings &settings, ReceiverInterface &Comm) :
    QDialog(parent),
    ui(new Ui::InfoDialog),
    inputLSConfiguration(NULL),
    outputLSConfiguration(NULL),
    m_Settings(settings),
    m_Comm(Comm)
{
    ui->setupUi(this);
    inputLSConfiguration = new GraphicLSWidget(this, true);
    outputLSConfiguration = new GraphicLSWidget(this, false);

    int w = inputLSConfiguration->width();
    int h = height();// inputLSConfiguration->height();

    inputLSConfiguration->move(10, 30);
    outputLSConfiguration->move(20 + w, 30);

    resize(inputLSConfiguration->width() * 2 + 30, h/* + 40*/);
    ui->InputCaptionLabel->setGeometry(10, 10, w, 20);
    ui->OutputCaptionLabel->setGeometry(20 + w, 10, w, 20);

    // restore the position of the window
    if (m_Settings.value("SaveInfoWindowGeometry", false).toBool())
    {
        m_PositionSet = restoreGeometry(m_Settings.value("InfoWindowGeometry").toByteArray());
    }

    connect(this, SIGNAL(SendCmd(QString)), &m_Comm, SLOT(SendCmd(QString)));

    QStringList responseList;
    responseList << AudioStatusDataResponse_AST().getResponseID();
    responseList << VideoStatusDataResponse_VST().getResponseID();
    MsgDistributor::AddResponseListener(this, responseList);
}

InfoDialog::~InfoDialog()
{
    delete ui;
    delete inputLSConfiguration;
    delete outputLSConfiguration;
}

void InfoDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void InfoDialog::closeEvent(QCloseEvent *event)
{
    //m_Comm.disconnect(SIGNAL(DataReceived(QString)));
    QDialog::closeEvent(event);
}

void InfoDialog::moveEvent(QMoveEvent* event)
{
    m_Settings.setValue("InfoWindowGeometry", saveGeometry());
    QDialog::moveEvent(event);
}

void InfoDialog::ShowInfoDialog()
{
    if (!isVisible())
    {
        if (!m_PositionSet || !m_Settings.value("SaveInfoWindowGeometry", false).toBool())
        {
            QWidget* Parent = dynamic_cast<QWidget*>(parent());
            int x = Parent->pos().x() + Parent->width() + 20;
            QPoint pos;
            pos.setX(x);
            pos.setY(Parent->pos().y());
            this->move(pos);
        }
        this->show();
    } else {
        this->hide();
    }
    SendCmd("?AST");
    SendCmd("?VST");
}

void InfoDialog::ResponseReceived(ReceivedObjectBase *response)
{
    // AST
    AudioStatusDataResponse_AST* ast = dynamic_cast<AudioStatusDataResponse_AST*>(response);
    if (ast != NULL)
    {
        inputLSConfiguration->NewData(ast->iChFormat);
        outputLSConfiguration->NewData(ast->oChFormat);
        m_AudioInfo = ast->getSummary();
        ui->infoText->setText("AUDIO:\n" + m_AudioInfo + "\nVIDEO:\n" + m_VideoInfo + "\n");
        return;
    }
    // VST
    VideoStatusDataResponse_VST* vst = dynamic_cast<VideoStatusDataResponse_VST*>(response);
    if (vst != NULL)
    {
        m_VideoInfo = vst->getSummary();
        ui->infoText->setText("AUDIO:\n" + m_AudioInfo + "\nVIDEO:\n" + m_VideoInfo + "\n");
        return;
    }
}

void InfoDialog::Reset()
{
    m_AudioInfo = "";
    m_VideoInfo = "";
    ui->infoText->setText("AUDIO:\n" + m_AudioInfo + "\nVIDEO:\n" + m_VideoInfo + "\n");
    inputLSConfiguration->reset();
    outputLSConfiguration->reset();
}
