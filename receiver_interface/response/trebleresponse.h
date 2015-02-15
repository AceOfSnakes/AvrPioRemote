#ifndef TREBLERESPONSE_H
#define TREBLERESPONSE_H

#include "receivedobjectbase.h"
#include "zonenumberbase.h"

class TrebleResponse : public ReceivedObjectBase, public ZoneNumberBase
{
public:
    TrebleResponse();
    ~TrebleResponse();
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
