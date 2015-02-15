#include "eqresponse.h"

EQResponse::EQResponse() :
    m_EQData(9, 50)
{

}

EQResponse::~EQResponse()
{

}

QStringList EQResponse::getMsgIDs()
{
    return QStringList() << "ATB";
}

QString EQResponse::getResponseID()
{
    return "EQResponse";
}

bool EQResponse::parseString(QString str)
{
    if (str.startsWith("ATB"))
    {
        int n;
        int i;
        for (i = 0; i < m_EQData.count() && (5 + i * 2) < str.length(); i++)
        {
            n = str.mid(5 + i * 2, 2).toInt();
            m_EQData[i] = n;
        }
        return i == 9;
    }
    return false;
}

const QVector<int> &EQResponse::GetEQData()
{
    return m_EQData;
}
