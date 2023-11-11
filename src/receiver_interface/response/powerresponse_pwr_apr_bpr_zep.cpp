#include "powerresponse_pwr_apr_bpr_zep.h"
#include <cstdio>

PowerResponse_PWR_APR_BPR_ZEP::PowerResponse_PWR_APR_BPR_ZEP() :
    m_PoweredOn(false)
{

}

PowerResponse_PWR_APR_BPR_ZEP::~PowerResponse_PWR_APR_BPR_ZEP()
{

}

QStringList PowerResponse_PWR_APR_BPR_ZEP::getMsgIDs()
{
    return QStringList() << "PWR" << "APR" << "BPR" << "ZEP" << "ZPW" << "PW3";
}

QString PowerResponse_PWR_APR_BPR_ZEP::getResponseID()
{
    return "PowerResponse";
}

bool PowerResponse_PWR_APR_BPR_ZEP::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "PWR%d", &n) == 1)
    {
        m_Zone = ZoneMain;
        if (m_IsPioneer)
        {
            m_PoweredOn = (n == 0);
        }
        else
        {
            m_PoweredOn = (n != 0);
        }
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
    if (sscanf(str.toLatin1(), "ZPW%d", &n) == 1)
    {
        m_Zone = Zone2;
        m_PoweredOn = (n != 0);
        return true;
    }
    if (sscanf(str.toLatin1(), "PW3%d", &n) == 1)
    {
        m_Zone = Zone3;
        m_PoweredOn = (n != 0);
        return true;
    }
    return false;
}

bool PowerResponse_PWR_APR_BPR_ZEP::IsPoweredOn()
{
    return m_PoweredOn;
}
