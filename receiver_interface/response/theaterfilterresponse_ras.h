#ifndef THEATERFILTER_H
#define THEATERFILTER_H

#include "receivedobjectbase.h"

class TheaterFilterResponse_RAS : public ReceivedObjectBase
{
public:
    TheaterFilterResponse_RAS();
    ~TheaterFilterResponse_RAS();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    int GetValue();

private:
    int     m_Value;
};

#endif // THEATERFILTER_H
