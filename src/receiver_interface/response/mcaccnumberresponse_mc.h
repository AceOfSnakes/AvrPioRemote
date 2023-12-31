#ifndef MCACCNUMBERRESPONSE_H
#define MCACCNUMBERRESPONSE_H

#include "receivedobjectbase.h"

class MCACCNumberResponse_MC : public ReceivedObjectBase
{
public:
    MCACCNumberResponse_MC();
    ~MCACCNumberResponse_MC();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    int GetMCACCNumber();
private:
    int    m_MCACCNumber;
};

#endif // MCACCNUMBERRESPONSE_H
