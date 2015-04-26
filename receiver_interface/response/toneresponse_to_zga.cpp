#include "toneresponse_to_zga.h"
#include <cstdio>

ToneResponse_TO_ZGA::ToneResponse_TO_ZGA() :
    m_ToneOn(false)
{

}

ToneResponse_TO_ZGA::~ToneResponse_TO_ZGA()
{

}

QStringList ToneResponse_TO_ZGA::getMsgIDs()
{
    return QStringList() << "TO" << "ZGA";
}

QString ToneResponse_TO_ZGA::getResponseID()
{
    return "ToneResponse";
}

bool ToneResponse_TO_ZGA::parseString(QString str)
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

bool ToneResponse_TO_ZGA::IsToneOn()
{
    return m_ToneOn;
}
