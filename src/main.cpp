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
//    qputenv("QT_WIDGETS_HICONTRACT_STYLING", "0");
    int customArgc;
    char** customArgv = argv;

#ifdef Q_OS_WINDOWS
//#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    customArgc = argc + 2;
    customArgv = new char*[customArgc];
    for (int i = 0; i < argc; ++i) {
        customArgv[i] = argv[i];
    }
    customArgv[argc] = const_cast<char*>("-platform");
    customArgv[argc + 1] = const_cast<char*>("windows:dpiawareness=0");
//#endif
#endif
    QApplication app(customArgc, customArgv);



    //Q_INIT_RESOURCE(avrpioremote);
    app.setApplicationName(APPLICATION_NAME);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    app.setDesktopFileName("avrpioremote-qt5");
#else
    app.setDesktopFileName("avrpioremote");
#endif
    SingleApplication singleApp(QString("com.github.AceOfSnakes.").append(APPLICATION_NAME));
    if(AVRPioRemote::isSingleApp()) {
        if (!singleApp.checkInstance()) {
            return 0; // Secondary copy exits instantly, giving focus back to Windows
        }
    }
    AVRPioRemote w;
    QObject::connect(&singleApp, &SingleApplication::wakeUpRequested, &w, &AVRPioRemote::wakeUp);
    w.show();
    return app.exec();
}
