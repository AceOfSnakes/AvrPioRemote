#ifndef HDMICONTROLARCRESPONSE_H
#define HDMICONTROLARCRESPONSE_H

#include "receivedobjectbase.h"

class HDMIControlARCResponse_STT : public ReceivedObjectBase
{
public:
    HDMIControlARCResponse_STT();
    ~HDMIControlARCResponse_STT();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool IsHDMIControlARCOn();
private:
    bool    m_HDMIControlARCOn;
};

#endif // HDMICONTROLARCRESPONSE_H
