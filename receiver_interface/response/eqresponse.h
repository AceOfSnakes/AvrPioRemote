#ifndef EQRESPONSE_H
#define EQRESPONSE_H

#include "receivedobjectbase.h"
#include <QVector>

class EQResponse : public ReceivedObjectBase
{
public:
    EQResponse();
    ~EQResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    const QVector<int>& GetEQData();
private:
    QVector<int>    m_EQData;
};

#endif // EQRESPONSE_H
