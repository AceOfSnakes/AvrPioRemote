#include "fixedpcmresponse_fxp.h"

FixedPCMResponse_FXP::FixedPCMResponse_FXP() :
    m_Value(0)
{

}

FixedPCMResponse_FXP::~FixedPCMResponse_FXP()
{

}

QStringList FixedPCMResponse_FXP::getMsgIDs()
{
    return QStringList() << "FXP";
}

QString FixedPCMResponse_FXP::getResponseID()
{
    return "FixedPCMResponse";
}

bool FixedPCMResponse_FXP::parseString(QString str)
{
    if (sscanf(str.toLatin1(), "FXP%d", &m_Value) == 1)
    {
        return true;
    }
    return false;
}

int FixedPCMResponse_FXP::GetValue()
{
    return m_Value;
}
