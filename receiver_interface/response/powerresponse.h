#ifndef POWERRESPONSE_H
#define POWERRESPONSE_H

#include "receivedobjectbase.h"

class PowerResponse : public ReceivedObjectBase
{
public:
    PowerResponse();
    ~PowerResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool IsPoweredOn();
    Zone GetZone();

private:
    bool    m_PoweredOn;
    Zone    m_Zone;
};

#endif // POWERRESPONSE_H
