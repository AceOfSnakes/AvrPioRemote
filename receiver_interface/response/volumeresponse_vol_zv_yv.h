#ifndef VOLUMERESPONSE_H
#define VOLUMERESPONSE_H

#include "receivedobjectbase.h"
#include "zonenumberbase.h"

class VolumeResponse_VOL_ZV_YV : public ReceivedObjectBase, public ZoneNumberBase
{
public:
    VolumeResponse_VOL_ZV_YV();
    ~VolumeResponse_VOL_ZV_YV();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    int GetValue();
    float GetdBValue();
    QString GetAsString();

private:
    int     m_Volume;
};

#endif // VOLUMERESPONSE_H
