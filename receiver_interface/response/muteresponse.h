#ifndef MUTERESPONSE_H
#define MUTERESPONSE_H

#include "receivedobjectbase.h"
#include "zonenumberbase.h"

class MuteResponse : public ReceivedObjectBase, public ZoneNumberBase
{
public:
    MuteResponse();
    ~MuteResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool IsMuted();

private:
    bool    m_Muted;
};

#endif // MUTERESPONSE_H
