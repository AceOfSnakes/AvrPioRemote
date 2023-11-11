#include "channellevelresponse_clv.h"

ChannelLevelResponse_CLV::ChannelLevelResponse_CLV() :
    m_Level(0)
{

}

ChannelLevelResponse_CLV::~ChannelLevelResponse_CLV()
{

}

QStringList ChannelLevelResponse_CLV::getMsgIDs()
{
    return QStringList() << "CLV";
}

QString ChannelLevelResponse_CLV::getResponseID()
{
    return "ChannelLevelResponse";
}

bool ChannelLevelResponse_CLV::parseString(QString str)
{
    if (str.startsWith("CLV"))
    {
        m_Speaker = str.mid(3,3);
        m_Level = str.mid(6,2).toInt();
        return true;
    }
    return false;
}

int ChannelLevelResponse_CLV::GetLevel()
{
    return m_Level;
}

QString ChannelLevelResponse_CLV::GetSpeaker()
{
    return m_Speaker;
}
