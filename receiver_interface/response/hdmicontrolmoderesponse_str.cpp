#include "hdmicontrolmoderesponse_str.h"
#include <cstdio>

HDMIControlModeResponse_STR::HDMIControlModeResponse_STR()
{

}

HDMIControlModeResponse_STR::~HDMIControlModeResponse_STR()
{

}

QStringList HDMIControlModeResponse_STR::getMsgIDs()
{
    return QStringList() << "STR";
}

QString HDMIControlModeResponse_STR::getResponseID()
{
    return "HDMIControlModeResponse";
}

bool HDMIControlModeResponse_STR::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "STR%d", &n) == 1)
    {
        m_HDMIControlModeOn = (n != 0);
        return true;
    }
    return false;
}

bool HDMIControlModeResponse_STR::IsHDMIControlModeOn()
{
    return m_HDMIControlModeOn;
}
