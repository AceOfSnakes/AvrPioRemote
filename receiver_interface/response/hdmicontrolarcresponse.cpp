#include "hdmicontrolarcresponse.h"
#include <cstdio>

HDMIControlARCResponse::HDMIControlARCResponse()
{

}

HDMIControlARCResponse::~HDMIControlARCResponse()
{

}

QStringList HDMIControlARCResponse::getMsgIDs()
{
    return QStringList() << "STT";
}

QString HDMIControlARCResponse::getResponseID()
{
    return "HDMIControlARCResponse";
}

bool HDMIControlARCResponse::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "STT%d", &n) == 1)
    {
        m_HDMIControlARCOn = (n != 0);
        return true;
    }
    return false;
}

bool HDMIControlARCResponse::IsHDMIControlARCOn()
{
    return m_HDMIControlARCOn;
}
