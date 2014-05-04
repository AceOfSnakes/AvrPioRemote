#include "internal.h"
#include <QUrl>

#include <HUpnpCore/HUdn>
#include <HUpnpCore/HDeviceInfo>
#include <HUpnpCore/HControlPoint>
#include <HUpnpCore/HClientDevice>
#include <HUpnpCore/HClientService>
#include <HUpnpCore/HStateVariableInfo>
#include <HUpnpCore/HStateVariableEvent>
#include <HUpnpCore/HClientStateVariable>
#include <HUpnpCore/HControlPointConfiguration>

Internal::Internal()
{
    m_controlPoint = NULL;
}

Internal::~Internal()
{
    delete m_controlPoint;
    m_controlPoint = NULL;
}

bool Internal::Init()
{
    Herqq::Upnp::HControlPointConfiguration configuration;
    //configuration.setDesiredSubscriptionTimeout(30);
    // you can uncomment this ^^ to test subscription renewal with UPnP devices
    // that respect the subscription timeout requests of control points
    m_controlPoint = new Herqq::Upnp::HControlPoint(configuration, this);

    bool ok = connect(
        m_controlPoint,
        SIGNAL(rootDeviceOnline(Herqq::Upnp::HClientDevice*)),
        this,
        SLOT(rootDeviceOnline(Herqq::Upnp::HClientDevice*)));

    if (!ok)
    {
        delete m_controlPoint;
        m_controlPoint = NULL;
        return false;
    }
    //Q_ASSERT(ok);

//    ok = connect(
//        m_controlPoint,
//        SIGNAL(rootDeviceOffline(Herqq::Upnp::HClientDevice*)),
//        this,
//        SLOT(rootDeviceOffline(Herqq::Upnp::HClientDevice*)));

//    Q_ASSERT(ok);
    if (!m_controlPoint->init())
    {
        delete m_controlPoint;
        m_controlPoint = NULL;
    }
    return true;
}

void Internal::stateVariableChanged(
    const Herqq::Upnp::HClientStateVariable* /*source*/, const Herqq::Upnp::HStateVariableEvent& /*event*/)
{
   // m_ui->status->append(QString(
   //     "State variable [%1] changed value from [%2] to [%3]").arg(
   //         source->info().name(), event.previousValue().toString(),
   //         event.newValue().toString()));
}

void Internal::rootDeviceOnline(Herqq::Upnp::HClientDevice* newDevice)
{
    //m_controlpointNavigator->rootDeviceOnline(newDevice);
    //connectToEvents(newDevice);
    //qDebug() << "rootDeviceOnline";
    //emit UpdateLabel("rootDeviceOnline" + newDevice->info().friendlyName());
    QString tmp;
    foreach(QUrl url, newDevice->locations())
    {
        tmp += url.toString() + " ";
    }

    EmitNewDevice(newDevice->info().friendlyName(), tmp);
}

void Internal::rootDeviceOffline(Herqq::Upnp::HClientDevice* /*device*/)
{
    //m_controlpointNavigator->rootDeviceOffline(device);
    //m_dataItemDisplay->deviceRemoved(device->info().udn());
    //emit contentSourceRemoved(device);
    //qDebug() << "rootDeviceOffline";
    //emit UpdateLabel("rootDeviceOffline");
}

extern "C" {

HUpnpWrapper* GetInstance()
{
    return new Internal();
}

void FreeInstance(HUpnpWrapper* instance)
{
    delete instance;
}
}
