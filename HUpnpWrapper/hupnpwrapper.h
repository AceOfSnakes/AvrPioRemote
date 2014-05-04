#ifndef HUPNPWRAPPER_H
#define HUPNPWRAPPER_H

#include <QObject>
#include <QtCore/qglobal.h>

#if defined(HUPNPWRAPPER_LIBRARY)
#  define HUPNPWRAPPERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define HUPNPWRAPPERSHARED_EXPORT Q_DECL_IMPORT
#endif

class HUPNPWRAPPERSHARED_EXPORT HUpnpWrapper : public QObject
{
    Q_OBJECT
public:
    HUpnpWrapper();
    virtual ~HUpnpWrapper(){}
    virtual bool Init() = 0;
protected:
    void EmitNewDevice(QString name, QString url);

signals:
    void NewDevice(QString name, QString url);
};

extern "C" {
    HUpnpWrapper* HUPNPWRAPPERSHARED_EXPORT GetInstance();
    void HUPNPWRAPPERSHARED_EXPORT FreeInstance(HUpnpWrapper*instance);
}

#endif // HUPNPWRAPPER_H
