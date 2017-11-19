#include "pioneerreceiver.h"

PioneerReceiver::PioneerReceiver()
{

}

void PioneerReceiver::NewDataToRead()
{
    QString data = DiscoveryDevice::read();
    int count = data.length();

    // split lines
    int lineLength = 0;
    int lineStartPos = 0;
    for(int i = 0; i < count; i++)
    {
        if (data[i] != '\n' && data[i] != '\r')
        {
            continue;
        }
        lineLength = i - lineStartPos;
        if (lineLength > 0)
        {
            m_ReceivedString.append(data.mid(lineStartPos, lineLength));
            m_ReceivedString.replace("\r", "");
            m_ReceivedString.replace("\n", "");
            if (m_ReceivedString != "")
            {
//                QString str;
//                //QTextCodec::setCodecForCStrings();
//                str = str.fromUtf8(m_ReceivedString.c_str());

////                QByteArray encodedString = m_ReceivedString.c_str();
////                QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
////                str = codec->toUnicode(encodedString);
//                //str = str.fromStdString(m_ReceivedString);
////                str = str.trimmed();
////                str.remove(QChar('\r'));
////                str.remove(QChar('\n'));

                emit DataReceived(m_ReceivedString);
            }
            m_ReceivedString = "";
        }
        lineStartPos = i + 1;
    }
    if (lineStartPos < count)
    {
        m_ReceivedString.append((const char*)&data[lineStartPos]);
    }
}

