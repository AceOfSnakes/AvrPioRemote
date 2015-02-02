#include "inputnameresponse.h"

InputNameResponse::InputNameResponse() :
    m_Renamed(false),
    m_InputNo(0)
{

}

InputNameResponse::~InputNameResponse()
{

}

QStringList InputNameResponse::getMsgIDs()
{
    return QStringList() << "RGB";
}

QString InputNameResponse::getResponseID()
{
    return "InputNameResponse";
}

bool InputNameResponse::parseString(QString str)
{
    if (str.startsWith("RGB") && str.length() >= 7)
    {
        m_Renamed = (str[5] == '1');
        m_InputName = str.mid(6);
        m_InputNo = str.mid(3, 2).toInt();
        return true;
    }
    return false;
}

bool InputNameResponse::IsRenamed()
{
    return m_Renamed;
}

int InputNameResponse::GetInputNo()
{
    return m_InputNo;
}

QString InputNameResponse::GetInputName()
{
    return m_InputName;
}
