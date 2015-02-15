#include "xcurveresponse.h"

XCurveResponse::XCurveResponse()
{

}

XCurveResponse::~XCurveResponse()
{

}

QStringList XCurveResponse::getMsgIDs()
{
    return QStringList() << "SST";
}

QString XCurveResponse::getResponseID()
{
    return "XCurveResponse";
}

bool XCurveResponse::parseString(QString str)
{
    if (sscanf(str.toLatin1(), "SST%d", &m_Value) == 1)
    {
        return true;
    }
    return false;
}

int XCurveResponse::GetValue()
{
    return m_Value;
}

float XCurveResponse::GetdBValue()
{
    return (float)m_Value * (-.5f);
}

QString XCurveResponse::GetAsString()
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
