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
private:
    QVector<int>    m_EmData;
};

#endif // EMPHASISRESPONSE_H
