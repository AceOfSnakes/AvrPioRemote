#include "trebleresponse_tr_zgg.h"
#include <cstdio>

TrebleResponse_TR_ZGG::TrebleResponse_TR_ZGG() :
    m_Value(0)
{

}

TrebleResponse_TR_ZGG::~TrebleResponse_TR_ZGG()
{

}

QStringList TrebleResponse_TR_ZGG::getMsgIDs()
{
    return QStringList() << "TR" << "ZGC" << "TFRB" << "ZTNB" << "ZBLB";
}

QString TrebleResponse_TR_ZGG::getResponseID()
{
    return "TrebleResponse";
}

bool TrebleResponse_TR_ZGG::parseString(QString str)
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
    int tmp = 0;
    if (sscanf(str.toLatin1(), "TFRB%xT%x", &tmp, &m_Value) == 2)
    {
        m_Value = -m_Value;
        m_Zone = ZoneMain;
        return true;
    }
    if (sscanf(str.toLatin1(), "ZTNB%xT%x", &tmp, &m_Value) == 2)
    {
        m_Value = -m_Value;
        m_Zone = Zone2;
        return true;
    }
    if (sscanf(str.toLatin1(), "ZBLB%xT%x", &tmp, &m_Value) == 2)
    {
        m_Value = -m_Value;
        m_Zone = Zone3;
        return true;
    }
    return false;
}

float TrebleResponse_TR_ZGG::GetdBValue()
{
    float volume_dB = 0;

    if (m_IsPioneer)
    {
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
    }
    else
    {
        volume_dB = -m_Value;
    }

    return volume_dB;
}

QString TrebleResponse_TR_ZGG::GetAsString()
{
    QString str;
    float dBValue = GetdBValue();

    str = QString("%1dB").arg(dBValue);

    return str;
}

int TrebleResponse_TR_ZGG::GetValue()
{
    return m_Value;
}
