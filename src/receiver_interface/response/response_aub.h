#ifndef AUB_H
#define AUB_H

#include "receivedobjectbase.h"

class Response_AUB : public ReceivedObjectBase
{
public:
    Response_AUB();
    ~Response_AUB();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);

    bool GetHiBit();
private:
    bool        m_HiBit;
};

#endif // AUB_H
