#ifndef POWERRESPONSE_H
#define POWERRESPONSE_H

#include "receivedobjectbase.h"
#include "zonenumberbase.h"

class PowerResponse_PWR_APR_BPR_ZEP : public ReceivedObjectBase, public ZoneNumberBase
{
public:
    PowerResponse_PWR_APR_BPR_ZEP();
    ~PowerResponse_PWR_APR_BPR_ZEP();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool IsPoweredOn();

private:
    bool    m_PoweredOn;
};

#endif // POWERRESPONSE_H
