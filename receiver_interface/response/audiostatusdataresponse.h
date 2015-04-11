#ifndef AUDIOSTATUSDATA_H
#define AUDIOSTATUSDATA_H

#include "receivedobjectbase.h"

class AudioStatusDataResponse : public ReceivedObjectBase
{
public:
    AudioStatusDataResponse();
    ~AudioStatusDataResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    QString iChFormat;
    QString oChFormat;
    QString oSamleRate;
    QString oBitDepth;
    QString pqls;
    QString phaseControl;
    QString phaseControlReversePhase;
};

#endif // AUDIOSTATUSDATA_H
