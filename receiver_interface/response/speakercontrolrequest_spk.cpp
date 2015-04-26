#include "speakercontrolrequest_spk.h"

SpeakerControlRequest_SPK::SpeakerControlRequest_SPK() :
    m_SpeakerSetup(0)
{

}

SpeakerControlRequest_SPK::~SpeakerControlRequest_SPK()
{

}

QStringList SpeakerControlRequest_SPK::getMsgIDs()
{
    return QStringList() << "SPK";
}

QString SpeakerControlRequest_SPK::getResponseID()
{
    return "SpeakerControlRequest";
}

bool SpeakerControlRequest_SPK::parseString(QString str)
{
    if (str.startsWith("SPK"))
    {
        m_SpeakerSetup = str.mid(3).toInt();
        return true;
    }
    return false;
}

int SpeakerControlRequest_SPK::GetSpeakerSetup()
{
    return m_SpeakerSetup;
}
