#include "speakercontrolresponse_spk.h"

SpeakerControlResponse_SPK::SpeakerControlResponse_SPK() :
    m_SpeakerSetup(0)
{

}

SpeakerControlResponse_SPK::~SpeakerControlResponse_SPK()
{

}

QStringList SpeakerControlResponse_SPK::getMsgIDs()
{
    return QStringList() << "SPK";
}

QString SpeakerControlResponse_SPK::getResponseID()
{
    return "SpeakerControlResponse";
}

bool SpeakerControlResponse_SPK::parseString(QString str)
{
    if (str.startsWith("SPK"))
    {
        m_SpeakerSetup = str.mid(3).toInt();
        return true;
    }
    return false;
}

int SpeakerControlResponse_SPK::GetSpeakerSetup()
{
    return m_SpeakerSetup;
}
