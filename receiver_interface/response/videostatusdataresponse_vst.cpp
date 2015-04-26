#include "videostatusdataresponse_vst.h"

#include "Defs.h"
#include <QDebug>

const STRING_PAIR VIDEO_INPUT[] = {
    {"00", "PHONO"},
    {"01", "CD"},
    {"02", "TUNER"},
    {"03", "CD-R / TAPE"},
    {"04", "DVD"},
    {"05", "TV / SAT"},
    {"06", "SAT/CBL"},
    {"10", "VIDEO 1"},//(VIDEO)",
    {"12", "MULTI CH IN"},
    {"13", "USB DAC"},
    {"14", "VIDEO 2"},
    {"15", "DVR / BDR"},
    {"17", "iPod / USB"},
    {"18", "XM RADIO"},
    {"19", "HDMI 1"},
    {"20", "HDMI 2"},
    {"21", "HDMI 3"},
    {"22", "HDMI 4"},
    {"23", "HDMI 5"},
    {"24", "HDMI 6"},
    {"25", "BD"},
    {"26", "HMG (Internet Radio)"}, //HOME MEDIA GALLERY
    {"27", "SIRIUS"},
    {"31", "HDMI (cyclic)"},
    {"33", "ADAPTER PORT"},
    {"34", "HDMI 7"},
    {"38", "INTERNET RADIO"},
    {"40", "SiriusXM"},
    {"41", "PANDORA"},
    {"44", "MEDIA SERVER"},
    {"45", "FAVORITES"},
    {"48", "MHL"},
    {"", ""}
};

const char* VIDEO_INPUT_TERMINAL[] = {
    /* 00 */ "---",
    /* 01 */ "VIDEO",
    /* 02 */ "S-VIDEO",
    /* 03 */ "COMPONENT",
    /* 04 */ "HDMI",
    /* 05 */ "Self OSD/JPEG",
};

const char* VIDEO_INPUT_RESOLUTION[] = {
    /* 00 */ "",
    /* 01 */ "480/60i",
    /* 02 */ "576/50i",
    /* 03 */ "480/60p",
    /* 04 */ "576/50p",
    /* 05 */ "720/60p",
    /* 06 */ "720/50p",
    /* 07 */ "1080/60i",
    /* 08 */ "1080/50i",
    /* 09 */ "1080/60p",
    /* 10 */ "1080/50p",
    /* 11 */ "1080/24p",
    /* 12 */ "4Kx2K/24Hz",
    /* 13 */ "4Kx2K/25Hz",
    /* 14 */ "4Kx2K/30Hz",
    /* 15 */ "4Kx2K/24Hz(SMPTE)",

};

const char* VIDEO_INPUT_ASPECT[] = {
    /* 00 */ "",
    /* 01 */ "4:3",
    /* 02 */ "16:9",
    /* 03 */ "14:9",
};

const char* VIDEO_INPUT_COLOR_FORMAT[] = {
    /* 00 */ "",
    /* 01 */ "RGB Limit",
    /* 02 */ "RGB Full",
    /* 03 */ "YcbCr444",
    /* 04 */ "YcbCr422",
};

const char* VIDEO_INPUT_BITS[] = {
    /* 00 */ "",
    /* 01 */ "24bit (8bit*3)",
    /* 02 */ "30bit (10bit*3)",
    /* 03 */ "36bit (12bit*3)",
    /* 04 */ "48bit (16bit*3)",
};

const char* VIDEO_INPUT_COLOR_SPACE[] = {
    /* 00 */ "",
    /* 01 */ "Standard",
    /* 02 */ "xvYCC601",
    /* 03 */ "xvYCC709",
    /* 04 */ "sYCC",
    /* 05 */ "AdobeYCC601",
    /* 06 */ "AdobeRGB",
};

const char* SUPPORTED_COLOR_SPACE[] = {
    /* 00 */ "xvYCC601",
    /* 01 */ "xvYCC709",
    /* 02 */ "sYCC",
    /* 03 */ "AdobeYCC601",
    /* 04 */ "AdobeRGB",
};

const char* VIDEO_3D_FORMAT[] = {
   /* 00 */ "",
   /* 01 */ "Frame packing",
   /* 02 */ "Field alternative",
   /* 03 */ "Line alternative",
   /* 04 */ "Side-by-Side(Full)",
   /* 05 */ "L + depth",
   /* 06 */ "L + depth + graphics",
   /* 07 */ "Top-and-Bottom",
   /* 08 */ "Side-by-Side(Half)",
};


VideoStatusDataResponse_VST::VideoStatusDataResponse_VST()
{

}

VideoStatusDataResponse_VST::~VideoStatusDataResponse_VST()
{

}

QStringList VideoStatusDataResponse_VST::getMsgIDs()
{
    return QStringList() << "VST";
}

QString VideoStatusDataResponse_VST::getResponseID()
{
    return "VideoStatusDataResponse";
}

bool VideoStatusDataResponse_VST::parseString(QString str)
{
    if (str.startsWith("VST"))
    {
        QString vst = str.mid(3);
        //qDebug() << vst << vst.length();

        int n = 0;

        if (vst.length() >= 28)
        {
            // Terminal/Video Input
            n = vst[0].toLatin1() - '0';
            videoInput = (n >= 0 && n <= 5) ? (VIDEO_INPUT_TERMINAL[n]) : "---";

            // Input Resolution
            n = vst.mid(1, 2).toInt();
            inputResolution = (n >= 0 && n <= 15) ? (VIDEO_INPUT_RESOLUTION[n]) : "---";

            // Aspect Ratio
            n = vst[3].toLatin1() - '0';
            inputAspectRatio = (n >= 0 && n <= 3) ? (VIDEO_INPUT_ASPECT[n]) : "---";

            // Color Format (HDMI only)
            n = vst[4].toLatin1() - '0';
            inputColorFormat = (n >= 0 && n <= 4) ? (VIDEO_INPUT_COLOR_FORMAT[n]) : "---";

            // Bits (HDMI only)
            n = vst[5].toLatin1() - '0';
            inputBits = (n >= 0 && n <= 4) ? (VIDEO_INPUT_BITS[n]) : "---";

            // Color Space (HDMI only)
            n = vst[6].toLatin1() - '0';
            inputColorSpace = (n >= 0 && n <= 6) ? (VIDEO_INPUT_COLOR_SPACE[n]) : "---";

            // Output Resolution
            n = vst.mid(7, 2).toInt();
            outputResolution = (n >= 0 && n <= 15) ? (VIDEO_INPUT_RESOLUTION[n]) : "---";

            // Output Aspect
            n = vst[9].toLatin1() - '0';
            outputAspect = (n >= 0 && n <= 3) ? (VIDEO_INPUT_ASPECT[n]) : "---";

            // Output Color Format (HDMI only)
            n = vst[10].toLatin1() - '0';
            outputColorFormat = (n >= 0 && n <= 4) ? (VIDEO_INPUT_COLOR_FORMAT[n]) : "---";

            // Output Bits (HDMI only)
            n = vst[11].toLatin1() - '0';
            outputBits = (n >= 0 && n <= 4) ? (VIDEO_INPUT_BITS[n]) : "---";

            // Output Color Space (HDMI only)
            n = vst[12].toLatin1() - '0';
            outputColorSpace = (n >= 0 && n <= 6) ? (VIDEO_INPUT_COLOR_SPACE[n]) : "---";

            // HDMI out 1 Recommended Resolution
            n = vst.mid(13, 2).toInt();
            out1RecommendedResolution = (n >= 0 && n <= 15) ? (VIDEO_INPUT_RESOLUTION[n]) : "---";

            // HDMI out 1 Deep Color
            n = vst[15].toLatin1() - '0';
            out1ColorDepth = (n >= 0 && n <= 4) ? (VIDEO_INPUT_BITS[n]) : "---";

            // HDMI out 1 Color Space data17-21
            out1SupportedColorSpace = vst.mid(16, 5);

            // HDMI out 2 Recommended Resolution
            n = vst.mid(21, 2).toInt();
            out2RecommendedResolution = (n >= 0 && n <= 15) ? (VIDEO_INPUT_RESOLUTION[n]) : "---";

            // HDMI out 2 Deep Color
            n = vst[23].toLatin1() - '0';
            out2ColorDepth = (n >= 0 && n <= 4) ? (VIDEO_INPUT_BITS[n]) : "---";

            // HDMI out 2 Color Space
            out2SupportedColorSpace = vst.mid(24, 5);
        }
        if (vst.length() >= 41)
        {
            // HDMI out 3 Recommended Resolution
            n = vst.mid(29, 2).toInt();
            out3RecommendedResolution = (n >= 0 && n <= 15) ? (VIDEO_INPUT_RESOLUTION[n]) : "";

            // HDMI out 3 Deep Color
            n = vst[31].toLatin1() - '0';
            out3ColorDepth = (n >= 0 && n <= 4) ? (VIDEO_INPUT_BITS[n]) : "";

            // HDMI out 3 Color Space
            out3SupportedColorSpace = vst.mid(32, 5);

            // HDMI 3D Input format
            n = vst.mid(37, 2).toInt();
            input3DFormat = (n >= 0 && n <= 8) ? (VIDEO_3D_FORMAT[n]) : "";

            // HDMI 3D Output format
            n = vst.mid(39, 2).toInt();
            output3DFormat = (n >= 0 && n <= 8) ? (VIDEO_3D_FORMAT[n]) : "";
        }
        if (vst.length() >= 49)
        {
            // HDMI out 4 Recommended Resolution
            n = vst.mid(41, 2).toInt();
            out4RecommendedResolution = (n >= 0 && n <= 15) ? (VIDEO_INPUT_RESOLUTION[n]) : "";

            // HDMI out 4 Deep Color
            n = vst[43].toLatin1() - '0';
            out4ColorDepth = (n >= 0 && n <= 4) ? (VIDEO_INPUT_BITS[n]) : "";

            // HDMI out 4 Color Space
            out4SupportedColorSpace = vst.mid(44, 5);
        }
        return true;
    }
    return false;
}

QString VideoStatusDataResponse_VST::getSummary()
{
    QString summary, str;

    summary += "Video Input: " + videoInput;
    summary += "\nInput Resolution: " + inputResolution;
    summary += "\nInput Aspect Ratio: " + inputAspectRatio;
    summary += "\nInput Color Format: " + inputColorFormat;
    summary += "\nInput Bit Format: " + inputBits;
    summary += "\nInput Color Space: " + inputColorSpace;
    summary += "\nOutput Resolution: " + outputResolution;
    summary += "\nOutput Aspect: " + outputAspect;
    summary += "\nOutput Color Format: " + outputColorFormat;
    summary += "\nOutput Bit Format: " + outputBits;
    summary += "\nOutput Color Space: " + outputColorSpace;
    summary += "\nOutput 1 Recommended Resolution: " + out1RecommendedResolution;
    summary += "\nOutput 1 Color Depth: " + out1ColorDepth;
    summary += "\nOutput 1 Supported Color Space: " + getSupportedColorSpace(out1SupportedColorSpace);

    if (!out2RecommendedResolution.isEmpty())
        summary += "\nOutput 2 Recommended: Resolution" + out2RecommendedResolution;
    if (!out2ColorDepth.isEmpty())
        summary += "\nOutput 2 Color Depth: " + out2ColorDepth;
    if (!out2SupportedColorSpace.isEmpty()) {
        str = getSupportedColorSpace(out2SupportedColorSpace);
        if (!str.isEmpty())
            summary += "\nOutput 2 Supported Color Space: " + str;
    }

    if (!out3RecommendedResolution.isEmpty())
        summary += "\nOutput 3 Recommended Resolution: " + out3RecommendedResolution;
    if (!out3ColorDepth.isEmpty())
        summary += "\nOutput 3 Color Depth: " + out3ColorDepth;
    if (!out3SupportedColorSpace.isEmpty()) {
        str = getSupportedColorSpace(out3SupportedColorSpace);
        if (!str.isEmpty())
            summary += "\nOutput 3 Supported Color Space: " + str;
    }
    if (!input3DFormat.isEmpty())
        summary += "\nInput 3D Format: " + input3DFormat;
    if (!output3DFormat.isEmpty())
        summary += "\nOutput 3D Format: " + output3DFormat;
    if (!out4RecommendedResolution.isEmpty())
        summary += "\nOutput 4 Recommended Reslolution: " + out4RecommendedResolution;
    if (!out4ColorDepth.isEmpty())
        summary += "\nOutput 4 Color Depth: " + out4ColorDepth;
    if (!out4SupportedColorSpace.isEmpty()) {
        str = getSupportedColorSpace(out4SupportedColorSpace);
        if (!str.isEmpty())
            summary += "\nOutput 4 Supported Color Space: " + str;
    }

    return summary;
}

QString VideoStatusDataResponse_VST::getSupportedColorSpace(QString str)
{
    QString cs;
    for(int i = 0; i <str.length() && i < 5; i++)
    {
        //qDebug() << str[i] << SUPPORTED_COLOR_SPACE[i];
        if (str[i] != '0')
            cs += QString(SUPPORTED_COLOR_SPACE[i]) + " ";
    }
    return cs;
}
