#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

class Logger
{
public:
    static void Log(const QString& msg);
private:
    Logger();
    static Logger* m_OwnInstance;
};

#endif // LOGGER_H
