#include "speakersystemrequest_ssf.h"

SpeakerSystemRequest_SSF::SpeakerSystemRequest_SSF() :
    m_SpeakerSystem("00")
{

}

SpeakerSystemRequest_SSF::~SpeakerSystemRequest_SSF()
{

}

QStringList SpeakerSystemRequest_SSF::getMsgIDs()
{
    return QStringList() << "SSF";
}

QString SpeakerSystemRequest_SSF::getResponseID()
{
    return "SpeakerSystemRequest";
}

bool SpeakerSystemRequest_SSF::parseString(QString str)
{
    if (str.startsWith("SSF"))
    {
        m_SpeakerSystem = str.mid(3, 2);

        return true;
    }
    return false;
}

QString SpeakerSystemRequest_SSF::GetSpeakerSystem()
{
    return m_SpeakerSystem;
}
