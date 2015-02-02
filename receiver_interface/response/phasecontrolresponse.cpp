#include "phasecontrolresponse.h"

PhaseControlResponse::PhaseControlResponse() :
    m_PhaseControlOn(false)
{

}

PhaseControlResponse::~PhaseControlResponse()
{

}

QStringList PhaseControlResponse::getMsgIDs()
{
    return QStringList() << "IS";
}

QString PhaseControlResponse::getResponseID()
{
    return "PhaseControlResponse";
}

bool PhaseControlResponse::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "IS%d", &n))
    {
        m_PhaseControlOn = (n != 0);
        return true;
    }
    return false;
}

bool PhaseControlResponse::IsPhaseControlOn()
{
    return m_PhaseControlOn;
}
