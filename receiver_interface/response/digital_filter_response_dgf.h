#ifndef DIGITALFILTER_H
#define DIGITALFILTER_H

#include "receivedobjectbase.h"

class DigitalFilterResponse_DGF : public ReceivedObjectBase
{
public:
    DigitalFilterResponse_DGF();
    ~DigitalFilterResponse_DGF();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    int GetValue();

private:
    int     m_Value;
};

#endif // DIGITALFILTER_H
