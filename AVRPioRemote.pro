#-------------------------------------------------
#
# Project created by QtCreator 2013-04-19T23:58:49
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AVRPioRemote
TEMPLATE = app


SOURCES += main.cpp \
        avrpioremote.cpp \
        Defs.cpp \
    netradiodialog.cpp \
    aboutdialog.cpp \
    loudspeakersettingsdialog.cpp \
    tunerdialog.cpp \
    actionwithparameter.cpp

HEADERS  += avrpioremote.h \
         Defs.h \
    netradiodialog.h \
    aboutdialog.h \
    loudspeakersettingsdialog.h \
    tunerdialog.h \
    actionwithparameter.h

FORMS    += avrpioremote.ui \
    netradiodialog.ui \
    aboutdialog.ui \
    loudspeakersettingsdialog.ui \
    tunerdialog.ui

OTHER_FILES += \
    images/cancel.png \
    images/pen.png \
    images/ok.png
