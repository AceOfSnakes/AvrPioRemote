#-------------------------------------------------
#
# Project created by QtCreator 2014-05-02T13:30:21
#
#-------------------------------------------------

QT       -= gui

TARGET = HUpnpWrapper
TEMPLATE = lib

DEFINES += HUPNPWRAPPER_LIBRARY

SOURCES += hupnpwrapper.cpp \
    internal.cpp

HEADERS += hupnpwrapper.h\
    internal.h

INCLUDEPATH += $$PWD/hupnp/include

LIBS += -L"$$PWD/hupnp/bin" -lHUpnp \
        -L"$$PWD/hupnp/lib/qtsoap-2.7-opensource/lib"

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DEPENDPATH += $$PWD/hupnp/include
