#include "errorresponse_b_e.h"
#include <cstdio>

ErrorResponse_B_E::ErrorResponse_B_E() :
    m_ErrorType(ErrorUnknown)
{

}

ErrorResponse_B_E::~ErrorResponse_B_E()
{

}

QStringList ErrorResponse_B_E::getMsgIDs()
{
    return QStringList() << "B" << "E";
}

QString ErrorResponse_B_E::getResponseID()
{
    return "ErrorResponse";
}

bool ErrorResponse_B_E::parseString(QString str)
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

ErrorResponse_B_E::ErrorType ErrorResponse_B_E::GetError()
{
    return m_ErrorType;
}
