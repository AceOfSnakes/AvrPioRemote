#include "muteresponse_mut_z2mut_z3mut.h"
#include <cstdio>

MuteResponse_MUT_Z2MUT_Z3MUT::MuteResponse_MUT_Z2MUT_Z3MUT() :
    m_Muted(false)
{

}

MuteResponse_MUT_Z2MUT_Z3MUT::~MuteResponse_MUT_Z2MUT_Z3MUT()
{

}

QStringList MuteResponse_MUT_Z2MUT_Z3MUT::getMsgIDs()
{
    return QStringList() << "MUT" << "Z2MUT" << "Z3MUT";
}

QString MuteResponse_MUT_Z2MUT_Z3MUT::getResponseID()
{
    return "MuteResponse";
}

bool MuteResponse_MUT_Z2MUT_Z3MUT::parseString(QString str)
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

bool MuteResponse_MUT_Z2MUT_Z3MUT::IsMuted()
{
    return m_Muted;
}
