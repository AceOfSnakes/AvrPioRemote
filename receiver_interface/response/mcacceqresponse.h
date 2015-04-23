#ifndef MCACCEQRESPONSE_H
#define MCACCEQRESPONSE_H

#include "receivedobjectbase.h"

class MCACCEQResponse : public ReceivedObjectBase
{
public:
    MCACCEQResponse();
    ~MCACCEQResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    int GetMCACCNo();
    QString GetSpeakerId();
    int GetEqNo();
    int GetEQValue();
    QString GetDBValueString();
    static QString GetDBValueString(int value);
private:
    int         m_MCACCNo;
    QString     m_Speaker;
    int         m_EQNo;
    int         m_EQValue;
};

#endif // MCACCEQRESPONSE_H
