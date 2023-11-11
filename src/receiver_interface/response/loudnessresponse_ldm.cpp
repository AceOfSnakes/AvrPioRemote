#include "loudnessresponse_ldm.h"

LoudnessResponse_LDM::LoudnessResponse_LDM() :
    m_Value(0)
{

}

LoudnessResponse_LDM::~LoudnessResponse_LDM()
{

}

QStringList LoudnessResponse_LDM::getMsgIDs()
{
    return QStringList() << "LDM";
}

QString LoudnessResponse_LDM::getResponseID()
{
    return "LoudnessResponse";
}

bool LoudnessResponse_LDM::parseString(QString str)
{
    if (sscanf(str.toLatin1(), "LDM%d", &m_Value) == 1)
    {
        return true;
    }
    return false;
}

int LoudnessResponse_LDM::GetValue()
{
    return m_Value;
}
