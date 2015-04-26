#include "hibitresponse.h"
#include <cstdio>

HiBitResponse::HiBitResponse() :
    m_HiBitOn(false)
{

}

HiBitResponse::~HiBitResponse()
{

}

QStringList HiBitResponse::getMsgIDs()
{
    return QStringList() << "ATI";
}

QString HiBitResponse::getResponseID()
{
    return "HiBitResponse";
}

bool HiBitResponse::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "ATI%d", &n))
    {
        m_HiBitOn = (n != 0);
        return true;
    }
    return false;
}

bool HiBitResponse::IsHiBitOn()
{
    return m_HiBitOn;
}
