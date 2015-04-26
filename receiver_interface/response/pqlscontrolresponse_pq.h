#ifndef PQLSCONTROLRESPONSE_H
#define PQLSCONTROLRESPONSE_H

#include "receivedobjectbase.h"

class PQLSControlResponse_PQ : public ReceivedObjectBase
{
public:
    PQLSControlResponse_PQ();
    ~PQLSControlResponse_PQ();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool    IsPQLSControlOn();
private:
    bool    m_PQLSControlOn;
};

#endif // PQLSCONTROLRESPONSE_H
