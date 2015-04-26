#ifndef SPEAKERCONTROLREQUEST_H
#define SPEAKERCONTROLREQUEST_H

#include "receivedobjectbase.h"

class SpeakerControlRequest_SPK : public ReceivedObjectBase
{
public:
    SpeakerControlRequest_SPK();
    ~SpeakerControlRequest_SPK();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    int GetSpeakerSetup();
private:
    int     m_SpeakerSetup;
};

#endif // SPEAKERCONTROLREQUEST_H
