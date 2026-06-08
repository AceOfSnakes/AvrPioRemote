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
#include "avrpioremote.h"
#include <QApplication>
#include "singleapplication.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Prevent the tray icon app from closing when hidden                                                                   QApplication::setQuitOnLastWindowClosed(false);

    // Provide a completely unique key for your application
    SingleApplication singleApp(QString("com.github.AceOfSnakes.").append(APPLICATION_NAME));
    if (!singleApp.checkInstance()) {
        return 0; // Secondary copy exits instantly, giving focus back to Windows
    }

    AVRPioRemote w;
    w.show();

    // Connect incoming wake-up signals from taskbar/new launches
    QObject::connect(&singleApp, &SingleApplication::wakeUpRequested, &w, [&w]() {
        w.wakeUp();
    });

    return app.exec();

    /*QApplication a(argc, argv);
    AVRPioRemote w;
    w.show();

    return a.exec();*/
}
