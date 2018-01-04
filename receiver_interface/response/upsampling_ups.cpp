#include "upsampling_ups.h"

UpsamplinResponse_UPS::UpsamplinResponse_UPS() :
    m_Value(0)
{

}

UpsamplinResponse_UPS::~UpsamplinResponse_UPS()
{

}

QStringList UpsamplinResponse_UPS::getMsgIDs()
{
    return QStringList() << "UPS";
}

QString UpsamplinResponse_UPS::getResponseID()
{
    return "UpsamplinResponse";
}

bool UpsamplinResponse_UPS::parseString(QString str)
{
    if (sscanf(str.toLatin1(), "UPS%d", &m_Value) == 1)
    {
        return true;
    }
    return false;
}

int UpsamplinResponse_UPS::GetValue()
{
    return m_Value;
}
