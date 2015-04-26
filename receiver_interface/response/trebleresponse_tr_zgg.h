#ifndef TREBLERESPONSE_H
#define TREBLERESPONSE_H

#include "receivedobjectbase.h"
#include "zonenumberbase.h"

class TR_ZGG_TrebleResponse : public ReceivedObjectBase, public ZoneNumberBase
{
public:
    TR_ZGG_TrebleResponse();
    ~TR_ZGG_TrebleResponse();
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
