#ifndef SURROUNDPOSITIONRESPONSE_H
#define SURROUNDPOSITIONRESPONSE_H

#include "receivedobjectbase.h"

class SurroundPositionResponse_SSP : public ReceivedObjectBase
{
public:
    SurroundPositionResponse_SSP();
    ~SurroundPositionResponse_SSP();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    bool IsOnSide();
private:
    bool        m_OnSide;
};

#endif // SURROUNDPOSITIONRESPONSE_H
