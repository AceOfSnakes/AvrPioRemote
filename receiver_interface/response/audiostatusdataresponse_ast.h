#ifndef AUDIOSTATUSDATA_H
#define AUDIOSTATUSDATA_H

#include "receivedobjectbase.h"

class AudioStatusDataResponse_AST : public ReceivedObjectBase
{
public:
    AudioStatusDataResponse_AST();
    ~AudioStatusDataResponse_AST();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    QString iChFormat;
    QString oChFormat;
    QString oSampleRate;
    QString oBitDepth;
    QString pqls;
    QString phaseControl;
    QString phaseControlReversePhase;
    QString codec, samplingRate;
    QString listeningMode;
    QString upmix;

    QString getSummary();
    QString fillPioneerChannelsString(QString onkyoChannels);
};

#endif // AUDIOSTATUSDATA_H
