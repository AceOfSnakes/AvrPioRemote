#ifndef AUDIOSCALAR_H
#define AUDIOSCALAR_H

#include "receivedobjectbase.h"

class AudioScalarResponse_ASC : public ReceivedObjectBase
{
public:
    AudioScalarResponse_ASC();
    ~AudioScalarResponse_ASC();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    int GetValue();

private:
    int     m_Value;
};

#endif // AUDIOSCALAR_H
