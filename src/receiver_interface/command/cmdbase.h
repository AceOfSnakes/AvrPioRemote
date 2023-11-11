#ifndef CMDBASE_H
#define CMDBASE_H

#include <QObject>

class CmdBase : public QObject
{
    Q_OBJECT
public:
    explicit CmdBase(QObject *parent = 0);
    ~CmdBase();

    void parseString(QString str);
signals:

public slots:
};

#endif // CMDBASE_H
