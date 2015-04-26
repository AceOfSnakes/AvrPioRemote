#ifndef BASSRESPONSE_H
#define BASSRESPONSE_H

#include "receivedobjectbase.h"
#include "zonenumberbase.h"

class BassResponse_BA_ZGB : public ReceivedObjectBase, public ZoneNumberBase
{
public:
    BassResponse_BA_ZGB();
    ~BassResponse_BA_ZGB();
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
