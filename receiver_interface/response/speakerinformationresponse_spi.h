#ifndef SPEAKERINFORMATIONRESPONSE_H
#define SPEAKERINFORMATIONRESPONSE_H

#include "receivedobjectbase.h"

class SpeakerInformationResponse_SPI : public ReceivedObjectBase
{
public:
    SpeakerInformationResponse_SPI();
    ~SpeakerInformationResponse_SPI();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    enum SUBWOOFER {
        SUBWOOFER_NO = 0,
        SUBWOOFER_1CH = 1,
        SUBWOOFER_2CH = 2,
    };
    enum SPEAKER_SETUP {
        SPEAKER_SETUP_NONE = 0,
        SPEAKER_SETUP_SMALL = 1,
        SPEAKER_SETUP_LARGE = 2
    };
    enum CROSSOVER {
        CROSSOVER_50HZ,
        CROSSOVER_80HZ,
        CROSSOVER_100HZ,
        CROSSOVER_150HZ,
        CROSSOVER_200HZ
    };
    enum SPEAKER_HEIGHT_POSITION {
        SPEAKER_HEIGHT_POSITION_NO = 0,
        SPEAKER_HEIGHT_POSITION_FH = 1,
        SPEAKER_HEIGHT_POSITION_TF = 2,
        SPEAKER_HEIGHT_POSITION_TM = 3,
        SPEAKER_HEIGHT_POSITION_TR = 4,
        SPEAKER_HEIGHT_POSITION_RH = 5,
        SPEAKER_HEIGHT_POSITION_DD_F = 6,
        SPEAKER_HEIGHT_POSITION_DD_S = 7,
        SPEAKER_HEIGHT_POSITION_SP_B = 8
    };
    enum BI_AMP {
        BI_AMP_NO = 0,
        BI_AMP_FRONT = 1,
        BI_AMP_FRONT_CENTER = 3,
        BI_AMP_FRONT_SURROUND = 5,
        BI_AMP_CENTER_SURROUND = 6,
        BI_AMP_FRONT_CENTER_SURROUND = 7,
    };
    int subwoofer;
    int front;
    int center;
    int surround;
    int surroundBack;
    int height1;
    int height2;
    int crossover;
    int height1Pos;
    int height2Pos;
    int biAmp;
private:
};

#endif // SPEAKERINFORMATIONRESPONSE_H
