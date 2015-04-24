#ifndef SPEAKERSETTINGRESPONSE_H
#define SPEAKERSETTINGRESPONSE_H

#include "receivedobjectbase.h"

class SpeakerSettingResponse : public ReceivedObjectBase
{
public:
    SpeakerSettingResponse();
    ~SpeakerSettingResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    int getSpeakerNo();
    int getSpeakerSetting();
private:
    int     m_Speaker;
    int     m_Setting;
};

#endif // SPEAKERSETTINGRESPONSE_H
