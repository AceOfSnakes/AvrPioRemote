#ifndef ONKYORECEIVER_H
#define ONKYORECEIVER_H

#include <QObject>
#include "discoverydevice.h"

#pragma pack(push)
#pragma pack(1)
struct eISCPHeader
{
    char magic[4];
    uint32_t headerSize;
    uint32_t dataSize;
    char version;
    char reserved1;
    char reserved2;
    char reserved3;

    eISCPHeader();
};
#pragma pack(pop)

class OnkyoReceiver : public DiscoveryDevice
{
public:
    OnkyoReceiver();
    virtual int write(QString str);
    virtual QString read();
    void setIsReceiver(bool isReceiver);
private:
    bool isReceiver;
};

#endif // ONKYORECEIVER_H
