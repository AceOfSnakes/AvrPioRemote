#ifndef TREBLERESPONSE_H
#define TREBLERESPONSE_H

#include "receivedobjectbase.h"
#include "zonenumberbase.h"

class TrebleResponse_TR_ZGG : public ReceivedObjectBase, public ZoneNumberBase
{
public:
    TrebleResponse_TR_ZGG();
    ~TrebleResponse_TR_ZGG();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    int GetValue();
    float GetdBValue();
    QString GetAsString();

private:
    int     m_Value;
};

#endif // TREBLERESPONSE_H
