#ifndef XOVER_SSQ_H
#define XOVER_SSQ_H

#include "receivedobjectbase.h"

class XOverResponse_SSQ : public ReceivedObjectBase
{
public:
    XOverResponse_SSQ();
    ~XOverResponse_SSQ();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    enum Frequency
    {
        Frequency_50Hz,
        Frequency_80Hz,
        Frequency_100Hz,
        Frequency_150Hz,
        Frequency_200Hz,
    };
    Frequency GetFrequency();
private:
    Frequency       m_Frequency;
};

#endif // XOVER_SSQ_H
