#ifndef LOUDNESSRESPONSE_H
#define LOUDNESSRESPONSE_H

#include "receivedobjectbase.h"

class LoudnessResponse_LDM : public ReceivedObjectBase
{
public:
    LoudnessResponse_LDM();
    ~LoudnessResponse_LDM();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    int GetValue();

private:
    int     m_Value;
};

#endif // LOUDNESSRESPONSE_H
