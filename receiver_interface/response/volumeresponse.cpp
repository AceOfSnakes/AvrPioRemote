#include "volumeresponse.h"
#include <cstdio>

VolumeResponse::VolumeResponse() :
    m_Volume(0)
{

}

VolumeResponse::~VolumeResponse()
{

}

QStringList VolumeResponse::getMsgIDs()
{
    return QStringList() << "VOL" << "ZV" << "YV";
}

QString VolumeResponse::getResponseID()
{
    return "VolumeResponse";
}

bool VolumeResponse::parseString(QString str)
{
    if (sscanf(str.toLatin1(), "VOL%d", &m_Volume) == 1)
    {
        m_Zone = ZoneMain;
        return true;
    }
    if (sscanf(str.toLatin1(), "ZV%d", &m_Volume) == 1)
    {
        m_Zone = Zone2;
        return true;
    }
    if (sscanf(str.toLatin1(), "YV%d", &m_Volume) == 1)
    {
        m_Zone = Zone3;
        return true;
    }
    return false;
}

float VolumeResponse::GetdBValue()
{
    float volume_dB = 0;

    switch(m_Zone)
    {
    case ZoneMain:
        volume_dB = -80.5 + (double)m_Volume * 0.5;
        break;
    case Zone2:
    case Zone3:
    case Zone4:
        volume_dB = -81.0 + (double)m_Volume * 1.0;
        break;
    }

    return volume_dB;
}

QString VolumeResponse::GetAsString()
{
    QString str;
    float dBValue = GetdBValue();

    switch(m_Zone)
    {
    case ZoneMain:
        if (dBValue <= -80.5)
            str = "---.---";
        else if (dBValue <= 0.0)
            str = QString("%1dB").arg(dBValue, 4, 'f', 1);
        else
            str = QString("+%1dB").arg(dBValue, 4, 'f', 1);
        break;
    case Zone2:
    case Zone3:
    case Zone4:
        if (dBValue <= 0.0)
            str = QString("%1dB").arg(dBValue, 4, 'f', 1);
        else
            str = QString("+%1dB").arg(dBValue, 4, 'f', 1);
        break;
    }

    return str;
}

int VolumeResponse::GetValue()
{
    return m_Volume;
}
