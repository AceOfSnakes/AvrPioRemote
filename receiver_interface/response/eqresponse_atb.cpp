#include "eqresponse_atb.h"

EQResponse_ATB::EQResponse_ATB() :
    m_EQData(9, 50)
{

}

EQResponse_ATB::~EQResponse_ATB()
{

}

QStringList EQResponse_ATB::getMsgIDs()
{
    return QStringList() << "ATB";
}

QString EQResponse_ATB::getResponseID()
{
    return "EQResponse";
}

bool EQResponse_ATB::parseString(QString str)
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

const QVector<int> &EQResponse_ATB::GetEQData()
{
    return m_EQData;
}
