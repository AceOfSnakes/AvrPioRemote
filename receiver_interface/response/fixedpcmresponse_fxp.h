#ifndef FIXEDPCMRESPONSE_H
#define FIXEDPCMRESPONSE_H

#include "receivedobjectbase.h"

class FixedPCMResponse_FXP : public ReceivedObjectBase
{
public:
    FixedPCMResponse_FXP();
    ~FixedPCMResponse_FXP();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    int GetValue();

private:
    int     m_Value;
};

#endif // FIXEDPCMRESPONSE_H
