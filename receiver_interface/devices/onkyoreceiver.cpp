#include "onkyoreceiver.h"
#include <QtEndian>

#define eISCP_ID_RECEIVER '1'
#define eISCP_ID_PLAYER '7'  // hopefully the right number for the players
#define DATA_MAX_LENGTH 1500

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
    m_IsReceiver = true;
    m_ReceivingData = false;
    m_ReceivingDataLength = 0;
}

void OnkyoReceiver::setIsReceiver(bool isReceiver)
{
    this->m_IsReceiver = isReceiver;
}

int OnkyoReceiver::write(QString str)
{
    qDebug() << "Onkyo write to " /*<< hostAddress*/ << ":" << port << " ---> " << str;
    eISCPHeader header;
    header.dataSize  = qToBigEndian(str.length() + 3);
    QByteArray datagram;

    datagram.append((char*)(&header), sizeof(header));

    datagram.append('!');
    if (m_IsReceiver)
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

void OnkyoReceiver::NewDataToRead()
{
    QByteArray data = socket->readAll();
    qDebug() << data;
    m_BufferedData.append(data);
    if(m_BufferedData.length() > DATA_MAX_LENGTH)
    {
        m_BufferedData.remove(0, m_BufferedData.length() - DATA_MAX_LENGTH);
    }

    bool done;
    do
    {
        done = true;
        if (!m_ReceivingData) // search for header
        {
            int idx = m_BufferedData.indexOf("ISCP");
            if (idx < 0)
            {
                //m_ReceivedData.clear();
                return;
            }
            eISCPHeader header;
            if (isMsgBegin(m_BufferedData, idx, header))
            {
                m_ReceivingData = true;
                m_ReceivingDataLength = header.dataSize;
                m_BufferedData.remove(0, idx + sizeof(eISCPHeader));
                done = false;
            }
        }
        if (m_ReceivingData)
        {
            if (m_BufferedData.length() >= m_ReceivingDataLength)
            {
                // remove EOL, \r and \n characters
                int len = m_ReceivingDataLength;
                for (int i = 2; i >= 0; i--)
                {
                    if ((char)m_BufferedData[len -1] == '\n' || (char)m_BufferedData[len -1] == '\r' || (char)m_BufferedData[len -1] == '\u001A')
                    {
                        len--;
                    }
                }
                if (m_BufferedData[0] != '!')
                {
                    qDebug() << "Onkyo error: start char '!' not found";
                    m_BufferedData.remove(0, m_ReceivingDataLength);
                    m_ReceivingData = false;
                    return;
                }
                if (m_IsReceiver && m_BufferedData[1] != eISCP_ID_RECEIVER)
                {
                    qDebug() << "Onkyo error: data is not for the receiver";
                    m_BufferedData.remove(0, m_ReceivingDataLength);
                    m_ReceivingData = false;
                    return;
                }
                if (!m_IsReceiver && m_BufferedData[1] != eISCP_ID_PLAYER)
                {
                    qDebug() << "Onkyo error: data is not for the player";
                    m_BufferedData.remove(0, m_ReceivingDataLength);
                    m_ReceivingData = false;
                    return;
                }
                QByteArray msg = m_BufferedData.mid(2, len - 2);
                QString received_data = QString(msg);
                m_BufferedData.remove(0, m_ReceivingDataLength);
                m_ReceivingData = false;
                emit DataReceived(received_data);
                done = false;
            }
        }
    } while(!done);
}

bool OnkyoReceiver::isMsgBegin(QByteArray data, int start, eISCPHeader& header)
{
    int size = data.length() - start;
    if (size < sizeof(eISCPHeader))
    {
        qDebug() << "Onkyo error: not enough data for the header";
        return false;
    }
    memcpy((char*)&header, data.data() + start, sizeof(eISCPHeader));
    if (strncmp("ISCP", header.magic, 4) != 0)
    {
        qDebug() << "Onkyo error: no identifier 'ISCP' found";
        return "";
    }
    if (header.version != 1)
    {
        qDebug() << "Onkyo error: header version wrong: 1 != " << header.version;
        return "";
    }
    header.headerSize = qFromBigEndian(header.headerSize);
    if (header.headerSize != sizeof(eISCPHeader))
    {
        qDebug() << "Onkyo error: header size doesn't match: " << sizeof(eISCPHeader) << header.headerSize;
        return "";
    }
    header.dataSize = qFromBigEndian(header.dataSize);
    if (header.dataSize > DATA_MAX_LENGTH)
    {
        qDebug() << "Onkyo error: too much data " << header.dataSize;
        return false;
    }
    return true;
}

//QString OnkyoReceiver::read()
//{
//    QByteArray data = socket->readAll();
//    if (data.size() < sizeof(eISCPHeader))
//    {
//        qDebug() << "Onkyo error: not enough data for the header";
//        return "";
//    }
//    if (data.size() > DATA_MAX_LENGTH)
//    {
//        qDebug() << "Onkyo error: too much data " << data.size();
//        return "";
//    }
//    eISCPHeader header;
//    memcpy((char*)&header, data.data(), sizeof(eISCPHeader));
//    if (strncmp("ISCP", header.magic, 4) != 0)
//    {
//        qDebug() << "Onkyo error: no identifier 'ISCP' found";
//        return "";
//    }
//    header.headerSize = qFromBigEndian(header.headerSize);
//    header.dataSize = qFromBigEndian(header.dataSize);
//    if (data.size() < (int)(header.headerSize + header.dataSize))
//    {
//        qDebug() << "Onkyo error: packet size wrong " << data.size() << (header.headerSize + header.dataSize);
//        return "";
//    }
//    int dataBegin = header.headerSize;
//    if (data[dataBegin] != '!')
//    {
//        qDebug() << "Onkyo error: start char '!' not found";
//        return "";
//    }
//    dataBegin++;
//    if (m_IsReceiver && data[dataBegin] != eISCP_ID_RECEIVER)
//    {
//        qDebug() << "Onkyo error: data is not for the receiver";
//        return "";
//    }
//    if (!m_IsReceiver && data[dataBegin] != eISCP_ID_PLAYER)
//    {
//        qDebug() << "Onkyo error: data is not for the player";
//        return "";
//    }
//    dataBegin++;
//    // remove EOL, \r and \n characters
//    if ((char)data[data.size() -1] == '\n' || (char)data[data.size() -1] == '\r' || (char)data[data.size() -1] == '\u001A')
//    {
//        data[data.size() -1] = '\0';
//    }
//    if ((char)data[data.size() -2] == '\r' || (char)data[data.size() -2] == '\n' || (char)data[data.size() -2] == '\u001A')
//    {
//        data[data.size() -2] = '\0';
//    }
//    if ((char)data[data.size() -3] == '\u001A' || (char)data[data.size() -3] == '\r' || (char)data[data.size() -3] == '\n')
//    {
//        data[data.size() -3] = '\0';
//    }

//    return QString(data.data() + dataBegin);
//}
