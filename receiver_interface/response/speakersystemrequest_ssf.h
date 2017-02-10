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

    QString GetSpeakerSystem();
private:
    QString m_SpeakerSystem;
};

#endif // SPEAKERSYSTEMREQUEST_H
