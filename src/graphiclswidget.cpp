#include "graphiclswidget.h"

#include <QRect>

GraphicLSWidget::GraphicLSWidget(QWidget *parent, bool input)
    : QWidget(parent),
      m_Input(input),
      m_BoxWidth(30),
      m_BoxHeight(22),
      m_IsBig(true)
{
    reset();
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    resize(10 + m_BoxWidth * 5, 10 + m_BoxHeight * 6);
    if (input)
        setToolTip(tr("Input LS configuration"));
    else
        setToolTip(tr("Output LS configuration"));
}

void GraphicLSWidget::makeSmall()
{
    m_BoxWidth = 8,
    m_BoxHeight = 6,
    m_IsBig = false;
    setStyleSheet("border: 1px solid red");
    resize(6 + m_BoxWidth * 5, 6 + m_BoxHeight * 6);
}

//QSize GraphicLSWidget::sizeHint() const
//{
//    return QSize(10 + m_BoxWidth * 5, 10 + m_BoxHeight * 6);
//}

//QSize GraphicLSWidget::minimumSizeHint() const
//{
//    return QSize(10 + m_BoxWidth * 5, 10 + m_BoxHeight * 6);
//}

void GraphicLSWidget::paintEvent(QPaintEvent * /* event */)
{
    if (m_Input) {
        if (m_CurrentData.length() >= 16) {
            QPainter painter(this);
            //painter.setRenderHint(QPainter::Antialiasing, true);

            drawBox(painter, 1, 1, m_CurrentData[0] != '0', "L");
            drawBox(painter, 2, 1, m_CurrentData[1] != '0', "C");
            drawBox(painter, 3, 1, m_CurrentData[2] != '0', "R");

            drawBox(painter, 1, 2, m_CurrentData[3] != '0', "SL");
            drawBox(painter, 3, 2, m_CurrentData[4] != '0', "SR");

            drawBox(painter, 1, 3, m_CurrentData[5] != '0', "SBL");
            drawBox(painter, 2, 3, m_CurrentData[6] != '0', "SC");
            drawBox(painter, 3, 3, m_CurrentData[7] != '0', "SBR");

            drawBox(painter, 2, 4, m_CurrentData[8] != '0', "LFE");

            drawBox(painter, 1, 0, m_CurrentData[9] != '0', "FHL");
            drawBox(painter, 3, 0, m_CurrentData[10] != '0', "FHR");

            drawBox(painter, 0, 1, m_CurrentData[11] != '0', "FWL");
            drawBox(painter, 4, 1, m_CurrentData[12] != '0', "FWR");

            drawBox(painter, 1, 5, m_CurrentData[13] != '0', "XL");
            drawBox(painter, 2, 5, m_CurrentData[14] != '0', "XC");
            drawBox(painter, 3, 5, m_CurrentData[15] != '0', "XR");
        }
    } else {
        if (m_CurrentData.length() >= 13) {
            QPainter painter(this);
            //painter.setRenderHint(QPainter::Antialiasing, true);

            drawBox(painter, 1, 1, m_CurrentData[0] != '0', "L");
            drawBox(painter, 2, 1, m_CurrentData[1] != '0', "C");
            drawBox(painter, 3, 1, m_CurrentData[2] != '0', "R");

            drawBox(painter, 1, 2, m_CurrentData[3] != '0', "SL");
            drawBox(painter, 3, 2, m_CurrentData[4] != '0', "SR");

            drawBox(painter, 1, 3, m_CurrentData[5] != '0', "SBL");
            drawBox(painter, 2, 3, m_CurrentData[6] != '0', "SB");
            drawBox(painter, 3, 3, m_CurrentData[7] != '0', "SBR");

            drawBox(painter, 2, 4, m_CurrentData[8] != '0', "SW");

            drawBox(painter, 1, 0, m_CurrentData[9] != '0', "FHL");
            drawBox(painter, 3, 0, m_CurrentData[10] != '0', "FHR");

            drawBox(painter, 0, 1, m_CurrentData[11] != '0', "FWL");
            drawBox(painter, 4, 1, m_CurrentData[12] != '0', "FWR");
        }
    }


    //painter.setBrush(QBrush(Qt::green)); //style=Qt::ConicalGradientPattern, Qt::TexturePattern, Qt::RadialGradientPattern, Qt::LinearGradientPattern
    // Qt::SolidPattern, Qt::HorPattern, Qt::VerPattern, Qt::CrossPattern,
    //painter.drawRect(rect);
    //painter.drawRoundedRect(rect, 25, 25, Qt::RelativeSize);
    //painter.drawText(rect, Qt::AlignCenter, tr("Qt by\nDigia"));
    //painter.drawPixmap(10, 10, pixmap);
    //painter.setRenderHint(QPainter::Antialiasing, false);
    //painter.setPen(palette().dark().color());
    //painter.setBrush(Qt::NoBrush);
    //painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

void GraphicLSWidget::drawBox(QPainter& painter, int x, int y, bool on, QString str)
{
    if (m_IsBig) {
        if (on) {
            painter.setBrush(QColor(0, 170, 255)/*QBrush(Qt::green)*/); //style=Qt::ConicalGradientPattern, Qt::TexturePattern, Qt::RadialGradientPattern, Qt::LinearGradientPattern
            painter.setPen(QColor(0, 170, 255));
            painter.drawRect(5 + x * m_BoxWidth, 5 + y * m_BoxHeight, m_BoxWidth - 2, m_BoxHeight - 2);
            painter.setPen(palette().brightText().color());
            painter.drawText(7 + x * m_BoxWidth, 5 + y * m_BoxHeight, m_BoxWidth - 2, m_BoxHeight - 2, Qt::AlignCenter, str);
        }
        else {
            painter.setBrush(Qt::NoBrush);
            painter.setPen(palette().dark().color());
            painter.drawRect(5 + x * m_BoxWidth, 5 + y * m_BoxHeight, m_BoxWidth - 2, m_BoxHeight - 2);
            painter.drawText(7 + x * m_BoxWidth, 5 + y * m_BoxHeight, m_BoxWidth - 2, m_BoxHeight - 2, Qt::AlignCenter, str);
        }
    } else {
        if (on) {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(QColor(0, 170, 255))); //style=Qt::ConicalGradientPattern, Qt::TexturePattern, Qt::RadialGradientPattern, Qt::LinearGradientPattern
            if (m_Input)
                painter.drawRect(3 + x * m_BoxWidth, 3 + y * m_BoxHeight, m_BoxWidth - 1, m_BoxHeight - 1);
            else
                painter.drawRect(3 + x * m_BoxWidth, 6 + y * m_BoxHeight, m_BoxWidth - 1, m_BoxHeight - 1);
        }
        else {
            painter.setBrush(Qt::NoBrush);
            painter.setPen(palette().dark().color());
            if (m_Input)
                painter.drawRect(3 + x * m_BoxWidth, 3 + y * m_BoxHeight, m_BoxWidth - 2, m_BoxHeight - 2);
            else
                painter.drawRect(3 + x * m_BoxWidth, 6 + y * m_BoxHeight, m_BoxWidth - 2, m_BoxHeight - 2);
        }
        painter.setPen(QColor(220, 220, 220));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(0, 0, size().width() - 1, size().height() - 1);
    }
}

void GraphicLSWidget::NewData(QString str)
{
    m_CurrentData = str;
    this->update();
}

void GraphicLSWidget::reset()
{
    m_CurrentData = "00000000000000000000";
    this->update();
}
