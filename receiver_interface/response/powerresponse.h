#ifndef POWERRESPONSE_H
#define POWERRESPONSE_H

#include "receivedobjectbase.h"
#include "zonenumberbase.h"

class PowerResponse : public ReceivedObjectBase, public ZoneNumberBase
{
public:
    PowerResponse();
    ~PowerResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool IsPoweredOn();

private:
    bool    m_PoweredOn;
};

#endif // POWERRESPONSE_H
