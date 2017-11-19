#ifndef PIONEERRECEIVER_H
#define PIONEERRECEIVER_H

#include <QObject>
#include "discoverydevice.h"

class PioneerReceiver : public DiscoveryDevice
{
public:
    PioneerReceiver();
private:
    QString         m_ReceivedString;
    virtual void NewDataToRead();
};

#endif // PIONEERRECEIVER_H
