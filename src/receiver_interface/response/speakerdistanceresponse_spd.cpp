#include "speakerdistanceresponse_spd.h"

//SPDMuaaabbbcccdddeeefffggghhhiiijjjkkklllmmm
//sets Speaker Distance
//xxx=001-384 (0.01m - 9.00m unit is meters)
//xxx=001-12C (0.1ft - 30.0ft unit is feet)
//xxx=001-2D0 (0'0-1/2" - 30'0" unit is feet/inch)
//for not exist channel is always 000.
//
//M:MCACC Memory 1-3
//u:Unit 0:feet,1:meters,2:feet/inch
//aaa:Front Left
//bbb:Front Right
//ccc:Center
//ddd:Surround Left
//eee:Surround Right
//fff:Surround Back Left
//ggg:Surround Back Right
//hhh:Subwoofer 1
//iii:Height 1 Left
//jjj:Height 1 Right
//kkk:Height 2 Left
//lll:Height2 Right
//mmm:Subwoofer 2


SpeakerDistanceResponse_SPD::SpeakerDistanceResponse_SPD() :
    m_Speaker(0),
    m_Setting(0)
{

}

SpeakerDistanceResponse_SPD::~SpeakerDistanceResponse_SPD()
{

}

QStringList SpeakerDistanceResponse_SPD::getMsgIDs()
{
    return QStringList() << "SPD";
}

QString SpeakerDistanceResponse_SPD::getResponseID()
{
    return "SpeakerDistanceResponse_SPD";
}

bool SpeakerDistanceResponse_SPD::parseString(QString str)
{
    if (str.startsWith("SSG") && str.length() == 6)
    {
        m_Speaker = str.mid(3, 2).toInt();
        m_Setting = str.mid(5, 1).toInt();

        return true;
    }
    return false;
}

int SpeakerDistanceResponse_SPD::getSpeakerNo()
{
    return m_Speaker;
}

int SpeakerDistanceResponse_SPD::getSpeakerSetting()
{
    return m_Setting;
}
