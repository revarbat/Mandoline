#ifndef OPTHREAD_H
#define OPTHREAD_H

#include <pthread.h>
#include "Operation.hh"
#include "OpQueue.hh"

typedef enum {
    INIT,
    READY,
    BUSY,
    STOPPING
} OpThreadStatus;


class OpThread {
private:
    pthread_t theThread;
    pthread_mutex_t theMutex;
    OpQueue* parent;
    Operation* currentOp;
    OpThreadStatus status;
    volatile bool terminating;

public:
    OpThread(OpQueue* opQ);
    ~OpThread();

    OpThreadStatus getStatus();
    void requestTermination();
    void doWork();

private:
    void setStatus(OpThreadStatus stat);

};

#endif
// vim: set ts=4 sw=4 nowrap expandtab: settings

