#ifndef LABELEDSLIDER_H
#define LABELEDSLIDER_H

#include <QWidget>

namespace Ui {
class LabeledSlider;
}

class LabeledSlider : public QWidget
{
    Q_OBJECT

public:
    explicit LabeledSlider(QWidget *parent = 0);
    ~LabeledSlider();

    void Init(int max, int min, int value, bool topLabelVisible = false, QString topLabelText = "", bool bottomLabelVisible = false, QString bottomLabelText = "");
    void SetValue(int n, bool blockSig = true);
    int  GetValue();
    void SetTopLabelText(QString text);
    void SetBottomLabelText(QString text);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::LabeledSlider *ui;

signals:
    void ValueChanged(int n);

private slots:
    void OnSliderValueChanged(int);
};

#endif // LABELEDSLIDER_H
