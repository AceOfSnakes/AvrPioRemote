#ifndef HDMICONTROLMODERESPONSE_H
#define HDMICONTROLMODERESPONSE_H

#include "receivedobjectbase.h"

class HDMIControlModeResponse : public ReceivedObjectBase
{
public:
    HDMIControlModeResponse();
    ~HDMIControlModeResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool IsHDMIControlModeOn();
private:
    bool    m_HDMIControlModeOn;
};

#endif // HDMICONTROLMODERESPONSE_H
