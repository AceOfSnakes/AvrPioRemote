#include "graphiclswidget.h"

#include <QRect>

GraphicLSWidget::GraphicLSWidget(QWidget *parent, bool input)
    : QWidget(parent),
      m_Input(input),
      m_BoxWidth(30),
      m_BoxHeight(22)
{

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    resize(10 + m_BoxWidth * 5, 10 + m_BoxHeight * 6);
}

QSize GraphicLSWidget::sizeHint() const
{
    return QSize(10 + m_BoxWidth * 5, 10 + m_BoxHeight * 6);
}

QSize GraphicLSWidget::minimumSizeHint() const
{
    return QSize(10 + m_BoxWidth * 5, 10 + m_BoxHeight * 6);
}

void GraphicLSWidget::paintEvent(QPaintEvent * /* event */)
{
    if (m_Input) {
        if (m_CurrentData.length() >= 16) {
            QPainter painter(this);
            //painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(palette().dark().color());

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
            painter.setPen(palette().dark().color());

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
    if (on)
        painter.setBrush(QBrush(Qt::green)); //style=Qt::ConicalGradientPattern, Qt::TexturePattern, Qt::RadialGradientPattern, Qt::LinearGradientPattern
    else
        painter.setBrush(Qt::NoBrush);

    painter.drawRect(5 + x * m_BoxWidth, 5 + y * m_BoxHeight, m_BoxWidth - 2, m_BoxHeight - 2);
    painter.drawText(7 + x * m_BoxWidth, 5 + y * m_BoxHeight, m_BoxWidth - 2, m_BoxHeight - 2, Qt::AlignCenter, str);
}

void GraphicLSWidget::NewData(QString str)
{
    m_CurrentData = str;
    this->update();
}
