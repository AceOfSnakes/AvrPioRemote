#ifndef ERRORRESPONSE_H
#define ERRORRESPONSE_H

#include "receivedobjectbase.h"

class ErrorResponse : public ReceivedObjectBase
{
public:
    ErrorResponse();
    ~ErrorResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    enum ErrorType {
        ErrorUnknown = -2,
        ErrorBusy = -1,
        ErrorDoesntWorkNow = 2,
        ErrorNotSupported = 3,
        ErrorCommand = 4,
        ErrorParameter = 6
    };
    ErrorType GetError();
private:
    ErrorType   m_ErrorType;
};

#endif // ERRORRESPONSE_H
