#include "soundretrieverresponse_ata.h"
#include <cstdio>

SoundRetrieverResponse_ATA::SoundRetrieverResponse_ATA() :
    m_SoundRetrieverOn(false)
{

}

SoundRetrieverResponse_ATA::~SoundRetrieverResponse_ATA()
{

}

QStringList SoundRetrieverResponse_ATA::getMsgIDs()
{
    return QStringList() << "ATA" << "MOT";
}

QString SoundRetrieverResponse_ATA::getResponseID()
{
    return "SoundRetrieverResponse";
}

bool SoundRetrieverResponse_ATA::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "ATA%d", &n))
    {
        m_SoundRetrieverOn = (n != 0);
        return true;
    }
    if (sscanf(str.toLatin1(), "MOT%d", &n))
    {
        m_SoundRetrieverOn = (n != 0);
        return true;
    }
    return false;
}

bool SoundRetrieverResponse_ATA::IsSoundRetrieverOn()
{
    return m_SoundRetrieverOn;
}
