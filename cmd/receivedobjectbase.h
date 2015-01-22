#ifndef RECEIVEDOBJECTBASE_H
#define RECEIVEDOBJECTBASE_H

#include <QObject>

class ReceivedObjectBase : public QObject
{
    Q_OBJECT
public:
    explicit ReceivedObjectBase(QObject *parent = 0);
    ~ReceivedObjectBase();


signals:

public slots:
};

#endif // RECEIVEDOBJECTBASE_H
