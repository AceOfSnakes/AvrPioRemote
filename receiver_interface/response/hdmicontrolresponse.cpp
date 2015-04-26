#include "hdmicontrolresponse.h"
#include <cstdio>

HDMIControlResponse::HDMIControlResponse()
{

}

HDMIControlResponse::~HDMIControlResponse()
{

}

QStringList HDMIControlResponse::getMsgIDs()
{
    return QStringList() << "STQ";
}

QString HDMIControlResponse::getResponseID()
{
    return "HDMIControlResponse";
}

bool HDMIControlResponse::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "STQ%d", &n) == 1)
    {
        m_HDMIControlOn = (n != 0);
        return true;
    }
    return false;
}

bool HDMIControlResponse::IsHDMIControlOn()
{
    return m_HDMIControlOn;
}
