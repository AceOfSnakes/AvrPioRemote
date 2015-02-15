#ifndef XCURVERESPONSE_H
#define XCURVERESPONSE_H

#include "receivedobjectbase.h"

class XCurveResponse : public ReceivedObjectBase
{
public:
    XCurveResponse();
    ~XCurveResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    int GetValue();
    float GetdBValue();
    QString GetAsString();

private:
    int m_Value;
};

#endif // XCURVERESPONSE_H
