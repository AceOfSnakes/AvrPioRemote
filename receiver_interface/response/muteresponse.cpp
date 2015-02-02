#include "muteresponse.h"

MuteResponse::MuteResponse() :
    m_Muted(false),
    m_Zone(ZoneMain)
{

}

MuteResponse::~MuteResponse()
{

}

QStringList MuteResponse::getMsgIDs()
{
    return QStringList() << "MUT" << "Z2MUT" << "Z3MUT";
}

QString MuteResponse::getResponseID()
{
    return "MuteResponse";
}

bool MuteResponse::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "MUT%d", &n) == 1)
    {
        m_Zone = ZoneMain;
        m_Muted = (n == 0);
        return true;
    }
    if (sscanf(str.toLatin1(), "Z2MUT%d", &n) == 1)
    {
        m_Zone = Zone2;
        m_Muted = (n == 0);
        return true;
    }
    if (sscanf(str.toLatin1(), "Z3MUT%d", &n) == 1)
    {
        m_Zone = Zone3;
        m_Muted = (n == 0);
        return true;
    }
    return false;
}

bool MuteResponse::IsMuted()
{
    return m_Muted;
}

MuteResponse::Zone MuteResponse::GetZone()
{
    return m_Zone;
}
