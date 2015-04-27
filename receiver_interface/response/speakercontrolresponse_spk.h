#ifndef SPEAKERCONTROLREQUEST_H
#define SPEAKERCONTROLREQUEST_H

#include "receivedobjectbase.h"

class SpeakerControlResponse_SPK : public ReceivedObjectBase
{
public:
    SpeakerControlResponse_SPK();
    ~SpeakerControlResponse_SPK();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    int GetSpeakerSetup();
private:
    int     m_SpeakerSetup;
};

#endif // SPEAKERCONTROLREQUEST_H
