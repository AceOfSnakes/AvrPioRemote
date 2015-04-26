#include "toneresponse.h"
#include <cstdio>

ToneResponse::ToneResponse() :
    m_ToneOn(false)
{

}

ToneResponse::~ToneResponse()
{

}

QStringList ToneResponse::getMsgIDs()
{
    return QStringList() << "TO" << "ZGA";
}

QString ToneResponse::getResponseID()
{
    return "ToneResponse";
}

bool ToneResponse::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "TO%d", &n) == 1)
    {
        m_ToneOn = (n != 0);
        m_Zone = ZoneMain;
        return true;
    }
    if (sscanf(str.toLatin1(), "ZGA%d", &n) == 1)
    {
        m_ToneOn = (n != 0);
        m_Zone = Zone2;
        return true;
    }
    return false;
}

bool ToneResponse::IsToneOn()
{
    return m_ToneOn;
}
