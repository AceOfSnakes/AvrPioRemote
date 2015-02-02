#ifndef SOUNDRETRIEVERRESPONSE_H
#define SOUNDRETRIEVERRESPONSE_H

#include "receivedobjectbase.h"

class SoundRetrieverResponse : public ReceivedObjectBase
{
public:
    SoundRetrieverResponse();
    ~SoundRetrieverResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool    IsSoundRetrieverOn();
private:
    bool    m_SoundRetrieverOn;
};

#endif // SOUNDRETRIEVERRESPONSE_H
