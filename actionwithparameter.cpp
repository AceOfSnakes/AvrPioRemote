#include "actionwithparameter.h"

ActionWithParameter::ActionWithParameter(QWidget *parent, const QString &Param) : QAction(parent)
{
    m_Param = Param;
    connect(this, SIGNAL(triggered()), this, SLOT(ActionTriggered()));
}

ActionWithParameter::ActionWithParameter(QWidget* parent, const QString& Name, const QString& Param) : QAction(Name, parent)
{
    m_Param = Param;
    connect(this, SIGNAL(triggered()), this, SLOT(ActionTriggered()));
}

void ActionWithParameter::ActionTriggered()
{
    emit ActionTriggered(m_Param);
}
