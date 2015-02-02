#ifndef HIBITRESPONSE_H
#define HIBITRESPONSE_H

#include "receivedobjectbase.h"

class HiBitResponse : public ReceivedObjectBase
{
public:
    HiBitResponse();
    ~HiBitResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool    IsHiBitOn();
private:
    bool    m_HiBitOn;
};

#endif // HIBITRESPONSE_H
