#ifndef UPSAMPLING_H
#define UPSAMPLING_H

#include "receivedobjectbase.h"

class UpsamplinResponse_UPS : public ReceivedObjectBase
{
public:
    UpsamplinResponse_UPS();
    ~UpsamplinResponse_UPS();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    int GetValue();

private:
    int     m_Value;
};

#endif // UPSAMPLING_H
