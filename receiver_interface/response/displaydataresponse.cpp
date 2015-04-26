#include "displaydataresponse.h"
#include <cstdio>

DisplayDataResponse::DisplayDataResponse()
{

}

DisplayDataResponse::~DisplayDataResponse()
{

}

QStringList DisplayDataResponse::getMsgIDs()
{
    return QStringList() << "FL";
}

QString DisplayDataResponse::getResponseID()
{
    return "DisplayDataResponse";
}

bool DisplayDataResponse::parseString(QString str)
{
    if (str.length() > 4 && sscanf(str.toLatin1(), "FL0%1d", &m_DisplayType) == 1)
    {
        m_DisplayLine = DecodeHexString(str.mid(4));
        return true;
    }
    return false;
}

QString DisplayDataResponse::getDisplayLine()
{
    return m_DisplayLine;
}

int DisplayDataResponse::getDisplayType()
{
    return m_DisplayType;
}
