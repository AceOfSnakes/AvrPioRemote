#ifndef EMPHASISRESPONSE_H
#define EMPHASISRESPONSE_H

#include "receivedobjectbase.h"
#include <QVector>


class EmphasisResponse_ILV : public ReceivedObjectBase
{
public:
    EmphasisResponse_ILV();
    ~EmphasisResponse_ILV();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    const QVector<int>& GetEmphasisData();
    static const QString GetCmdData(int n);
    static const QString GetDbString(int n);
private:
    QVector<int>    m_EmData;
};

#endif // EMPHASISRESPONSE_H
