#include "audiostatusdataresponse.h"
#include "Defs.h"

AudioStatusDataResponse::AudioStatusDataResponse()
{

}

AudioStatusDataResponse::~AudioStatusDataResponse()
{

}

QStringList AudioStatusDataResponse::getMsgIDs()
{
    return QStringList() << "AST";
}

QString AudioStatusDataResponse::getResponseID()
{
    return "AudioStatusDataResponse";
}

bool AudioStatusDataResponse::parseString(QString str)
{
    if (str.startsWith("AST"))
    {
        //AST030100000000000000001010000010000
        QString ast = str.mid(3);
        if (ast.length() >= 33)
        {
            QString codec, samplingRate;
            //int n = 0;

            // codec
            codec = FindValueByKey(AST1, ast.mid(0, 2));
            if (codec == "")
                codec = "???";

            // sampling rate
            samplingRate = FindValueByKey(AST3, ast.mid(2, 2));
            if (samplingRate == "")
                samplingRate = "???";

            if (ast.length() >= 55) {
                // input channel format info
                iChFormat = ast.mid(4, 21);
                // output channel
                oChFormat = ast.mid(25, 18);
                // output sample rate
                oSamleRate = ast.mid(43, 2);
                // output bit depth
                oBitDepth = ast.mid(45, 2);
                // 47-50 reserved
                // working pqls
                pqls = ast.mid(51, 1);
                // auto phase control plus
                phaseControl = ast.mid(52, 2);
                // auto phase control plus reverse phase
                phaseControlReversePhase = ast.mid(54, 1);

//                qDebug() << "Output sample rate" << oSamleRate;
//                qDebug() << "Output bit depth" << oBitDepth;
//                qDebug() << "PQLS" << pqls;
//                qDebug() << "Phase Control Plus" << phaseControl;
//                qDebug() << "Phase Control Plus (Reverse Phase)" << phaseControlReversePhase;
            } else if (ast.length() >= 43) {
                // input channel format info
                iChFormat = ast.mid(4, 21);
                // output channel
                oChFormat = ast.mid(25, 18);
            } else {
                // input channel format info
                iChFormat = ast.mid(4, 16);
                // output channel
                oChFormat = ast.mid(20, 13);
            }
//            qDebug() << "Input  channel format" << iChFormat;
//            qDebug() << "Output channel format" << oChFormat;
            //emit AudioStatusData(codec, samplingRate);
            return true;
        }
    }
    return false;
}
