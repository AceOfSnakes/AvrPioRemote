#include "xcurveresponse_sst.h"
#include <cstdio>

XCurveResponse_SST::XCurveResponse_SST()
{

}

XCurveResponse_SST::~XCurveResponse_SST()
{

}

QStringList XCurveResponse_SST::getMsgIDs()
{
    return QStringList() << "SST";
}

QString XCurveResponse_SST::getResponseID()
{
    return "XCurveResponse";
}

bool XCurveResponse_SST::parseString(QString str)
{
    if (sscanf(str.toLatin1(), "SST%d", &m_Value) == 1)
    {
        return true;
    }
    return false;
}

int XCurveResponse_SST::GetValue()
{
    return m_Value;
}

float XCurveResponse_SST::GetdBValue()
{
    return (float)m_Value * (-.5f);
}

QString XCurveResponse_SST::GetAsString()
{
    if (m_Value <= 6 && m_Value > 0)
    {
        return QString("%1").arg(GetdBValue(), 3, 'f', 1);
    }
    else
    {
        return (tr("OFF"));
    }
}
