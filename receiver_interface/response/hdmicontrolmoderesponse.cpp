#include "hdmicontrolmoderesponse.h"
#include <cstdio>

HDMIControlModeResponse::HDMIControlModeResponse()
{

}

HDMIControlModeResponse::~HDMIControlModeResponse()
{

}

QStringList HDMIControlModeResponse::getMsgIDs()
{
    return QStringList() << "STR";
}

QString HDMIControlModeResponse::getResponseID()
{
    return "HDMIControlModeResponse";
}

bool HDMIControlModeResponse::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "STR%d", &n) == 1)
    {
        m_HDMIControlModeOn = (n != 0);
        return true;
    }
    return false;
}

bool HDMIControlModeResponse::IsHDMIControlModeOn()
{
    return m_HDMIControlModeOn;
}
