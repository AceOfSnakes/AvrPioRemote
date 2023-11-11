#include "mcaccnumberresponse_mc.h"
#include <cstdio>

MCACCNumberResponse_MC::MCACCNumberResponse_MC() :
    m_MCACCNumber(1)
{

}

MCACCNumberResponse_MC::~MCACCNumberResponse_MC()
{

}

QStringList MCACCNumberResponse_MC::getMsgIDs()
{
    return QStringList() << "MC" << "MCM";
}

QString MCACCNumberResponse_MC::getResponseID()
{
    return "MCACCNumberResponse";
}

bool MCACCNumberResponse_MC::parseString(QString str)
{
    if (sscanf(str.toLatin1(), "MC%d", &m_MCACCNumber) == 1)
    {
        return true;
    }
    if (sscanf(str.toLatin1(), "MCM%d", &m_MCACCNumber) == 1)
    {
        return true;
    }
    return false;
}

int MCACCNumberResponse_MC::GetMCACCNumber()
{
    return m_MCACCNumber;
}
