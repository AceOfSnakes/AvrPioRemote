#ifndef MUTERESPONSE_H
#define MUTERESPONSE_H

#include "receivedobjectbase.h"
#include "zonenumberbase.h"

class MuteResponse_MUT_Z2MUT_Z3MUT : public ReceivedObjectBase, public ZoneNumberBase
{
public:
    MuteResponse_MUT_Z2MUT_Z3MUT();
    ~MuteResponse_MUT_Z2MUT_Z3MUT();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool IsMuted();

private:
    bool    m_Muted;
};

#endif // MUTERESPONSE_H
