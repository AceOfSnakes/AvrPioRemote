#include "audiostatusdataresponse_ast.h"
#include "Defs.h"

AudioStatusDataResponse_AST::AudioStatusDataResponse_AST()
{

}

AudioStatusDataResponse_AST::~AudioStatusDataResponse_AST()
{

}

QStringList AudioStatusDataResponse_AST::getMsgIDs()
{
    return QStringList() << "AST" << "IFA";
}

QString AudioStatusDataResponse_AST::getResponseID()
{
    return "AudioStatusDataResponse";
}

bool AudioStatusDataResponse_AST::parseString(QString str)
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
    else if (str.startsWith("IFA"))
    {
        // "IFAHDMI 4,PCM,48 kHz,2.0 ch,Extended Stereo,5.1.2 ch,48 kHz,Off,---,Normal,No,"
        // 0: a...a: Audio Input Port
        // 1: b…b: Input Signal Format
        // 2: c…c: Sampling Frequency
        // 3: d…d: Input Signal Channel
        // 4: e…e: Listening Mode
        // 5: f…f: Output Signal Channel
        // 6: g…g: Output Sampling Frequency
        // 7: h...h: PQLS (Off/2ch/Multich/Bitstream)
        // 8: i...i: Auto Phase Control Current Delay (0ms - 16ms / ---)
        // 9: j...j: Auto Phase Control Phase (Normal/Reverse)
        // 10: k...k: Upmix Mode(No/PL2/PL2X/PL2Z/DolbySurround/Neo6/NeoX/NeuralX/THXS2/ADYDSX)

        QStringList parts = str.mid(3).split(',',
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
            QString::KeepEmptyParts);
#else
            Qt::KeepEmptyParts);
#endif
        if (parts.length() >= 11)
        {
            codec = parts[1];
            samplingRate = parts[2];
            listeningMode = parts[4];
            oSampleRate = parts[6];
            pqls = parts[7];
            phaseControl = parts[9];
            phaseControlReversePhase = parts[10];
            upmix = parts[10];
            iChFormat = fillPioneerChannelsString(parts[3]);
            oChFormat = fillPioneerChannelsString(parts[5]);
            return true;
        }
    }
    return false;
}

QString AudioStatusDataResponse_AST::fillPioneerChannelsString(QString onkyoChannels)
{
    QString pioneerChannels;
    pioneerChannels.fill('0', 16);
    QStringList tmpList = onkyoChannels.split(' ',

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
                                              QString::KeepEmptyParts);
#else
                                              Qt::KeepEmptyParts);
#endif

    if (tmpList.length() == 2)
    {
        QString tmp = tmpList[0];
        QStringList channels = tmp.split('.',
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
                                         QString::KeepEmptyParts);
#else
                                         Qt::KeepEmptyParts);
#endif

        if (channels.length() >= 2)
        {
            int n = channels[0].toInt();
            if ((n & 1) == 1) // center
            {
                pioneerChannels[1] = '1';
                n--;
            }
            if (n >= 2)
            {
                pioneerChannels[0] = '1'; // L
                pioneerChannels[2] = '1'; // R
            }
            if (n >= 4)
            {
                pioneerChannels[3] = '1'; // SL
                pioneerChannels[4] = '1'; // SR
            }
            if (n >= 6)
            {
                pioneerChannels[5] = '1'; // SBL
                pioneerChannels[6] = '1'; // SBR
            }
            n = channels[1].toInt();
            if (n > 0)
            {
                pioneerChannels[8] = '1'; // LFE
            }
        }
        if (channels.length() == 3)
        {
            int n = channels[2].toInt();
            if (n >= 2)
            {
                pioneerChannels[9] = '1'; // FHL
                pioneerChannels[10] = '1'; // FHR
            }
            if (n >= 4)
            {
                pioneerChannels[11] = '1'; // FWL
                pioneerChannels[12] = '1'; // FWR
            }
        }
    }
    return pioneerChannels;
}

QString AudioStatusDataResponse_AST::getSummary()
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

    if (!listeningMode.isEmpty())
        summary += "Listening mode: " + listeningMode + "\n";

    if (!upmix.isEmpty())
        summary += "Upmix mode: " + upmix + "\n";

    return summary;
}
