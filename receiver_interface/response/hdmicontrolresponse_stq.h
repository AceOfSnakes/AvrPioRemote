#ifndef HDMICONTROLRESPONSE_H
#define HDMICONTROLRESPONSE_H

#include "receivedobjectbase.h"

class HDMIControlResponse_SRQ : public ReceivedObjectBase
{
public:
    HDMIControlResponse_SRQ();
    ~HDMIControlResponse_SRQ();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool IsHDMIControlOn();
private:
    bool    m_HDMIControlOn;
};

#endif // HDMICONTROLRESPONSE_H
