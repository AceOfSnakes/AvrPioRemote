#ifndef INTERNAL_H
#define INTERNAL_H

#include <QObject>
#include "hupnpwrapper.h"
#include <HUpnpCore/HUpnp>

class Internal : public HUpnpWrapper
{
    Q_OBJECT
public:
    explicit Internal();
    virtual ~Internal();
    virtual bool Init();

public slots:
    void rootDeviceOnline(Herqq::Upnp::HClientDevice*);
    void rootDeviceOffline(Herqq::Upnp::HClientDevice*);
    void stateVariableChanged(
        const Herqq::Upnp::HClientStateVariable*,
        const Herqq::Upnp::HStateVariableEvent&);

private:
    Herqq::Upnp::HControlPoint* m_controlPoint;

};

#endif // INTERNAL_H
