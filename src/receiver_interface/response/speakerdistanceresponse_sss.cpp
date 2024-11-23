#include "speakerdistanceresponse_sss.h"

SpeakerDistanceResponse_SSS::SpeakerDistanceResponse_SSS() :
    m_MCACCNo(0),
    m_Units(METER),
    m_Distance(0)

{

}

SpeakerDistanceResponse_SSS::~SpeakerDistanceResponse_SSS()
{

}

QStringList SpeakerDistanceResponse_SSS::getMsgIDs()
{
    return QStringList() << "SSS";
}

QString SpeakerDistanceResponse_SSS::getResponseID()
{
    return "SpeakerDistanceResponse";
}

bool SpeakerDistanceResponse_SSS::parseString(QString str)
{
    if (str.startsWith("SSS") && str.length() >= 15)
    {
        m_MCACCNo = str.mid(3, 2).toInt();
        m_Speaker = str.mid(5, 3);
        m_Units = (UNITS)str.mid(8, 1).toInt();
        m_Distance = str.mid(9, 6).toInt();

        return true;
    }
    return false;
}

double SpeakerDistanceResponse_SSS::GetValue()
{
    if (m_Units == FEET) {
        QString tmp = QString("%1").arg(m_Distance, 6, 10, QChar('0'));
        QString ft = tmp.mid(0, 2);
        QString inch = tmp.mid(2, 2);
        QString subInch = tmp.mid(4, 2);
        QString result = ft + "´." + inch + "´´";
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        if (subInch != "00") {
            result += "-" + subInch[0] + "/" + subInch[0];
        }
#else
        if (subInch != "00") {
            result = result.append("-").append(subInch[0]).append("/").append(subInch[0]);
        }
#endif
        result += " " + getUnits();
        return 0;
    } else if (m_Units == METER) {
        return ((double)m_Distance) / 100.0;
    } else {
        return 0;
    }
}

int SpeakerDistanceResponse_SSS::GetMCACCNo()
{
    return m_MCACCNo;
}

QString SpeakerDistanceResponse_SSS::GetSpeakerId()
{
    return m_Speaker;
}

int SpeakerDistanceResponse_SSS::GetRawValue()
{
    return m_Distance;
}

SpeakerDistanceResponse_SSS::UNITS SpeakerDistanceResponse_SSS::getRawUnits()
{
    return m_Units;
}

QString SpeakerDistanceResponse_SSS::getUnits()
{
    if (m_Units == FEET) {
        return "ft";
    } else if (m_Units == METER) {
        return "m";
    } else {
        return "-";
    }
}

