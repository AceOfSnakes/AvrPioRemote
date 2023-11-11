#ifndef INPUTNAMERESPONSE_H
#define INPUTNAMERESPONSE_H

#include "receivedobjectbase.h"

class InputNameResponse_RGB : public ReceivedObjectBase
{
public:
    InputNameResponse_RGB();
    ~InputNameResponse_RGB();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    bool IsRenamed();
    int GetInputNo();
    QString GetInputName();
private:
    bool    m_Renamed;
    int     m_InputNo;
    QString m_InputName;
};

#endif // INPUTNAMERESPONSE_H
