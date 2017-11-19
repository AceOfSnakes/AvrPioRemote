#include "inputfunctionresponse_fn.h"
#include <cstdio>

QMap<int, QString> InputFunctionResponse_FN::_initMap()
{
    QMap<int, QString> inputs;

    inputs[ 0] = "PHONO";
    inputs[ 1] = "CD";
    inputs[ 2] = "TUNER";
    inputs[ 3] = "CD-R / TAPE";
    inputs[ 4] = "DVD";
    inputs[ 5] = "TV / SAT";
    inputs[ 6] = "SAT/CBL";
    inputs[10] = "VIDEO 1"; //(VIDEO)",
    inputs[12] = "MULTI CH IN";
    inputs[13] = "USB DAC";
    inputs[14] = "VIDEO 2";
    inputs[15] = "DVR / BDR";
    inputs[17] = "iPod / USB";
    inputs[18] = "XM RADIO";
    inputs[19] = "HDMI 1";
    inputs[20] = "HDMI 2";
    inputs[21] = "HDMI 3";
    inputs[22] = "HDMI 4";
    inputs[23] = "HDMI 5";
    inputs[24] = "HDMI 6";
    inputs[25] = "BD";
    inputs[26] = "HMG (Internet Radio)"; //HOME MEDIA GALLERY
    inputs[27] = "SIRIUS";
    inputs[31] = "HDMI (cyclic)";
    inputs[33] = "ADAPTER PORT";
    inputs[34] = "HDMI 7";
    inputs[38] = "INTERNET RADIO";
    inputs[40] = "SiriusXM";
    inputs[41] = "PANDORA";
    inputs[44] = "MEDIA SERVER";
    inputs[45] = "FAVORITES";
    inputs[48] = "MHL";

    return inputs;
}

QMap<int, QString> InputFunctionResponse_FN::_initOnkyoMap()
{
    QMap<int, QString> inputs;
    inputs[0x01] = "CBL/SAT";
    inputs[0x02] = "GAME";
    inputs[0x03] = "AUX";
    inputs[0x10] = "BD/DVD";
    inputs[0x11] = "STRM BOX";
    inputs[0x12] = "TV";
    inputs[0x22] = "PHONO";
    inputs[0x23] = "CD";
    inputs[0x24] = "FM";
    inputs[0x25] = "AM";
    inputs[0x26] = "TUNER";
    inputs[0x29] = "USB(Front)";
    inputs[0x2B] = "NET";
    inputs[0x2C] = "USB(toggle)";
    inputs[0x2E] = "BT AUDIO";
    inputs[0x55] = "HDMI 5";
    inputs[0x56] = "HDMI 6";
    inputs[0x57] = "HDMI 7";
//    "UP"	sets Selector Position Wrap-Around Up
//    "DOWN"	sets Selector Position Wrap-Around Down
//    "QSTN"	gets The Selector Position
    return inputs;
}


QMap<int, QString>  InputFunctionResponse_FN::m_PIONEER_INPUT = _initMap();
QMap<int, QString>  InputFunctionResponse_FN::m_ONKYO_INPUT = _initOnkyoMap();


InputFunctionResponse_FN::InputFunctionResponse_FN() :
    m_inputNumber(-1)
{

}

InputFunctionResponse_FN::~InputFunctionResponse_FN()
{

}

QString InputFunctionResponse_FN::getResponseID()
{
    return "InputFunctionResponse";
}

QStringList InputFunctionResponse_FN::getMsgIDs()
{
    return QStringList() << "FN" << "SLI";
}

bool InputFunctionResponse_FN::parseString(QString str)
{
    if (m_IsPioneer)
    {
        if (sscanf(str.toLatin1(), "FN%d", &m_inputNumber) == 1)
        {
            QString str = m_PIONEER_INPUT[m_inputNumber];
            if (str == "") {
                m_inputNumber = -1;
                return false;
            }
            return true;
        }
    }
    else
    {
        if (sscanf(str.toLatin1(), "SLI%x", &m_inputNumber) == 1)
        {
            QString str = m_ONKYO_INPUT[m_inputNumber];
            if (str == "") {
                m_inputNumber = -1;
                return false;
            }
            return true;
        }
    }
    return false;
}

int InputFunctionResponse_FN::getNumber()
{
    return m_inputNumber;
}

QString InputFunctionResponse_FN::getName()
{

    QString str = "";
    if (m_IsPioneer)
    {
        str = m_PIONEER_INPUT[m_inputNumber];
        if (str == "") {
            m_inputNumber = -1;
            str = tr("unknown");
        }
    }
    else
    {
        str = m_ONKYO_INPUT[m_inputNumber];
        if (str == "") {
            m_inputNumber = -1;
            str = tr("unknown");
        }
    }
    return str;
}

const QMap<int, QString> InputFunctionResponse_FN::getInputList()
{
    if (m_IsPioneer)
    {
        return m_PIONEER_INPUT;
    }
    else
    {
        return m_ONKYO_INPUT;
    }
}
