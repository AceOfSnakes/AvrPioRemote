#ifndef CHANNELLEVELRESPONSE_H
#define CHANNELLEVELRESPONSE_H

#include "receivedobjectbase.h"

class ChannelLevelResponse_CLV : public ReceivedObjectBase
{
public:
    ChannelLevelResponse_CLV();
    ~ChannelLevelResponse_CLV();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    int GetLevel();
    QString GetSpeaker();
private:
    int     m_Level;
    QString m_Speaker;
};

#endif // CHANNELLEVELRESPONSE_H
