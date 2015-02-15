#include "msgdistributor.h"
#include <QMutableHashIterator>
#include <QDebug>

//////////////////////////////////////////////////////
/// STATIC
//////////////////////////////////////////////////////
///
MsgDistributor* MsgDistributor::m_OwnInstance = NULL;

MsgDistributor::MsgDistributor()
{

}

MsgDistributor* MsgDistributor::getInstance()
{
    if (m_OwnInstance == NULL)
    {
        m_OwnInstance = new MsgDistributor();
        m_OwnInstance->_init();
    }
    return m_OwnInstance;
}

void MsgDistributor::AddResponseListener(ResponseListener* listener, QStringList &ids)
{
  getInstance()-> _addResponseListener(listener, ids);
}

void MsgDistributor::NotifyListener(const QString &str)
{
    getInstance()->_notifyListener(str);
}

//////////////////////////////////////////////////////
/// NON STATIC
//////////////////////////////////////////////////////

MsgDistributor::~MsgDistributor()
{
    QVector<ReceivedObjectBase*> toDelete;

    QMultiMap<QString, ReceivedObjectBase*>::const_iterator it = m_Responses.constBegin();
    while(it != m_Responses.constEnd())
    {
       QList<ReceivedObjectBase*> values = m_Responses.values(it.key());
       foreach(ReceivedObjectBase *obj, values)
       {
           if (!toDelete.contains(obj))
               toDelete.append(obj);
       }
       it++;
    }

    m_Responses.clear();
    foreach (ReceivedObjectBase *obj, toDelete) {
        delete obj;
    }
    toDelete.clear();
}

void MsgDistributor::_init()
{
    _addResponse(new InputFunctionResponse());
    _addResponse(new PowerResponse());
    _addResponse(new DisplayDataResponse());
    _addResponse(new MCACCNumberResponse());
    _addResponse(new HDMIPassThroughResponse());
    _addResponse(new HDMIControlResponse());
    _addResponse(new HDMIControlModeResponse());
    _addResponse(new HDMIControlARCResponse());
    _addResponse(new VolumeResponse());
    _addResponse(new MuteResponse());
    _addResponse(new InputNameResponse());
    _addResponse(new ErrorResponse());
    _addResponse(new PhaseControlResponse());
    _addResponse(new HiBitResponse());
    _addResponse(new PQLSControlResponse());
    _addResponse(new SoundRetrieverResponse());
    _addResponse(new EQResponse());
    _addResponse(new ToneResponse());
    _addResponse(new BassResponse());
    _addResponse(new XCurveResponse());
    _addResponse(new EmphasisResponse());
}

void MsgDistributor::_addResponse(ReceivedObjectBase* obj)
{
    if (obj != NULL)
    {
        foreach (QString id, obj->getMsgIDs())
        {
            m_Responses.insert(id, obj);
        }
    }
}

void MsgDistributor::_addResponseListener(ResponseListener* listener, QStringList &ids)
{
    foreach (QString id, ids) {
        m_Listener.insert(id, listener);
    }
}

void MsgDistributor::_notifyListener(const QString& str)
{
    QVector<ReceivedObjectBase*> objs = _getResponseObjects(str);
    foreach (ReceivedObjectBase* obj, objs)
    {
        QList<ResponseListener*> listenerList = m_Listener.values(obj->getResponseID());
        foreach (ResponseListener* listener, listenerList)
        {
            listener->ResponseReceived(obj);
        }
    }
}

QString MsgDistributor::_getIdFromString(const QString &str)
{
    QString id;
    foreach(QChar c, str)
    {
        if (!c.isLetter())
            break;
        id+=c;
    }
    return id;
}

QVector<ReceivedObjectBase*> MsgDistributor::_getResponseObjects(const QString &str)
{
    QVector<ReceivedObjectBase*> v;
    QString id;
    if (str.startsWith("Z2MUT"))
        id = "Z2MUT";
    else if (str.startsWith("Z3MUT"))
        id = "Z3MUT";
    else
        id = _getIdFromString(str);
    QList<ReceivedObjectBase*> responseList = m_Responses.values(id);
    foreach (ReceivedObjectBase* response, responseList)
    {
        if (response->parseString(str))
            v.append(response);
    }
    return v;
}

