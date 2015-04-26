#include "xoverresponse_ssq.h"

XOverResponse_SSQ::XOverResponse_SSQ() :
    m_Frequency(Frequency_50Hz)
{

}

XOverResponse_SSQ::~XOverResponse_SSQ()
{

}

QStringList XOverResponse_SSQ::getMsgIDs()
{
    return QStringList() << "SSQ";
}

QString XOverResponse_SSQ::getResponseID()
{
    return "XOverResponse";
}

bool XOverResponse_SSQ::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "SSQ%d", &n) == 1)
    {
        m_Frequency = (Frequency)n;
        return true;
    }
    return false;
}

XOverResponse_SSQ::Frequency XOverResponse_SSQ::GetFrequency()
{
    return m_Frequency;
}

