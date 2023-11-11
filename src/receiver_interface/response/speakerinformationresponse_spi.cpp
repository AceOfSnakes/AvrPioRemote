#include "speakerinformationresponse_spi.h"

//SPI1111111080600
//SPIabcdefghhhijk
//a:Subwoofer 0:No,1:Yes/1ch,2:2ch
//b:Front 1:Small,2:Large
//c:Center 0:None,1:Small,2:Large
//d:Surround 0:None,1:Small,2:Large
//e:Surround Back 0:None,1:Small,2:Large
//f:Height 1 0:None,1:Small,2:Large
//g:Height 2 0:None,1:Small,2:Large
//hhh:Crossover 50,80,100,150,200
//i:Height 1 Position 0:No,1:FH,2:TF,3:TM,4:TR,5:RH,6:DD SP(F),7:DD SP(S),8:DD SP(B)
//j:Height 2 Position 0:No,1:FH,2:TF,3:TM,4:TR,5:RH,6:DD SP(F),7:DD SP(S),8:DD SP(B)
//k:Bi-Amp 0:No,1:F,3:F+C,5:F+S,6:C+S,7:F+C+S

SpeakerInformationResponse_SPI::SpeakerInformationResponse_SPI()
{
    subwoofer       = SUBWOOFER_NO;
    front           = SPEAKER_SETUP_NONE;
    center          = SPEAKER_SETUP_NONE;
    surround        = SPEAKER_SETUP_NONE;
    surroundBack    = SPEAKER_SETUP_NONE;
    height1         = SPEAKER_SETUP_NONE;
    height2         = SPEAKER_SETUP_NONE;
    crossover       = CROSSOVER_80HZ;
    height1Pos      = SPEAKER_HEIGHT_POSITION_NO;
    height2Pos      = SPEAKER_HEIGHT_POSITION_NO;
    biAmp           = BI_AMP_NO;
}

SpeakerInformationResponse_SPI::SpeakerInformationResponse_SPI(const SpeakerInformationResponse_SPI &toCopy)
{
    *this = toCopy;
}

SpeakerInformationResponse_SPI::~SpeakerInformationResponse_SPI()
{

}

QStringList SpeakerInformationResponse_SPI::getMsgIDs()
{
    return QStringList() << "SPI";
}

QString SpeakerInformationResponse_SPI::getResponseID()
{
    return "SpeakerInformationResponse";
}

bool SpeakerInformationResponse_SPI::parseString(QString str)
{
    if (str.startsWith("SPI"))
    {
        bool ok = false;
        subwoofer       = str.mid(3, 1).toInt();
        front           = str.mid(4, 1).toInt();
        center          = str.mid(5, 1).toInt();
        surround        = str.mid(6, 1).toInt();
        surroundBack    = str.mid(7, 1).toInt();
        height1         = str.mid(8, 1).toInt();
        height2         = str.mid(9, 1).toInt();
        crossover       = str.mid(10, 3).toInt(&ok, 10);
        height1Pos      = str.mid(13, 1).toInt();
        height2Pos      = str.mid(14, 1).toInt();
        biAmp           = str.mid(15, 1).toInt();
        return true;
    }
    return false;
}

QString SpeakerInformationResponse_SPI::getCmdString()
{
    QString cmd = QString::asprintf("SPI%d%d%d%d%d%d%d%03d%d%d%d",subwoofer,front,center,surround,surroundBack,height1,height2,crossover,height1Pos,height2Pos,biAmp);
    return cmd;
}

SpeakerInformationResponse_SPI &SpeakerInformationResponse_SPI::operator = (const SpeakerInformationResponse_SPI &toCopy)
{
    subwoofer       = toCopy.subwoofer   ;
    front           = toCopy.front       ;
    center          = toCopy.center      ;
    surround        = toCopy.surround    ;
    surroundBack    = toCopy.surroundBack;
    height1         = toCopy.height1     ;
    height2         = toCopy.height2     ;
    crossover       = toCopy.crossover   ;
    height1Pos      = toCopy.height1Pos  ;
    height2Pos      = toCopy.height2Pos  ;
    biAmp           = toCopy.biAmp       ;
    return *this;
}
