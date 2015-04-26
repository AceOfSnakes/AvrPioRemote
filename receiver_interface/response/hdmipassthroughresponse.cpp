#include "hdmipassthroughresponse.h"
#include <cstdio>

QMap<HDMIPassThroughResponse::PassThroughFunction, QString> HDMIPassThroughResponse::m_NameList = _initNameList();

QMap<HDMIPassThroughResponse::PassThroughFunction, QString> HDMIPassThroughResponse::_initNameList()
{
    QMap<PassThroughFunction, QString> list;
    list.insert(PassThroughOff, QString("Off"));
    list.insert(PassThroughLast, QString("Last"));
    list.insert(PassThroughBD, QString("BD"));
    list.insert(PassThroughHDMI1, QString("HDMI 1"));
    list.insert(PassThroughHDMI2, QString("HDMI 2"));
    list.insert(PassThroughHDMI3, QString("HDMI 3"));
    list.insert(PassThroughHDMI4, QString("HDMI 4"));
    list.insert(PassThroughHDMI5, QString("HDMI 5"));
    list.insert(PassThroughHDMI6, QString("HDMI 6"));
    list.insert(PassThroughHDMI7, QString("HDMI 7"));
    list.insert(PassThroughHDMI8, QString("HDMI 8"));
    list.insert(PassThroughHDMI9, QString("HDMI 9"));
    list.insert(PassThroughHDMI10, QString("HDMI 10"));
    return list;
}

HDMIPassThroughResponse::HDMIPassThroughResponse() :
    m_LastEnabled(PassThroughOff)
{

}

HDMIPassThroughResponse::~HDMIPassThroughResponse()
{

}

QStringList HDMIPassThroughResponse::getMsgIDs()
{
    return QStringList() << "STU";
}

QString HDMIPassThroughResponse::getResponseID()
{
    return "HDMIPassThroughResponse";
}

bool HDMIPassThroughResponse::parseString(QString str)
{
    int n = 0;
    if (sscanf(str.toLatin1(), "STU%d", &n) == 1)
    {
        m_LastEnabled = (PassThroughFunction) n;
        return true;
    }
    return false;
}

HDMIPassThroughResponse::PassThroughFunction HDMIPassThroughResponse::GetPassThroughFunction()
{
    return m_LastEnabled;
}

QString HDMIPassThroughResponse::GetFunctionName()
{
    return m_NameList[m_LastEnabled];
}

QStringList HDMIPassThroughResponse::GetFunctionNamesList()
{
    QStringList list;

    list << m_NameList[PassThroughOff];
    list << m_NameList[PassThroughLast];
    list << m_NameList[PassThroughBD];
    list << m_NameList[PassThroughHDMI1];
    list << m_NameList[PassThroughHDMI2];
    list << m_NameList[PassThroughHDMI3];
    list << m_NameList[PassThroughHDMI4];
    list << m_NameList[PassThroughHDMI5];
    list << m_NameList[PassThroughHDMI6];
    list << m_NameList[PassThroughHDMI7];
    list << m_NameList[PassThroughHDMI8];
    list << m_NameList[PassThroughHDMI9];
    list << m_NameList[PassThroughHDMI10];

    return list;
}
