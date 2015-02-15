#ifndef EMPHASISRESPONSE_H
#define EMPHASISRESPONSE_H

#include "receivedobjectbase.h"
#include <QVector>


class EmphasisResponse : public ReceivedObjectBase
{
public:
    EmphasisResponse();
    ~EmphasisResponse();
    virtual QString getResponseID();
    virtual QStringList getMsgIDs();
    virtual bool parseString(QString str);
    const QVector<int>& GetEmphasisData();
private:
    QVector<int>    m_EmData;
};

#endif // EMPHASISRESPONSE_H
