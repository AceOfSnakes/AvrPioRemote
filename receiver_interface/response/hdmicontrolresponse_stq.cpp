#include "hdmicontrolresponse_stq.h"
#include <cstdio>

HDMIControlResponse_SRQ::HDMIControlResponse_SRQ()
{

}

HDMIControlResponse_SRQ::~HDMIControlResponse_SRQ()
{

}

QStringList HDMIControlResponse_SRQ::getMsgIDs()
{
    return QStringList() << "STQ";
}

QString HDMIControlResponse_SRQ::getResponseID()
{
    return "HDMIControlResponse";
}

bool HDMIControlResponse_SRQ::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "STQ%d", &n) == 1)
    {
        m_HDMIControlOn = (n != 0);
        return true;
    }
    return false;
}

bool HDMIControlResponse_SRQ::IsHDMIControlOn()
{
    return m_HDMIControlOn;
}
