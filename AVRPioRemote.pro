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

static { # everything below takes effect with CONFIG += static
    CONFIG += static
#    CONFIG += staticlib # this is needed if you create a static library, not a static executable
    DEFINES += STATIC
    message("~~~ static build ~~~") # this is for information, that the static build is done
    win32: TARGET = $$join(TARGET,,,) #this adds an s in the end, so you can seperate static build from non static build
}
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
    mcacceqdialog.cpp \
    autosearchdialog.cpp \
    wiringdialog.cpp \
    wiringmodel.cpp \
    comboboxdelegate.cpp \
    checkboxdelegate.cpp \
    emphasisdialog.cpp \
    labeledslider.cpp


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
    mcacceqdialog.h \
    autosearchdialog.h \
    wiringdialog.h \
    wiringmodel.h \
    comboboxdelegate.h \
    checkboxdelegate.h \
    emphasisdialog.h \
    labeledslider.h
 


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
    mcacceqdialog.ui \
    autosearchdialog.ui \
    wiringdialog.ui \
    emphasisdialog.ui \
    labeledslider.ui


OTHER_FILES += \
    images/cancel.png \
    images/pen.png \
    images/ok.png \
    images/save.png \
    images/Gnome-video-x-generic.svg \
    images/Gnome-media-playback-start.svg \
    images/Gnome-folder-open.svg \
    images/Gnome-emblem-photos.svg \
    images/Gnome-audio-x-generic.svg \
    android/AndroidManifest.xml \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/version.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java

RESOURCES += \
    avrpioremote.qrc

TRANSLATIONS = avrpioremote_en.ts \
               avrpioremote_de.ts \
               avrpioremote_ru.ts \
