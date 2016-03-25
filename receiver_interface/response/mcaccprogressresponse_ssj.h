#ifndef MCACCPROGRESSRESPONSE_SSJ_H
#define MCACCPROGRESSRESPONSE_SSJ_H

#include "receivedobjectbase.h"

class MCACCProgressResponse_SSJ : public ReceivedObjectBase
{
public:
    MCACCProgressResponse_SSJ();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    enum ERROR {
        NO_ERROR = 0,
        NO_MIC,
        NOISY,
        MIC_ERROR,
        UNSUPPORTED_CONNECTION,
        REVERSE_PHASE,
        SUBWOOFER_LEVEL_ERROR
    };

    int GetProgress();
    int GetTotal();
    int GetStatus();
    ERROR GetError();
    QString GetErrorDescription();
private:
    int     m_progress;
    int     m_total;
    int     m_status;
    ERROR   m_error;
};

#endif // MCACCPROGRESSRESPONSE_SSJ_H
