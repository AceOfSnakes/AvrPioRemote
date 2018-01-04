#include "audioscalarresponse_asc.h"

AudioScalarResponse_ASC::AudioScalarResponse_ASC() :
    m_Value(0)
{

}

AudioScalarResponse_ASC::~AudioScalarResponse_ASC()
{

}

QStringList AudioScalarResponse_ASC::getMsgIDs()
{
    return QStringList() << "ASC";
}

QString AudioScalarResponse_ASC::getResponseID()
{
    return "AudioScalarResponse";
}

bool AudioScalarResponse_ASC::parseString(QString str)
{
    if (sscanf(str.toLatin1(), "ASC%d", &m_Value) == 1)
    {
        return true;
    }
    return false;
}

int AudioScalarResponse_ASC::GetValue()
{
    return m_Value;
}
