#include "mcacceqresponse.h"

MCACCEQResponse::MCACCEQResponse() :
    m_MCACCNo(0),
    m_EQNo(0),
    m_EQValue(0)
{

}

MCACCEQResponse::~MCACCEQResponse()
{

}

QStringList MCACCEQResponse::getMsgIDs()
{
    return QStringList() << "SUW";
}

QString MCACCEQResponse::getResponseID()
{
    return "MCACCEQResponse";
}

bool MCACCEQResponse::parseString(QString str)
{
    if (str.startsWith("SUW") && str.length() >= 12)
    {
        m_MCACCNo = str.mid(3, 2).toInt();
        m_Speaker = str.mid(5, 3);
        m_EQNo = str.mid(8, 2).toInt();
        m_EQValue = str.mid(10, 2).toInt();

        return true;
    }
    return false;
}

QString MCACCEQResponse::GetDBValueString()
{
    return GetDBValueString(m_EQValue);
}

QString MCACCEQResponse::GetDBValueString(int value)
{
    QString str;
    double eqValue = ((double)value - 50.0) / 2.0;

    if (eqValue == 0.0)//eqValue > -0.1 && eqValue <= 0.1)
    {
        str = "0.0";
    }
    else if (eqValue < 0.0)
        str = QString("%1").arg(eqValue, 3, 'f', 1);
    else
        str = QString("+%1").arg(eqValue, 3, 'f', 1);
   return str;
}

int MCACCEQResponse::GetMCACCNo()
{
    return m_MCACCNo;
}

QString MCACCEQResponse::GetSpeakerId()
{
    return m_Speaker;
}

int MCACCEQResponse::GetEqNo()
{
    return m_EQNo;
}

int MCACCEQResponse::GetEQValue()
{
    return m_EQValue;
}
