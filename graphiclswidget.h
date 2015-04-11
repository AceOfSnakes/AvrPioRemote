#ifndef GRAPHICLSWIDGET
#define GRAPHICLSWIDGET

#include <QWidget>
#include <QPainter>

class GraphicLSWidget  : public QWidget {
    Q_OBJECT

public:
    GraphicLSWidget(QWidget *parent, bool input);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void drawBox(QPainter& painter, int x, int y, bool on, QString str);

    bool        m_Input;
    const int   m_BoxWidth;
    const int   m_BoxHeight;
    QString     m_CurrentData;

public slots:
    void NewData(QString str);

};

#endif // GRAPHICLSWIDGET

