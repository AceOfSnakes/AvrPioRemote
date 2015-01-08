#include "labeledslider.h"
#include "ui_labeledslider.h"
#include <QSlider>
#include <QLabel>

LabeledSlider::LabeledSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LabeledSlider)
{
    ui->setupUi(this);
}

LabeledSlider::~LabeledSlider()
{
    delete ui;
}

void LabeledSlider::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        //ui->retranslateUi(this);
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void LabeledSlider::retranslateUi(QWidget */*LabeledSlider*/)
{
//    LabeledSlider->setWindowTitle(QApplication::translate("LabeledSlider", "Form", 0));
//    topLabel->setText(QApplication::translate("LabeledSlider", "top", 0));
//    bottomLabel->setText(QApplication::translate("LabeledSlider", "bottom", 0));
} // retranslateUi


void LabeledSlider::Init(int max, int min, int value, bool topLabelVisible, QString topLabelText, bool bottomLabelVisible, QString bottomLabelText)
{
    ui->slider->setMaximum(max);
    ui->slider->setMinimum(min);
    ui->slider->setValue(value);
    ui->slider->setSingleStep(1);
    connect(ui->slider, SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    ui->topLabel->setVisible(topLabelVisible);
    ui->topLabel->setText(topLabelText);
    ui->bottomLabel->setVisible(bottomLabelVisible);
    ui->bottomLabel->setText(bottomLabelText);
}

void LabeledSlider::LabeledSlider::SetValue(int n, bool blockSig)
{
    if (blockSig)
        ui->slider->blockSignals(true);
    ui->slider->setValue(n);
    if (blockSig)
        ui->slider->blockSignals(false);
}

int LabeledSlider::GetValue()
{
    return ui->slider->value();
}

void LabeledSlider::SetTopLabelText(QString text)
{
    ui->topLabel->setText(text);
}

void LabeledSlider::SetBottomLabelText(QString text)
{
    ui->bottomLabel->setText(text);
}

void LabeledSlider::OnSliderValueChanged(int n)
{
    emit ValueChanged(n);
}
