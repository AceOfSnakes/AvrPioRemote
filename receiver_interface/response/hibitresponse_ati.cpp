#include "hibitresponse_ati.h"
#include <cstdio>

HiBitResponse_ATI::HiBitResponse_ATI() :
    m_HiBitOn(false)
{

}

HiBitResponse_ATI::~HiBitResponse_ATI()
{

}

QStringList HiBitResponse_ATI::getMsgIDs()
{
    return QStringList() << "ATI";
}

QString HiBitResponse_ATI::getResponseID()
{
    return "HiBitResponse";
}

bool HiBitResponse_ATI::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "ATI%d", &n))
    {
        m_HiBitOn = (n != 0);
        return true;
    }
    return false;
}

bool HiBitResponse_ATI::IsHiBitOn()
{
    return m_HiBitOn;
}
