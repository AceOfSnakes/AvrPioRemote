#ifndef DISPLAYDATARESPONSE_H
#define DISPLAYDATARESPONSE_H

#include "receivedobjectbase.h"

class DisplayDataResponse : public ReceivedObjectBase
{
public:
    DisplayDataResponse();
    ~DisplayDataResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    QString getDisplayLine();
    int getDisplayType();
private:
    QString m_DisplayLine;
    int     m_DisplayType;
};

#endif // DISPLAYDATARESPONSE_H
