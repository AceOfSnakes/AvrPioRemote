#include "hupnpwrapper.h"


HUpnpWrapper::HUpnpWrapper()
{
}

void HUpnpWrapper::EmitNewDevice(QString name, QString url)
{
    emit NewDevice(name, url);
}
