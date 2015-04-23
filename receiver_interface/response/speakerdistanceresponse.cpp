#include "speakerdistanceresponse.h"

SpeakerDistanceResponse::SpeakerDistanceResponse() :
    m_MCACCNo(0),
    m_Units(METER),
    m_Distance(0)

{

}

SpeakerDistanceResponse::~SpeakerDistanceResponse()
{

}

QStringList SpeakerDistanceResponse::getMsgIDs()
{
    return QStringList() << "SSS";
}

QString SpeakerDistanceResponse::getResponseID()
{
    return "SpeakerDistanceResponse";
}

bool SpeakerDistanceResponse::parseString(QString str)
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

double SpeakerDistanceResponse::GetValue()
{
    if (m_Units == FEET) {
        QString tmp = QString("%1").arg(m_Distance, 6, 10, QChar('0'));
        QString ft = tmp.mid(0, 2);
        QString inch = tmp.mid(2, 2);
        QString subInch = tmp.mid(4, 2);
        QString result = ft + "´." + inch + "´´";
        if (subInch != "00")
            result += "-" + subInch[0] + "/" + subInch[0];
        result += " " + getUnits();

        return 0;
    } else if (m_Units == METER) {
        return ((double)m_Distance) / 100.0;
    } else {
        return 0;
    }
}

int SpeakerDistanceResponse::GetMCACCNo()
{
    return m_MCACCNo;
}

QString SpeakerDistanceResponse::GetSpeakerId()
{
    return m_Speaker;
}

int SpeakerDistanceResponse::GetRawValue()
{
    return m_Distance;
}

SpeakerDistanceResponse::UNITS SpeakerDistanceResponse::getRawUnits()
{
    return m_Units;
}

QString SpeakerDistanceResponse::getUnits()
{
    if (m_Units == FEET) {
        return "ft";
    } else if (m_Units == METER) {
        return "m";
    } else {
        return "-";
    }
}

