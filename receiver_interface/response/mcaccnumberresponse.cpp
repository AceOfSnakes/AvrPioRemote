#include "mcaccnumberresponse.h"
#include <cstdio>

MCACCNumberResponse::MCACCNumberResponse() :
    m_MCACCNumber(1)
{

}

MCACCNumberResponse::~MCACCNumberResponse()
{

}

QStringList MCACCNumberResponse::getMsgIDs()
{
    return QStringList() << "MC";
}

QString MCACCNumberResponse::getResponseID()
{
    return "MCACCNumberResponse";
}

bool MCACCNumberResponse::parseString(QString str)
{
    if (sscanf(str.toLatin1(), "MC%d", &m_MCACCNumber) == 1)
    {
        return true;
    }
    return false;
}

int MCACCNumberResponse::GetMCACCNumber()
{
    return m_MCACCNumber;
}
