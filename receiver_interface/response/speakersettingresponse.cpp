#include "speakersettingresponse.h"

SpeakerSettingResponse::SpeakerSettingResponse() :
    m_Speaker(0),
    m_Setting(0)
{

}

SpeakerSettingResponse::~SpeakerSettingResponse()
{

}

QStringList SpeakerSettingResponse::getMsgIDs()
{
    return QStringList() << "SSG";
}

QString SpeakerSettingResponse::getResponseID()
{
    return "SpeakerSettingResponse";
}

bool SpeakerSettingResponse::parseString(QString str)
{
    if (str.startsWith("SSG") && str.length() == 6)
    {
        m_Speaker = str.mid(3, 2).toInt();
        m_Setting = str.mid(5, 1).toInt();

        return true;
    }
    return false;
}

int SpeakerSettingResponse::getSpeakerNo()
{
    return m_Speaker;
}

int SpeakerSettingResponse::getSpeakerSetting()
{
    return m_Setting;
}
