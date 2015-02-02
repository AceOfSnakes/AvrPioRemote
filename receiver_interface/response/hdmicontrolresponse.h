#ifndef HDMICONTROLRESPONSE_H
#define HDMICONTROLRESPONSE_H

#include "receivedobjectbase.h"

class HDMIControlResponse : public ReceivedObjectBase
{
public:
    HDMIControlResponse();
    ~HDMIControlResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool IsHDMIControlOn();
private:
    bool    m_HDMIControlOn;
};

#endif // HDMICONTROLRESPONSE_H
