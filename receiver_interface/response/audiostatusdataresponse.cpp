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
                oSampleRate = ast.mid(43, 2);
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
            return true;
        }
    }
    return false;
}

QString AudioStatusDataResponse::getSummary()
{
    QString summary;

    summary += "Codec: " + codec + "\n";
    summary += "Sample rate: " + samplingRate + "\n";
    //if (!iChFormat.isEmpty())
    //    summary += "Input channel format: " + iChFormat + "\n";
    //if (!oChFormat.isEmpty())
    //    summary += "Output channel format: " + oChFormat + "\n";
    if (!oSampleRate.isEmpty())
        summary += "Output sample rate: " + oSampleRate + "\n";
    if (!oBitDepth.isEmpty())
        summary += "Output bit depth: " + oBitDepth + "\n";

    if (pqls == "0")
        summary += "PQLS: OFF\n";
    else if (pqls == "1")
        summary += "PQLS: 2ch\n";
    else if (pqls == "2")
        summary += "PQLS: Multi ch\n";
    else if (pqls == "3")
        summary += "PQLS: Bitstream\n";

    if (!phaseControl.isEmpty())
        summary += "Auto Phase Control Plus: " + phaseControl + " ms\n";
    if (phaseControlReversePhase == "0")
        summary += "Auto Phase Control Plus (No reverse phase)\n";
    else if (phaseControlReversePhase == "1")
        summary += "Auto Phase Control Plus (Reverse phase)\n";

    return summary;
}
