#ifndef TONERESPONSE_H
#define TONERESPONSE_H

#include "receivedobjectbase.h"
#include "zonenumberbase.h"

class ToneResponse_TO_ZGA : public ReceivedObjectBase, public ZoneNumberBase
{
public:
    ToneResponse_TO_ZGA();
    ~ToneResponse_TO_ZGA();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool IsToneOn();

private:
    bool m_ToneOn;
};

#endif // TONERESPONSE_H
