#include "response_aub.h"

Response_AUB::Response_AUB() :
    m_HiBit(false)
{

}

Response_AUB::~Response_AUB()
{

}

QStringList Response_AUB::getMsgIDs()
{
    return QStringList() << "AUB";
}

QString Response_AUB::getResponseID()
{
    return "AUB";
}

bool Response_AUB::parseString(QString str)
{
    if (str.startsWith("AUB") && str.length() > 16)
    {
        m_HiBit = (str[16] == '0');
        return true;
    }
    return false;
}

bool Response_AUB::GetHiBit()
{
    return m_HiBit;
}
