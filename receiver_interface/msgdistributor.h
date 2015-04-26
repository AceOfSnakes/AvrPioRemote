#ifndef MSGMANAGER_H
#define MSGMANAGER_H

#include <QMultiMap>
#include <QVector>
#include "response/receivedobjectbase.h"
#include "response/responselistener.h"
#include "response/inputfunctionresponse_fn.h"
#include "response/powerresponse_pwr_apr_bpr_zep.h"
#include "response/displaydataresponse_fl.h"
#include "response/mcaccnumberresponse_mc.h"
#include "response/hdmipassthroughresponse_stu.h"
#include "response/hdmicontrolresponse_stq.h"
#include "response/hdmicontrolmoderesponse_str.h"
#include "response/hdmicontrolarcresponse_stt.h"
#include "response/volumeresponse_vol_zv_yv.h"
#include "response/muteresponse_mut_z2mut_z3mut.h"
#include "response/inputnameresponse_rgb.h"
#include "response/errorresponse_b_e.h"
#include "response/phasecontrolresponse_is.h"
#include "response/hibitresponse_ati.h"
#include "response/pqlscontrolresponse_pq.h"
#include "response/soundretrieverresponse_ata.h"
#include "response/eqresponse_atb.h"
#include "response/toneresponse_to_zga.h"
#include "response/bassresponse_ba_zgb.h"
#include "response/trebleresponse_tr_zgg.h"
#include "response/xcurveresponse_sst.h"
#include "response/emphasisresponse_ilv.h"
#include "response/audiostatusdataresponse_ast.h"
#include "response/videostatusdataresponse_vst.h"
#include "response/mcacceqresponse_suw.h"
#include "response/speakerdistanceresponse_sss.h"
#include "response/speakersettingresponse_ssg.h"
#include "response/surroundpositionresponse_ssp.h"
#include "response/speakersystemrequest_ssf.h"
#include "response/channellevelresponse_clv.h"
#include "response/speakercontrolrequest_spk.h"
#include "response/xoverresponse_ssq.h"
#include "response/response_aub.h"

class MsgDistributor
{
public:
    ~MsgDistributor();
    static void AddResponseListener(ResponseListener*, QStringList&);
    static void NotifyListener(const QString &);
    static void NotifyListener(ReceivedObjectBase *);

private:
    static MsgDistributor* m_OwnInstance;
    static MsgDistributor *getInstance();

    MsgDistributor();
    void _init();
    void _addResponse(ReceivedObjectBase*);
    void _addResponseListener(ResponseListener* listener, QStringList &ids);
    void _notifyListener(const QString &);
    QString _getIdFromString(const QString &);
    QVector<ReceivedObjectBase*> _getResponseObjects(const QString&);

    QMultiMap<QString, ResponseListener*>    m_Listener;
    QMultiMap<QString, ReceivedObjectBase*>  m_Responses;
};

#endif // MSGMANAGER_H
