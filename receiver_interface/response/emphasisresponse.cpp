#include "emphasisresponse.h"

EmphasisResponse::EmphasisResponse()
{

}

EmphasisResponse::~EmphasisResponse()
{

}

QStringList EmphasisResponse::getMsgIDs()
{
    return QStringList() << "ILV";
}

QString EmphasisResponse::getResponseID()
{
    return "EmphasisResponse";
}

bool EmphasisResponse::parseString(QString str)
{
    if (str.startsWith("ILV"))
    {
        int count = (str.length() - 3) / 2;
        m_EmData.clear();

        for (int i = 0; i < count; i++)
        {
            int n = str.mid(3 + i * 2, 2).toInt();
            m_EmData.push_back(n);
        }
        return true;
    }
    return false;
}

const QVector<int> &EmphasisResponse::GetEmphasisData()
{
    return m_EmData;
}
