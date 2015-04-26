#ifndef SPEAKERSYSTEMREQUEST_H
#define SPEAKERSYSTEMREQUEST_H

#include "receivedobjectbase.h"

class SpeakerSystemRequest_SSF : public ReceivedObjectBase
{
public:
    SpeakerSystemRequest_SSF();
    ~SpeakerSystemRequest_SSF();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    int GetSpeakerSystem();
private:
    int     m_SpeakerSystem;
};

#endif // SPEAKERSYSTEMREQUEST_H
