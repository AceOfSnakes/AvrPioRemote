#ifndef RESPONSELISTENER_H
#define RESPONSELISTENER_H

#include "receivedobjectbase.h"
class ResponseListener
{
public:
    ResponseListener();
    ~ResponseListener();
    virtual void ResponseReceived(ReceivedObjectBase*) = 0;
};

#endif // RESPONSELISTENER_H
