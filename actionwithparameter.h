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
#ifndef ACTIONWITHPARAMETER_H
#define ACTIONWITHPARAMETER_H

#include <QAction>

class ActionWithParameter : public QAction
{
    Q_OBJECT
public:
    explicit ActionWithParameter(QWidget* parent, const QString& Param);
    explicit ActionWithParameter(QWidget* parent, const QString& Name, const QString& Param);
private:
    QString m_Param;
public slots:
    void ActionTriggered();
signals:
    void ActionTriggered(const QString& Param);
};

#endif // ACTIONWITHPARAMETER_H
