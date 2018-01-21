#include "volumeresponse_vol_zv_yv.h"
#include <cstdio>

VolumeResponse_VOL_ZV_YV::VolumeResponse_VOL_ZV_YV() :
    m_Volume(0)
{

}

VolumeResponse_VOL_ZV_YV::~VolumeResponse_VOL_ZV_YV()
{

}

QStringList VolumeResponse_VOL_ZV_YV::getMsgIDs()
{
    return QStringList() << "VOL" << "ZV" << "YV" << "MVL" << "ZVL" << "VL3";
}

QString VolumeResponse_VOL_ZV_YV::getResponseID()
{
    return "VolumeResponse";
}

bool VolumeResponse_VOL_ZV_YV::parseString(QString str)
{
    if (m_IsPioneer)
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
    }
    else
    {
        if (sscanf(str.toLatin1(), "MVL%x", &m_Volume) == 1)
        {
            m_Zone = ZoneMain;
            return true;
        }
        if (sscanf(str.toLatin1(), "ZVL%x", &m_Volume) == 1)
        {
            m_Zone = Zone2;
            return true;
        }
        if (sscanf(str.toLatin1(), "VL3%x", &m_Volume) == 1)
        {
            m_Zone = Zone3;
            return true;
        }
    }
    return false;
}

float VolumeResponse_VOL_ZV_YV::GetdBValue()
{
    float volume_dB = 0;

    switch(m_Zone)
    {
    case ZoneMain:
        volume_dB = ((m_IsPioneer) ? (-80.5) : (-82)) + (double)m_Volume * 0.5;
        break;
    case Zone2:
    case Zone3:
    case Zone4:
        volume_dB = (m_IsPioneer) ? (-81.0 + (double)m_Volume * 1.0) : ((double)m_Volume * 0.5);
        break;
    }

    return volume_dB;
}

QString VolumeResponse_VOL_ZV_YV::GetAsString()
{
    QString str;
    float dBValue = GetdBValue();

    switch(m_Zone)
    {
    case ZoneMain:
        if ((m_IsPioneer && (dBValue <= -80.5)) || (!m_IsPioneer && (dBValue <= -82)))
        {
            str = "---.---";
        }
        else if (dBValue <= 0.0)
        {
            str = QString("%1dB").arg(dBValue, 4, 'f', 1);
        }
        else
        {
            str = QString("+%1dB").arg(dBValue, 4, 'f', 1);
        }
        break;
    case Zone2:
    case Zone3:
    case Zone4:
        if (dBValue <= 0.0)
        {
            str = QString("%1dB").arg(dBValue, 4, 'f', 1);
        }
        else
        {
            str = QString("+%1dB").arg(dBValue, 4, 'f', 1);
        }
        break;
    }

    return str;
}

int VolumeResponse_VOL_ZV_YV::GetValue()
{
    return m_Volume;
}
