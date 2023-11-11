#ifndef HIBITRESPONSE_H
#define HIBITRESPONSE_H

#include "receivedobjectbase.h"

class HiBitResponse_ATI : public ReceivedObjectBase
{
public:
    HiBitResponse_ATI();
    ~HiBitResponse_ATI();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool    IsHiBitOn();
private:
    bool    m_HiBitOn;
};

#endif // HIBITRESPONSE_H
