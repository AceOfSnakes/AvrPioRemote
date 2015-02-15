#ifndef MSGMANAGER_H
#define MSGMANAGER_H

#include <QMultiMap>
#include <QVector>
#include "response/receivedobjectbase.h"
#include "response/responselistener.h"
#include "response/inputfunctionresponse.h"
#include "response/powerresponse.h"
#include "response/displaydataresponse.h"
#include "response/mcaccnumberresponse.h"
#include "response/hdmipassthroughresponse.h"
#include "response/hdmicontrolresponse.h"
#include "response/hdmicontrolmoderesponse.h"
#include "response/hdmicontrolarcresponse.h"
#include "response/volumeresponse.h"
#include "response/muteresponse.h"
#include "response/inputnameresponse.h"
#include "response/errorresponse.h"
#include "response/phasecontrolresponse.h"
#include "response/hibitresponse.h"
#include "response/pqlscontrolresponse.h"
#include "response/soundretrieverresponse.h"
#include "response/eqresponse.h"
#include "response/toneresponse.h"
#include "response/bassresponse.h"
#include "response/trebleresponse.h"
#include "response/xcurveresponse.h"
#include "response/emphasisresponse.h"

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
