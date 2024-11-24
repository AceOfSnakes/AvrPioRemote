/*
 * AVRPioRemote
 * Copyright (C) 2013  Andreas Müller, Ulrich Mensfeld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "actionwithparameter.h"

ActionWithParameter::ActionWithParameter(QWidget *parent, const QString &Param)
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    : QAction(parent)
#else
    : QAction((QObject *)parent)
#endif
{
    m_Param = Param;
    connect(this, SIGNAL(triggered()), this, SLOT(ActionTriggered()));
}

ActionWithParameter::ActionWithParameter(QWidget* parent, const QString& Name, const QString& Param)
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    : QAction(Name, parent)
#else
    : QAction((QObject *)parent)
#endif

{
    m_Param = Param;
    connect(this, SIGNAL(triggered()), this, SLOT(ActionTriggered()));
}

void ActionWithParameter::ActionTriggered()
{
    emit ActionTriggered(m_Param);
}
