#include "inputnameresponse_rgb.h"

InputNameResponse_RGB::InputNameResponse_RGB() :
    m_Renamed(false),
    m_InputNo(0)
{

}

InputNameResponse_RGB::~InputNameResponse_RGB()
{

}

QStringList InputNameResponse_RGB::getMsgIDs()
{
    return QStringList() << "RGB";
}

QString InputNameResponse_RGB::getResponseID()
{
    return "InputNameResponse";
}

bool InputNameResponse_RGB::parseString(QString str)
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

bool InputNameResponse_RGB::IsRenamed()
{
    return m_Renamed;
}

int InputNameResponse_RGB::GetInputNo()
{
    return m_InputNo;
}

QString InputNameResponse_RGB::GetInputName()
{
    return m_InputName;
}
