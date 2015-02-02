#include "receivedobjectbase.h"

ReceivedObjectBase::ReceivedObjectBase(QObject *parent) : QObject(parent)
{

}

ReceivedObjectBase::~ReceivedObjectBase()
{

}

QString ReceivedObjectBase::DecodeHexString(const QString& hex)
{
    QString str = "";
    for (int i = 0; i < (int)hex.length(); i+=2)
    {
        int c = hex.mid(i, 2).toInt(NULL, 16);
        if (c == 5)
            str += "[)";
        else if (c == 6)
            str += "(]";
        else if (c == 9)
            str += "<|";
        else if (c == 10)
            str += "|>";
        else
            str += (QChar)c;
    }
    return str;
}

