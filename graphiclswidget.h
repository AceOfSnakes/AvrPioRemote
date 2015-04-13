#ifndef GRAPHICLSWIDGET
#define GRAPHICLSWIDGET

#include <QWidget>
#include <QPainter>

class GraphicLSWidget  : public QWidget {
    Q_OBJECT

public:
    GraphicLSWidget(QWidget *parent, bool input);

    void makeSmall();
//    QSize minimumSizeHint() const;
//    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void drawBox(QPainter& painter, int x, int y, bool on, QString str);

    bool        m_Input;
    int         m_BoxWidth;
    int         m_BoxHeight;
    QString     m_CurrentData;
    bool        m_IsBig;

public slots:
    void NewData(QString str);

};

#endif // GRAPHICLSWIDGET

