#include "surroundpositionresponse_ssp.h"

SurroundPositionResponse_SSP::SurroundPositionResponse_SSP() :
    m_OnSide(false)
{

}

SurroundPositionResponse_SSP::~SurroundPositionResponse_SSP()
{

}

QStringList SurroundPositionResponse_SSP::getMsgIDs()
{
    return QStringList() << "SSP";
}

QString SurroundPositionResponse_SSP::getResponseID()
{
    return "SurroundPositionResponse";
}

bool SurroundPositionResponse_SSP::parseString(QString str)
{
    if (str.startsWith("SSP"))
    {
        m_OnSide = (str.mid(3) == "0");
        return true;
    }
    return false;
}

bool SurroundPositionResponse_SSP::IsOnSide()
{
    return m_OnSide;
}
