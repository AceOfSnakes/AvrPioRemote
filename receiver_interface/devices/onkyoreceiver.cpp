#include "onkyoreceiver.h"
#include <QtEndian>

#define eISCP_ID_RECEIVER '1'
#define eISCP_ID_PLAYER '7'  // hopefully the right number for the players

eISCPHeader::eISCPHeader()
{
    strncpy(magic, "ISCP", 4);
    headerSize = qToBigEndian(16);
    dataSize = 0;
    version = 1;
    reserved1 = 0;
    reserved2 = 0;
    reserved3 = 0;
}

OnkyoReceiver::OnkyoReceiver()
{
    isReceiver = true;
}

void OnkyoReceiver::setIsReceiver(bool isReceiver)
{
    this->isReceiver = isReceiver;
}

int OnkyoReceiver::write(QString str)
{
    qDebug() << "Onkyo write to " /*<< hostAddress*/ << ":" << port << " ---> " << str;
    eISCPHeader header;
    header.dataSize  = qToBigEndian(str.length() + 3);
    QByteArray datagram;

    datagram.append((char*)(&header), sizeof(header));

    datagram.append('!');
    if (isReceiver)
    {
        datagram.append(eISCP_ID_RECEIVER);
    }
    else
    {
        datagram.append(eISCP_ID_PLAYER);
    }
    datagram.append(str);
    datagram.append('\r');

    socket->readAll(); // clear the incomming buffer, may cause problems
    int n = socket->write(datagram);
    qDebug() << "result: " << n;
    return n;
}

QString OnkyoReceiver::read()
{
    QByteArray data = socket->readAll();
    if (data.size() < sizeof(eISCPHeader))
    {
        qDebug() << "Onkyo error: not enough data for the header";
        return "";
    }
    if (data.size() > 1500)
    {
        qDebug() << "Onkyo error: too much data " << data.size();
        return "";
    }
    eISCPHeader header;
    memcpy((char*)&header, data.data(), sizeof(eISCPHeader));
    if (strncmp("ISCP", header.magic, 4) != 0)
    {
        qDebug() << "Onkyo error: no identifier 'ISCP' found";
        return "";
    }
    header.headerSize = qFromBigEndian(header.headerSize);
    header.dataSize = qFromBigEndian(header.dataSize);
    if (data.size() < (int)(header.headerSize + header.dataSize))
    {
        qDebug() << "Onkyo error: packet size wrong " << data.size() << (header.headerSize + header.dataSize);
        return "";
    }
    int dataBegin = header.headerSize;
    if (data[dataBegin] != '!')
    {
        qDebug() << "Onkyo error: start char '!' not found";
        return "";
    }
    dataBegin++;
    if (isReceiver && data[dataBegin] != eISCP_ID_RECEIVER)
    {
        qDebug() << "Onkyo error: data is not for the receiver";
        return "";
    }
    if (!isReceiver && data[dataBegin] != eISCP_ID_PLAYER)
    {
        qDebug() << "Onkyo error: data is not for the player";
        return "";
    }
    dataBegin++;
    // remove EOL, \r and \n characters
    if ((char)data[data.size() -1] == '\n' || (char)data[data.size() -1] == '\r' || (char)data[data.size() -1] == '\u001A')
    {
        data[data.size() -1] = '\0';
    }
    if ((char)data[data.size() -2] == '\r' || (char)data[data.size() -2] == '\n' || (char)data[data.size() -2] == '\u001A')
    {
        data[data.size() -2] = '\0';
    }
    if ((char)data[data.size() -3] == '\u001A' || (char)data[data.size() -3] == '\r' || (char)data[data.size() -3] == '\n')
    {
        data[data.size() -3] = '\0';
    }

    return QString(data.data() + dataBegin);
}
