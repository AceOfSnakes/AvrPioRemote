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
win32: RC_ICONS = pio-icon1.ico

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
    playerinterface.cpp \
    testdialog.cpp \
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
    labeledslider.cpp \
    hdmicontroldialog.cpp \
    receiver_interface/msgdistributor.cpp \
    receiver_interface/response/displaydataresponse.cpp \
    receiver_interface/response/inputfunctionresponse.cpp \
    receiver_interface/response/mcaccnumberresponse.cpp \
    receiver_interface/response/powerresponse.cpp \
    receiver_interface/response/receivedobjectbase.cpp \
    receiver_interface/response/responselistener.cpp \
    receiver_interface/command/cmdbase.cpp \
    receiver_interface/receiverinterface.cpp \
    receiver_interface/response/hdmipassthroughresponse.cpp \
    receiver_interface/response/hdmicontrolresponse.cpp \
    receiver_interface/response/hdmicontrolmoderesponse.cpp \
    receiver_interface/response/hdmicontrolarcresponse.cpp \
    receiver_interface/response/volumeresponse.cpp \
    receiver_interface/response/muteresponse.cpp \
    receiver_interface/response/inputnameresponse.cpp \
    receiver_interface/response/errorresponse.cpp \
    receiver_interface/response/phasecontrolresponse.cpp \
    receiver_interface/response/hibitresponse.cpp \
    receiver_interface/response/pqlscontrolresponse.cpp \
    receiver_interface/response/soundretrieverresponse.cpp


HEADERS  += avrpioremote.h \
         Defs.h \
    netradiodialog.h \
    bluraydialog.h \
    aboutdialog.h \
    loudspeakersettingsdialog.h \
    tunerdialog.h \
    actionwithparameter.h \
    playerinterface.h \
    testdialog.h \
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
    labeledslider.h \
    hdmicontroldialog.h \
    receiver_interface/msgdistributor.h \
    receiver_interface/response/displaydataresponse.h \
    receiver_interface/response/inputfunctionresponse.h \
    receiver_interface/response/mcaccnumberresponse.h \
    receiver_interface/response/powerresponse.h \
    receiver_interface/response/receivedobjectbase.h \
    receiver_interface/response/responselistener.h \
    receiver_interface/command/cmdbase.h \
    receiver_interface/receiverinterface.h \
    receiver_interface/response/hdmipassthroughresponse.h \
    receiver_interface/response/hdmicontrolresponse.h \
    receiver_interface/response/hdmicontrolmoderesponse.h \
    receiver_interface/response/hdmicontrolarcresponse.h \
    receiver_interface/response/volumeresponse.h \
    receiver_interface/response/muteresponse.h \
    receiver_interface/response/inputnameresponse.h \
    receiver_interface/response/errorresponse.h \
    receiver_interface/response/phasecontrolresponse.h \
    receiver_interface/response/hibitresponse.h \
    receiver_interface/response/pqlscontrolresponse.h \
    receiver_interface/response/soundretrieverresponse.h
 


FORMS    += avrpioremote.ui \
    netradiodialog.ui \
    bluraydialog.ui \
    aboutdialog.ui \
    loudspeakersettingsdialog.ui \
    tunerdialog.ui \
    testdialog.ui \
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
    labeledslider.ui \
    hdmicontroldialog.ui


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
    android/src/org/kde/necessitas/origo/QtApplication.java \
    pio-icon1.ico

RESOURCES += \
    avrpioremote.qrc

TRANSLATIONS = avrpioremote_en.ts \
               avrpioremote_de.ts \
               avrpioremote_ru.ts \
