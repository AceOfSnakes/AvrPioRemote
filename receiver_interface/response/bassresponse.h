#ifndef BASSRESPONSE_H
#define BASSRESPONSE_H

#include "receivedobjectbase.h"
#include "zonenumberbase.h"

class BassResponse : public ReceivedObjectBase, public ZoneNumberBase
{
public:
    BassResponse();
    ~BassResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    int GetValue();
    float GetdBValue();
    QString GetAsString();

private:
    int     m_Value;
};

#endif // BASSRESPONSE_H
