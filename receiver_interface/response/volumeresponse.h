#ifndef VOLUMERESPONSE_H
#define VOLUMERESPONSE_H

#include "receivedobjectbase.h"

class VolumeResponse : public ReceivedObjectBase
{
public:
    VolumeResponse();
    ~VolumeResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    int GetVolumeId();
    float GetdBVolume();
    QString GetVolumeString();
    Zone GetZone();

private:
    int     m_Volume;
    Zone    m_Zone;
};

#endif // VOLUMERESPONSE_H
