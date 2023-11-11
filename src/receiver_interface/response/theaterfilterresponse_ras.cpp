#include "theaterfilterresponse_ras.h"

TheaterFilterResponse_RAS::TheaterFilterResponse_RAS() :
    m_Value(0)
{

}

TheaterFilterResponse_RAS::~TheaterFilterResponse_RAS()
{

}

QStringList TheaterFilterResponse_RAS::getMsgIDs()
{
    return QStringList() << "RAS";
}

QString TheaterFilterResponse_RAS::getResponseID()
{
    return "TheaterFilterResponse";
}

bool TheaterFilterResponse_RAS::parseString(QString str)
{
    if (sscanf(str.toLatin1(), "RAS%d", &m_Value) == 1)
    {
        return true;
    }
    return false;
}

int TheaterFilterResponse_RAS::GetValue()
{
    return m_Value;
}
