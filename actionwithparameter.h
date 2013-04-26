#ifndef ACTIONWITHPARAMETER_H
#define ACTIONWITHPARAMETER_H

#include <QAction>

class ActionWithParameter : public QAction
{
    Q_OBJECT
public:
    explicit ActionWithParameter(QWidget* parent, const QString& Param);
private:
    QString m_Param;
public slots:
    void ActionTriggered();
signals:
    void ActionTriggered(const QString& Param);
};

#endif // ACTIONWITHPARAMETER_H
