#-------------------------------------------------
#
# Project created by QtCreator 2013-04-19T23:58:49
#
#-------------------------------------------------

QT       += core gui
QT       += network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AVRPioRemote
TEMPLATE = app


SOURCES += main.cpp \
        avrpioremote.cpp \
        Defs.cpp \
    netradiodialog.cpp \
    bluraydialog.cpp \
    aboutdialog.cpp \
    loudspeakersettingsdialog.cpp \
    tunerdialog.cpp \
    actionwithparameter.cpp \
    receiverinterface.cpp \
    playerinterface.cpp \
    testdialog.cpp \
    oldfavoritesdialog.cpp \
    getoldfavoritelistdialog.cpp \
    settingsdialog.cpp \
    logger.cpp \
    eqdialog.cpp \
    listeningmodedialog.cpp \
    usbdialog.cpp \
    zonecontroldialog.cpp \
    avsettingsdialog.cpp \
    mcacceqdialog.cpp


HEADERS  += avrpioremote.h \
         Defs.h \
    netradiodialog.h \
    bluraydialog.h \
    aboutdialog.h \
    loudspeakersettingsdialog.h \
    tunerdialog.h \
    actionwithparameter.h \
    receiverinterface.h \
    playerinterface.h \
    testdialog.h \
    oldfavoritesdialog.h \
    getoldfavoritelistdialog.h \
    settingsdialog.h \
    logger.h \
    eqdialog.h \
    listeningmodedialog.h \
    usbdialog.h \
    zonecontroldialog.h \
    avsettingsdialog.h \
    mcacceqdialog.h
 


FORMS    += avrpioremote.ui \
    netradiodialog.ui \
    bluraydialog.ui \
    aboutdialog.ui \
    loudspeakersettingsdialog.ui \
    tunerdialog.ui \
    testdialog.ui \
    oldfavoritesdialog.ui \
    getoldfavoritelistdialog.ui \
    settingsdialog.ui \
    eqdialog.ui \
    listeningmodedialog.ui \
    usbdialog.ui \
    zonecontroldialog.ui \
    avsettingsdialog.ui \
    mcacceqdialog.ui


OTHER_FILES += \
    images/cancel.png \
    images/pen.png \
    images/ok.png \
    images/save.png \
    images/Gnome-video-x-generic.svg \
    images/Gnome-media-playback-start.svg \
    images/Gnome-folder-open.svg \
    images/Gnome-emblem-photos.svg \
    images/Gnome-audio-x-generic.svg

RESOURCES += \
    avrpioremote.qrc

TRANSLATIONS = avrpioremote_en.ts \
               avrpioremote_de.ts \
               avrpioremote_ru.ts \
