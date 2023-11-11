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
    return QStringList() << "SPK" << "SPL";
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
    if (str.startsWith("SPL"))
    {
        QString str = str.mid(3);
        if (str == "SB")
        {
            m_SpeakerSetup = 0;
        }
        else if (str == "H1")
        {
            m_SpeakerSetup = 0;
        }
        else if (str == "A")
        {
            m_SpeakerSetup = 1;
        }
        else if (str == "B")
        {
            m_SpeakerSetup = 2;
        }
        else if (str == "AB")
        {
            m_SpeakerSetup = 3;
        }

        return true;
    }
    return false;
}

int SpeakerControlResponse_SPK::GetSpeakerSetup()
{
    return m_SpeakerSetup;
}
