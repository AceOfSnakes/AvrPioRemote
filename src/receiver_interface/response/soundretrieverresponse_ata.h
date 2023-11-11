#ifndef SOUNDRETRIEVERRESPONSE_H
#define SOUNDRETRIEVERRESPONSE_H

#include "receivedobjectbase.h"

class SoundRetrieverResponse_ATA : public ReceivedObjectBase
{
public:
    SoundRetrieverResponse_ATA();
    ~SoundRetrieverResponse_ATA();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool    IsSoundRetrieverOn();
private:
    bool    m_SoundRetrieverOn;
};

#endif // SOUNDRETRIEVERRESPONSE_H
