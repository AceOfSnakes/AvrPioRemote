#include "drcresponse_ltn.h"

DRCResponse_LTN::DRCResponse_LTN() :
    m_Value(0)
{

}

DRCResponse_LTN::~DRCResponse_LTN()
{

}

QStringList DRCResponse_LTN::getMsgIDs()
{
    return QStringList() << "LTN";
}

QString DRCResponse_LTN::getResponseID()
{
    return "DRCResponse";
}

bool DRCResponse_LTN::parseString(QString str)
{
    if (sscanf(str.toLatin1(), "LTN%d", &m_Value) == 1)
    {
        return true;
    }
    return false;
}

int DRCResponse_LTN::GetValue()
{
    return m_Value;
}
