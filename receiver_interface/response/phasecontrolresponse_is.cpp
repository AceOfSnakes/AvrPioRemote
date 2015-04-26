#include "phasecontrolresponse_is.h"
#include <cstdio>

PhaseControlResponse_IS::PhaseControlResponse_IS() :
    m_PhaseControlOn(false)
{

}

PhaseControlResponse_IS::~PhaseControlResponse_IS()
{

}

QStringList PhaseControlResponse_IS::getMsgIDs()
{
    return QStringList() << "IS";
}

QString PhaseControlResponse_IS::getResponseID()
{
    return "PhaseControlResponse";
}

bool PhaseControlResponse_IS::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "IS%d", &n))
    {
        m_PhaseControlOn = (n != 0);
        return true;
    }
    return false;
}

bool PhaseControlResponse_IS::IsPhaseControlOn()
{
    return m_PhaseControlOn;
}
