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

void MsgDistributor::NotifyListener(const QString &str, const bool is_pioneer)
{
    getInstance()->_notifyListener(str, is_pioneer);
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
    _addResponse(new InputFunctionResponse_FN());
    _addResponse(new PowerResponse_PWR_APR_BPR_ZEP());
    _addResponse(new DisplayDataResponse_FL());
    _addResponse(new MCACCNumberResponse_MC());
    _addResponse(new HDMIPassThroughResponse_STU());
    _addResponse(new HDMIControlResponse_SRQ());
    _addResponse(new HDMIControlModeResponse_STR());
    _addResponse(new HDMIControlARCResponse_STT());
    _addResponse(new VolumeResponse_VOL_ZV_YV());
    _addResponse(new MuteResponse_MUT_Z2MUT_Z3MUT());
    _addResponse(new InputNameResponse_RGB());
    _addResponse(new ErrorResponse_B_E());
    _addResponse(new PhaseControlResponse_IS());
    _addResponse(new HiBitResponse_ATI());
    _addResponse(new PQLSControlResponse_PQ());
    _addResponse(new SoundRetrieverResponse_ATA());
    _addResponse(new EQResponse_ATB());
    _addResponse(new ToneResponse_TO_ZGA());
    _addResponse(new BassResponse_BA_ZGB());
    _addResponse(new TrebleResponse_TR_ZGG());
    _addResponse(new XCurveResponse_SST());
    _addResponse(new EmphasisResponse_ILV());
    _addResponse(new AudioStatusDataResponse_AST());
    _addResponse(new VideoStatusDataResponse_VST());
    _addResponse(new MCACCEQResponse_SUW());
    _addResponse(new SpeakerDistanceResponse_SSS());
    _addResponse(new SpeakerSettingResponse_SSG());
    _addResponse(new SurroundPositionResponse_SSP());
    _addResponse(new SpeakerSystemRequest_SSF());
    _addResponse(new ChannelLevelResponse_CLV());
    _addResponse(new SpeakerControlResponse_SPK());
    _addResponse(new XOverResponse_SSQ());
    _addResponse(new Response_AUB());
    _addResponse(new MCACCProgressResponse_SSJ());
    _addResponse(new DigitalFilterResponse_DGF());
    _addResponse(new DRCResponse_LTN());
    _addResponse(new TheaterFilterResponse_RAS());
    _addResponse(new UpsamplinResponse_UPS());
    _addResponse(new LoudnessResponse_LDM());
    _addResponse(new FixedPCMResponse_FXP());
    _addResponse(new AudioScalarResponse_ASC());
    _addResponse(new SpeakerInformationResponse_SPI());
    _addResponse(new SpeakerDistanceResponse_SPD());
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

void MsgDistributor::_notifyListener(const QString& str, const bool is_pioneer)
{
    QVector<ReceivedObjectBase*> objs = _getResponseObjects(str, is_pioneer);
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
    if (str.startsWith("Z2MUT"))
        id = "Z2MUT";
    else if (str.startsWith("Z3MUT"))
        id = "Z3MUT";
    else if (str.startsWith("CLV"))
        id = "CLV";
    else if (str.startsWith("VL3"))
        id = "VL3";
    else if (str.startsWith("MT3"))
        id = "MT3";
    else if (str.startsWith("TFRB"))
        id = "TFRB";
    else if (str.startsWith("ZTNB"))
        id = "ZTNB";
    else if (str.startsWith("ZBLB"))
        id = "ZBLB";
    else if (str.startsWith("IFA"))
        id = "IFA";
    else if (str.startsWith("IFV"))
        id = "IFV";
    else if (str.startsWith("PW3"))
        id = "PW3";
    else
    {
        foreach(QChar c, str)
        {
            if (!c.isLetter())
                break;
            id+=c;
        }
    }
    return id;
}

QVector<ReceivedObjectBase*> MsgDistributor::_getResponseObjects(const QString &str, const bool is_pioneer)
{
    QVector<ReceivedObjectBase*> v;
    QString id;
    id = _getIdFromString(str);
    QList<ReceivedObjectBase*> responseList = m_Responses.values(id);
    foreach (ReceivedObjectBase* response, responseList)
    {
        if (response->parseString(str, is_pioneer))
            v.append(response);
    }
    return v;
}

