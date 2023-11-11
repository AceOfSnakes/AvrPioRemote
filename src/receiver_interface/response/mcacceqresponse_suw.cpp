#include "mcacceqresponse_suw.h"

MCACCEQResponse_SUW::MCACCEQResponse_SUW() :
    m_MCACCNo(0),
    m_EQNo(0),
    m_EQValue(0)
{

}

MCACCEQResponse_SUW::~MCACCEQResponse_SUW()
{

}

QStringList MCACCEQResponse_SUW::getMsgIDs()
{
    return QStringList() << "SUW";
}

QString MCACCEQResponse_SUW::getResponseID()
{
    return "MCACCEQResponse";
}

bool MCACCEQResponse_SUW::parseString(QString str)
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

QString MCACCEQResponse_SUW::GetDBValueString()
{
    return GetDBValueString(m_EQValue);
}

QString MCACCEQResponse_SUW::GetDBValueString(int value)
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

int MCACCEQResponse_SUW::GetMCACCNo()
{
    return m_MCACCNo;
}

QString MCACCEQResponse_SUW::GetSpeakerId()
{
    return m_Speaker;
}

int MCACCEQResponse_SUW::GetEqNo()
{
    return m_EQNo;
}

int MCACCEQResponse_SUW::GetEQValue()
{
    return m_EQValue;
}
