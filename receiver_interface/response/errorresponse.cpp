#include "errorresponse.h"
#include <cstdio>

ErrorResponse::ErrorResponse() :
    m_ErrorType(ErrorUnknown)
{

}

ErrorResponse::~ErrorResponse()
{

}

QStringList ErrorResponse::getMsgIDs()
{
    return QStringList() << "B" << "E";
}

QString ErrorResponse::getResponseID()
{
    return "ErrorResponse";
}

bool ErrorResponse::parseString(QString str)
{
    int n = 0;
    if (str.startsWith("B00"))
    {
        m_ErrorType = ErrorBusy;
        return true;
    }
    if (sscanf(str.toLatin1(), "E0%d", &n) == 1)
    {
        switch(n)
        {
        case 2:
            m_ErrorType = ErrorDoesntWorkNow;
            break;
        case 3:
            m_ErrorType = ErrorNotSupported;
            break;
        case 4:
            m_ErrorType = ErrorCommand;
            break;
        case 6:
            m_ErrorType = ErrorParameter;
            break;
        default:
            m_ErrorType = ErrorUnknown;
            break;
        }
        return true;
    }
    return false;
}

ErrorResponse::ErrorType ErrorResponse::GetError()
{
    return m_ErrorType;
}
