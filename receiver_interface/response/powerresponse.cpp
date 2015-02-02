#include "powerresponse.h"

PowerResponse::PowerResponse() :
    m_PoweredOn(false),
    m_Zone(ZoneMain)
{

}

PowerResponse::~PowerResponse()
{

}

QStringList PowerResponse::getMsgIDs()
{
    return QStringList() << "PWR" << "APR" << "BPR" << "ZEP";
}

QString PowerResponse::getResponseID()
{
    return "PowerResponse";
}

bool PowerResponse::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "PWR%d", &n) == 1)
    {
        m_Zone = ZoneMain;
        m_PoweredOn = (n == 0);
        return true;
    }
    if (sscanf(str.toLatin1(), "APR%d", &n) == 1)
    {
        m_Zone = Zone2;
        m_PoweredOn = (n == 0);
        return true;
    }
    if (sscanf(str.toLatin1(), "BPR%d", &n) == 1)
    {
        m_Zone = Zone3;
        m_PoweredOn = (n == 0);
        return true;
    }
    if (sscanf(str.toLatin1(), "ZEP%d", &n) == 1)
    {
        m_Zone = Zone4;
        m_PoweredOn = (n == 0);
        return true;
    }
    return false;
}

bool PowerResponse::IsPoweredOn()
{
    return m_PoweredOn;
}

PowerResponse::Zone PowerResponse::GetZone()
{
    return m_Zone;
}
