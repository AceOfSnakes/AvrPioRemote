#include "pqlscontrolresponse.h"
#include <cstdio>

PQLSControlResponse::PQLSControlResponse() :
    m_PQLSControlOn(false)
{

}

PQLSControlResponse::~PQLSControlResponse()
{

}

QStringList PQLSControlResponse::getMsgIDs()
{
    return QStringList() << "PQ";
}

QString PQLSControlResponse::getResponseID()
{
    return "PQLSControlResponse";
}

bool PQLSControlResponse::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "PQ%d", &n))
    {
        m_PQLSControlOn = (n != 0);
        return true;
    }
    return false;
}

bool PQLSControlResponse::IsPQLSControlOn()
{
    return m_PQLSControlOn;
}
