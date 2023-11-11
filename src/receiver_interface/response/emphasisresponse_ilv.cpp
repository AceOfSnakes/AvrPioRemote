#include "emphasisresponse_ilv.h"

EmphasisResponse_ILV::EmphasisResponse_ILV()
{

}

EmphasisResponse_ILV::~EmphasisResponse_ILV()
{

}

QStringList EmphasisResponse_ILV::getMsgIDs()
{
    return QStringList() << "ILV" << "TCL";
}

QString EmphasisResponse_ILV::getResponseID()
{
    return "EmphasisResponse";
}

bool EmphasisResponse_ILV::parseString(QString str)
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
    if (str.startsWith("TCL"))
    {
        int count = (str.length() - 3) / 3;
        m_EmData.clear();

        for (int i = 0; i < count; i++)
        {
            bool ok = false;
            int n = str.mid(3 + i * 3, 3).toInt(&ok, 16) + 50;
            m_EmData.push_back(n);
        }
        return true;
    }
    return false;
}

const QVector<int> &EmphasisResponse_ILV::GetEmphasisData()
{
    return m_EmData;
}

const QString EmphasisResponse_ILV::GetCmdData(int n)
{
    QString tmp;
    n = n - 50;
    if (n == 0)
    {
        tmp = "000";
    }
    else if (n < 0)
    {
        tmp = QString::asprintf("-%02X", -n);
    }
    else
    {
        tmp = QString::asprintf("+%02X", n);
    }
    return tmp;
}

const QString EmphasisResponse_ILV::GetDbString(int n)
{
    return QString::asprintf("%+0.1f", ((n-50) / 2.0));
}
