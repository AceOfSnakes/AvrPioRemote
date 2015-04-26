#include "speakersettingresponse_ssg.h"

SpeakerSettingResponse_SSG::SpeakerSettingResponse_SSG() :
    m_Speaker(0),
    m_Setting(0)
{

}

SpeakerSettingResponse_SSG::~SpeakerSettingResponse_SSG()
{

}

QStringList SpeakerSettingResponse_SSG::getMsgIDs()
{
    return QStringList() << "SSG";
}

QString SpeakerSettingResponse_SSG::getResponseID()
{
    return "SpeakerSettingResponse";
}

bool SpeakerSettingResponse_SSG::parseString(QString str)
{
    if (str.startsWith("SSG") && str.length() == 6)
    {
        m_Speaker = str.mid(3, 2).toInt();
        m_Setting = str.mid(5, 1).toInt();

        return true;
    }
    return false;
}

int SpeakerSettingResponse_SSG::getSpeakerNo()
{
    return m_Speaker;
}

int SpeakerSettingResponse_SSG::getSpeakerSetting()
{
    return m_Setting;
}
