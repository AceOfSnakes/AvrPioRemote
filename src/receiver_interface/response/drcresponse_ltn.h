#ifndef DRCRESPONSE_H
#define DRCRESPONSE_H

#include "receivedobjectbase.h"

class DRCResponse_LTN : public ReceivedObjectBase
{
public:
    DRCResponse_LTN();
    ~DRCResponse_LTN();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    int GetValue();

private:
    int     m_Value;
};

#endif // DRCRESPONSE_H
