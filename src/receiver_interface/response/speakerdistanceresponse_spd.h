#ifndef SPEAKERDISTANCESPDRESPONSE_H
#define SPEAKERDISTANCESPDRESPONSE_H

#include "receivedobjectbase.h"

class SpeakerDistanceResponse_SPD : public ReceivedObjectBase
{
public:
    SpeakerDistanceResponse_SPD();
    ~SpeakerDistanceResponse_SPD();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    int getSpeakerNo();
    int getSpeakerSetting();
private:
    int     m_Speaker;
    int     m_Setting;
};

#endif // SPEAKERDISTANCESPDRESPONSE_H
