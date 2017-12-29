#include "emphasisdialog.h"
#include "ui_emphasisdialog.h"
#include <QDebug>
#include <QHBoxLayout>

EmphasisDialog::EmphasisDialog(QWidget *parent, QSettings &settings) :
    QDialog(parent),
    ui(new Ui::EmphasisDialog),
    m_Settings(settings),
    m_BassCh1(-1),
    m_BassCh2(-1)
{
    ui->setupUi(this);

    connect(ui->bassCh1ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(BassCh1ComboBoxIndexChanged(int)));
    connect(ui->bassCh2ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(BassCh2ComboBoxIndexChanged(int)));

    QStringList responseList;
    responseList << EmphasisResponse_ILV().getResponseID();
    MsgDistributor::AddResponseListener(this, responseList);
}

EmphasisDialog::~EmphasisDialog()
{
    delete ui;
}

void EmphasisDialog::changeEvent(QEvent *e)
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

void EmphasisDialog::ReadBassChannels()
{
    int ip;
    QString str1;
    QString str2;

    ip = m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever
    str1 = QString("EQ-%1/EmphBassCh1").arg(ip);
    str2 = QString("EQ-%1/EmphBassCh2").arg(ip);

    m_BassCh1 = m_Settings.value(str1, -1).toInt();
    m_BassCh2 = m_Settings.value(str2, -1).toInt();
}

void EmphasisDialog::SaveBassChannels()
{
    int ip;
    QString str1;
    QString str2;

    ip = m_Settings.value("IP/4").toInt(); //letztes Oktett IP anhängen, falls mehrere Reciever
    str1 = QString("EQ-%1/EmphBassCh1").arg(ip);
    str2 = QString("EQ-%1/EmphBassCh2").arg(ip);

    m_Settings.setValue(str1, m_BassCh1);
    m_Settings.setValue(str2, m_BassCh2);
}

void EmphasisDialog::CreateEmphasisSlider(int count)
{
    ReadBassChannels();
    ui->bassCh1ComboBox->blockSignals(true);
    ui->bassCh2ComboBox->blockSignals(true);
    ui->bassCh1ComboBox->clear();
    ui->bassCh2ComboBox->clear();

    QLayoutItem* item;
    while ( ( item = ui->sliderLayout->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
    m_EmphasisSliders.clear();
    for(int i = 0; i < count; i++)
    {
        QString chNumber = QString("%1").arg(i + 1);
        LabeledSlider* slider = new LabeledSlider();
        slider->Init(74, 26, 50, true, chNumber);
        connect(slider, SIGNAL(ValueChanged(int)), this, SLOT(OnSliderReleased()));
        m_EmphasisSliders.append(slider);
        ui->sliderLayout->addWidget(slider);
        ui->bassCh1ComboBox->addItem(chNumber);
        ui->bassCh2ComboBox->addItem(chNumber);
    }
    ui->bassCh2ComboBox->addItem(" ");
    if (m_BassCh1 == -1)
    {
        // not set yet
        if (count == 20) // LX88
        {
            m_BassCh1 = 18;
            m_BassCh2 = 19;
        }
        else
        {
            m_BassCh1 = 7;
            m_BassCh2 = -1;
        }
        SaveBassChannels();
    }
    if (m_BassCh1 >= ui->bassCh1ComboBox->count())
        m_BassCh1 = ui->bassCh1ComboBox->count() - 1;
    ui->bassCh1ComboBox->setCurrentIndex(m_BassCh1);
    if (m_BassCh2 == -1)
    {
        ui->bassCh2ComboBox->setCurrentIndex(ui->bassCh2ComboBox->count() - 1);
    }
    else
    {
        ui->bassCh2ComboBox->setCurrentIndex(m_BassCh2);
    }
    ui->bassCh1ComboBox->blockSignals(false);
    ui->bassCh2ComboBox->blockSignals(false);
}

void EmphasisDialog::ResponseReceived(ReceivedObjectBase *response)
{
    // 5050745050505050505050505050505050506868ILV // LX88 20Ch
    // 5050385050505050505050505050505050ILV // SC2022 17Ch

    // Emphasis
    EmphasisResponse_ILV* em = dynamic_cast<EmphasisResponse_ILV*>(response);
    if (em != NULL)
    {
        const QVector<int>& emphasisData = em->GetEmphasisData();
        int count = emphasisData.count();
        if (m_EmphasisSliders.count() != count)
        {
            CreateEmphasisSlider(count);
        }
        for (int i = 0; i < count && i < m_EmphasisSliders.count(); i++)
        {
            m_EmphasisSliders[i]->blockSignals(true);
            m_EmphasisSliders[i]->SetValue(emphasisData[i]);
            m_EmphasisSliders[i]->blockSignals(false);
            if (i == 2)
                emit CenterChanged(m_EmphasisSliders[i]->GetValue());
            else if (i == m_BassCh1 || i == m_BassCh2)
                emit BassChanged(m_EmphasisSliders[i]->GetValue());
        }
        return;
    }
}

void EmphasisDialog::OnSliderReleased()
{
    QObject* sender = QObject::sender();
    LabeledSlider* slider = dynamic_cast<LabeledSlider*>(sender);
    if (slider != NULL)
    {
        for (int i = 0; i < m_EmphasisSliders.count(); i++)
        {
            if (m_EmphasisSliders[i] == slider)
            {
                if (i == 2)
                    emit CenterChanged(m_EmphasisSliders[i]->GetValue());
                else if (i == m_BassCh1 || i == m_BassCh2)
                    emit BassChanged(m_EmphasisSliders[i]->GetValue());
            }
        }
    }
    QString cmd = GetChannelString();
    cmd += "ILV";
    emit SendCmd(cmd);
}

QString EmphasisDialog::GetChannelString()
{
    QString str;
    for (int i = 0; i < m_EmphasisSliders.count(); i++)
    {
        str += QString("%1").arg(m_EmphasisSliders[i]->GetValue(), 2, 10, QChar('0'));
    }
    return str;
}

void EmphasisDialog::on_flatPushButton_clicked()
{
    QString cmd;
    for (int i = 0; i < m_EmphasisSliders.count(); i++)
    {
        cmd += "50";
        m_EmphasisSliders[i]->SetValue(50);
    }
    cmd += "ILV";
    emit SendCmd(cmd);
}

void EmphasisDialog::SetBass(int n)
{
    if (m_EmphasisSliders.size() > n)
    {
        if (m_BassCh1 != -1 && m_BassCh2 != -1)
        {
            m_EmphasisSliders[m_BassCh1]->SetValue(n);
            m_EmphasisSliders[m_BassCh2]->SetValue(n, false);
        }
        else if (m_BassCh1 != -1)
            m_EmphasisSliders[m_BassCh1]->SetValue(n, false);
        else if (m_BassCh2 != -1)
            m_EmphasisSliders[m_BassCh2]->SetValue(n, false);
    }
}

void EmphasisDialog::SetCenter(int n)
{
    if (m_EmphasisSliders.size() > n)
    {
        m_EmphasisSliders[2]->SetValue(n);
    }
}

int EmphasisDialog::GetBass()
{
    if (m_BassCh1 != -1)
        return m_EmphasisSliders[m_BassCh1]->GetValue();
    else
        return 50;
}

int EmphasisDialog::GetCenter()
{
    return m_EmphasisSliders[2]->GetValue();
}

void EmphasisDialog::BassCh1ComboBoxIndexChanged(int n)
{
    m_BassCh1 = n;
    SaveBassChannels();
}

void EmphasisDialog::BassCh2ComboBoxIndexChanged(int n)
{
    if (n >= m_EmphasisSliders.count())
        m_BassCh2 = -1;
    else
        m_BassCh2 = n;
    SaveBassChannels();
}

void EmphasisDialog::on_resetPushButton_clicked()
{
    ui->bassCh1ComboBox->blockSignals(true);
    ui->bassCh2ComboBox->blockSignals(true);
    // reset
    if (m_EmphasisSliders.count() == 20) // LX88
    {
        m_BassCh1 = 18;
        m_BassCh2 = 19;
    }
    else
    {
        m_BassCh1 = 7;
        m_BassCh2 = -1;
    }
    SaveBassChannels();
    if (m_BassCh1 >= ui->bassCh1ComboBox->count())
        m_BassCh1 = ui->bassCh1ComboBox->count() - 1;
    ui->bassCh1ComboBox->setCurrentIndex(m_BassCh1);
    if (m_BassCh2 == -1)
    {
        ui->bassCh2ComboBox->setCurrentIndex(ui->bassCh2ComboBox->count() - 1);
    }
    else
    {
        ui->bassCh2ComboBox->setCurrentIndex(m_BassCh2);
    }
    ui->bassCh1ComboBox->blockSignals(false);
    ui->bassCh2ComboBox->blockSignals(false);
    emit BassChanged(GetBass());
}

void EmphasisDialog::SetChannelString(QString str)
{
    EmphasisResponse_ILV emph;
    emph.parseString("ILV" + str);
    ResponseReceived(&emph);
    OnSliderReleased();
}
