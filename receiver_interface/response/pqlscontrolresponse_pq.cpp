#include "pqlscontrolresponse_pq.h"
#include <cstdio>

PQLSControlResponse_PQ::PQLSControlResponse_PQ() :
    m_PQLSControlOn(false)
{

}

PQLSControlResponse_PQ::~PQLSControlResponse_PQ()
{

}

QStringList PQLSControlResponse_PQ::getMsgIDs()
{
    return QStringList() << "PQ" << "PQL";
}

QString PQLSControlResponse_PQ::getResponseID()
{
    return "PQLSControlResponse";
}

bool PQLSControlResponse_PQ::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "PQ%d", &n))
    {
        m_PQLSControlOn = (n != 0);
        return true;
    }
    if (sscanf(str.toLatin1(), "PQL%d", &n))
    {
        m_PQLSControlOn = (n != 0);
        return true;
    }
    return false;
}

bool PQLSControlResponse_PQ::IsPQLSControlOn()
{
    return m_PQLSControlOn;
}
