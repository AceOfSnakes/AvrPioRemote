#include "hdmicontrolarcresponse_stt.h"
#include <cstdio>

HDMIControlARCResponse_STT::HDMIControlARCResponse_STT()
{

}

HDMIControlARCResponse_STT::~HDMIControlARCResponse_STT()
{

}

QStringList HDMIControlARCResponse_STT::getMsgIDs()
{
    return QStringList() << "STT";
}

QString HDMIControlARCResponse_STT::getResponseID()
{
    return "HDMIControlARCResponse";
}

bool HDMIControlARCResponse_STT::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "STT%d", &n) == 1)
    {
        m_HDMIControlARCOn = (n != 0);
        return true;
    }
    return false;
}

bool HDMIControlARCResponse_STT::IsHDMIControlARCOn()
{
    return m_HDMIControlARCOn;
}
