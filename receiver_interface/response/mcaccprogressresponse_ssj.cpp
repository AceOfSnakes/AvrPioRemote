#include "mcaccprogressresponse_ssj.h"

MCACCProgressResponse_SSJ::MCACCProgressResponse_SSJ():
    m_progress(0),
    m_total(0),
    m_status(0),
    m_error(NO_ERROR)
{

}

QStringList MCACCProgressResponse_SSJ::getMsgIDs()
{
    return QStringList() << "SSJ";
}

QString MCACCProgressResponse_SSJ::getResponseID()
{
    return "MCACCProgressResponse";
}

bool MCACCProgressResponse_SSJ::parseString(QString str)
{
    if (str.length() >= 10)
    {
        m_progress = str.mid(3, 2).toInt();
        m_total = str.mid(5, 2).toInt();
        m_status = str.mid(7, 1).toInt();
        m_error = (ERROR)str.mid(9, 1).toInt();
        return true;
    }
    return false;
}

int MCACCProgressResponse_SSJ::GetProgress()
{
    return m_progress;
}

int MCACCProgressResponse_SSJ::GetTotal()
{
    return m_total;
}

int MCACCProgressResponse_SSJ::GetStatus()
{
    return m_status;
}

MCACCProgressResponse_SSJ::ERROR MCACCProgressResponse_SSJ::GetError()
{
    return m_error;
}

QString MCACCProgressResponse_SSJ::GetErrorDescription()
{
    QString description = tr("Unknown");
    switch(m_error){
    case NO_ERROR:
        if (m_status == 0)
            description = tr("");
        else
            description = tr("Measuring running");
        break;
    case NO_MIC:
        description = tr("No microphon connected");
        break;
    case NOISY:
        description = tr("Too noisy");
        break;
    case MIC_ERROR:
        description = tr("Microphon problem");
        break;
    case UNSUPPORTED_CONNECTION:
        description = tr("Unsupported connection");
        break;
    case REVERSE_PHASE:
        description = tr("Phase reserved");
        break;
    case SUBWOOFER_LEVEL_ERROR:
        description = tr("Subwoofer level error");
        break;
    default:
        break;
    }
    return description;
}

