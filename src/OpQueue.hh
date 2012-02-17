#ifndef OPQUEUE_H
#define OPQUEUE_H

#include <list>
#include <pthread.h>
#include <errno.h>

class OpThread;
class Operation;

class OpQueue {
private:
    std::list<OpThread*> threadpool;
    pthread_mutex_t theMutex;
    pthread_cond_t theCond;

    std::list<Operation*> running;
    std::list<Operation*> pending;
    unsigned int max_threads;

public:
    OpQueue();
    ~OpQueue();

    void addOperation(Operation *op);
    void waitUntilAllOperationsAreFinished();
    int  waitUntilAllOperationsAreFinishedOrTimeout(int microstimeout);
    void setMaxConcurrentOperationCount(unsigned int maxcnt);

    Operation* waitForOperation(OpThread* th);
    void operationFinished(Operation* op);
    int  operationsRemaining();

private:
    void growOrPrunePool();
};

#endif
// vim: set ts=4 sw=4 nowrap expandtab: settings

