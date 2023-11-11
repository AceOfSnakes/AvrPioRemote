#include "digital_filter_response_dgf.h"

DigitalFilterResponse_DGF::DigitalFilterResponse_DGF() :
    m_Value(0)
{

}

DigitalFilterResponse_DGF::~DigitalFilterResponse_DGF()
{

}

QStringList DigitalFilterResponse_DGF::getMsgIDs()
{
    return QStringList() << "DGF";
}

QString DigitalFilterResponse_DGF::getResponseID()
{
    return "DigitalFilterResponse";
}

bool DigitalFilterResponse_DGF::parseString(QString str)
{
    if (sscanf(str.toLatin1(), "DGF%d", &m_Value) == 1)
    {
        return true;
    }
    return false;
}

int DigitalFilterResponse_DGF::GetValue()
{
    return m_Value;
}
