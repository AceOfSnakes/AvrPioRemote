#ifndef VIDEOSTATUSDATARESPONSE_H
#define VIDEOSTATUSDATARESPONSE_H

#include "receivedobjectbase.h"

class VideoStatusDataResponse_VST : public ReceivedObjectBase
{
public:
    VideoStatusDataResponse_VST();
    ~VideoStatusDataResponse_VST();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    QString getSummary();

    QString videoInput;
    QString inputResolution;
    QString inputAspectRatio;
    QString inputColorFormat;
    QString inputBits;
    QString inputColorSpace;
    QString outputResolution;
    QString outputAspect;
    QString outputColorFormat;
    QString outputBits;
    QString outputColorSpace;
    QString out1RecommendedResolution;
    QString out1ColorDepth;
    QString out1SupportedColorSpace;
    QString out2RecommendedResolution;
    QString out2ColorDepth;
    QString out2SupportedColorSpace;
    QString out3RecommendedResolution;
    QString out3ColorDepth;
    QString out3SupportedColorSpace;
    QString input3DFormat;
    QString output3DFormat;
    QString out4RecommendedResolution;
    QString out4ColorDepth;
    QString out4SupportedColorSpace;
    QString pictureMode;

private:
    QString getSupportedColorSpace(QString str);

};

#endif // VIDEOSTATUSDATARESPONSE_H

