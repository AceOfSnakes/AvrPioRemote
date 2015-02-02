#ifndef MUTERESPONSE_H
#define MUTERESPONSE_H

#include "receivedobjectbase.h"

class MuteResponse : public ReceivedObjectBase
{
public:
    MuteResponse();
    ~MuteResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool IsMuted();
    Zone GetZone();

private:
    bool    m_Muted;
    Zone    m_Zone;
};

#endif // MUTERESPONSE_H
