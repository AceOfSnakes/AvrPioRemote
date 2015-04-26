#ifndef PASSTHROUGHRESPONSE_H
#define PASSTHROUGHRESPONSE_H

#include "receivedobjectbase.h"
#include <QMap>
#include <QStringList>

class HDMIPassThroughResponse_STU : public ReceivedObjectBase
{
public:
    HDMIPassThroughResponse_STU();
    ~HDMIPassThroughResponse_STU();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    enum PassThroughFunction {
        PassThroughOff = 0,
        PassThroughLast,
        PassThroughBD,
        PassThroughHDMI1,
        PassThroughHDMI2,
        PassThroughHDMI3,
        PassThroughHDMI4,
        PassThroughHDMI5,
        PassThroughHDMI6,
        PassThroughHDMI7,
        PassThroughHDMI8,
        PassThroughHDMI9,
        PassThroughHDMI10,
    };
    PassThroughFunction GetPassThroughFunction();
    QString GetFunctionName();
    static QStringList GetFunctionNamesList();

private:
    PassThroughFunction                         m_LastEnabled;
    static QMap<PassThroughFunction, QString>   m_NameList;
    static QMap<PassThroughFunction, QString>   _initNameList();
};

#endif // PASSTHROUGHRESPONSE_H
