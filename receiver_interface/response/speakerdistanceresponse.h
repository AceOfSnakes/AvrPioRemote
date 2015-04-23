#ifndef SPEAKERDISTANCERESPONSE_H
#define SPEAKERDISTANCERESPONSE_H

#include "receivedobjectbase.h"

class SpeakerDistanceResponse : public ReceivedObjectBase
{
public:
    SpeakerDistanceResponse();
    ~SpeakerDistanceResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    int GetMCACCNo();
    QString GetSpeakerId();
    double GetValue();
    int GetRawValue();
    enum UNITS {
        FEET = 0,
        METER = 1,
    };
    UNITS getRawUnits();
    QString getUnits();

private:
    int         m_MCACCNo;
    QString     m_Speaker;
    UNITS       m_Units;
    int         m_Distance;
};

#endif // SPEAKERDISTANCERESPONSE_H
