#ifndef TONERESPONSE_H
#define TONERESPONSE_H

#include "receivedobjectbase.h"
#include "zonenumberbase.h"

class ToneResponse : public ReceivedObjectBase, public ZoneNumberBase
{
public:
    ToneResponse();
    ~ToneResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool IsToneOn();

private:
    bool m_ToneOn;
};

#endif // TONERESPONSE_H
