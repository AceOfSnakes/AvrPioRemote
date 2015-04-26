#include "soundretrieverresponse.h"
#include <cstdio>

SoundRetrieverResponse::SoundRetrieverResponse() :
    m_SoundRetrieverOn(false)
{

}

SoundRetrieverResponse::~SoundRetrieverResponse()
{

}

QStringList SoundRetrieverResponse::getMsgIDs()
{
    return QStringList() << "ATA";
}

QString SoundRetrieverResponse::getResponseID()
{
    return "SoundRetrieverResponse";
}

bool SoundRetrieverResponse::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "ATA%d", &n))
    {
        m_SoundRetrieverOn = (n != 0);
        return true;
    }
    return false;
}

bool SoundRetrieverResponse::IsSoundRetrieverOn()
{
    return m_SoundRetrieverOn;
}
