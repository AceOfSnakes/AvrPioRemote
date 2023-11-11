#include "displaydataresponse_fl.h"
#include <cstdio>

DisplayDataResponse_FL::DisplayDataResponse_FL()
{

}

DisplayDataResponse_FL::~DisplayDataResponse_FL()
{

}

QStringList DisplayDataResponse_FL::getMsgIDs()
{
    return QStringList() << "FL" << "FLD";
}

QString DisplayDataResponse_FL::getResponseID()
{
    return "DisplayDataResponse";
}

bool DisplayDataResponse_FL::parseString(QString str)
{
    if (m_IsPioneer)
    {
        if (str.length() > 4 && sscanf(str.toLatin1(), "FL0%1d", &m_DisplayType) == 1)
        {
            m_DisplayLine = DecodeHexString(str.mid(4));
            return true;
        }
    }
    else
    {
        if (str.length() > 3/* && sscanf(str.toLatin1(), "FLD0%1d", &m_DisplayType) == 1*/)
        {
            m_DisplayLine = DecodeHexString(str.mid(3));
            return true;
        }
    }
    return false;
}

QString DisplayDataResponse_FL::getDisplayLine()
{
    return m_DisplayLine;
}

int DisplayDataResponse_FL::getDisplayType()
{
    return m_DisplayType;
}
