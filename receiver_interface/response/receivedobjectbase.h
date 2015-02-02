#ifndef RECEIVEDOBJECTBASE_H
#define RECEIVEDOBJECTBASE_H

#include <QObject>
#include <QStringList>

class ReceivedObjectBase : public QObject
{
    Q_OBJECT
public:
    explicit ReceivedObjectBase(QObject *parent = 0);
    ~ReceivedObjectBase();
    virtual QString getResponseID() = 0;
    virtual QStringList getMsgIDs() = 0;
    virtual bool parseString(QString str) = 0;
    enum Zone {
        ZoneMain,
        Zone2,
        Zone3,
        Zone4
    };

protected:
    QString DecodeHexString(const QString& hex);

signals:

public slots:
};

#endif // RECEIVEDOBJECTBASE_H
