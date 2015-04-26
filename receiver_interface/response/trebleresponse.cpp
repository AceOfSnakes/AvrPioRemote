#include "trebleresponse.h"
#include <cstdio>

TrebleResponse::TrebleResponse() :
    m_Value(0)
{

}

TrebleResponse::~TrebleResponse()
{

}

QStringList TrebleResponse::getMsgIDs()
{
    return QStringList() << "TR" << "ZGC";
}

QString TrebleResponse::getResponseID()
{
    return "TrebleResponse";
}

bool TrebleResponse::parseString(QString str)
{
    if (sscanf(str.toLatin1(), "TR%d", &m_Value) == 1)
    {
        m_Zone = ZoneMain;
        return true;
    }
    if (sscanf(str.toLatin1(), "ZGC%d", &m_Value) == 1)
    {
        m_Zone = Zone2;
        return true;
    }
    return false;
}

float TrebleResponse::GetdBValue()
{
    float volume_dB = 0;

    switch(m_Zone)
    {
    case ZoneMain:
        volume_dB = (double)(6 - m_Value);
        break;
    case Zone2:
        volume_dB = (double)(m_Value - 50);
        break;
    default:
        break;
    }

    return volume_dB;
}

QString TrebleResponse::GetAsString()
{
    QString str;
    float dBValue = GetdBValue();

    str = QString("%1dB").arg(dBValue);

    return str;
}

int TrebleResponse::GetValue()
{
    return m_Value;
}
