#include <sys/time.h>
#include "OpQueue.hh"
#include "OpThread.hh"
#include "Operation.hh"



OpQueue::OpQueue() : pending(), max_threads(16)
{
    pthread_mutex_init(&theMutex, 0);
    pthread_cond_init(&theCond, 0);
}



OpQueue::~OpQueue()
{
    pthread_mutex_destroy(&theMutex);
    pthread_cond_destroy(&theCond);
}



Operation* OpQueue::waitForOperation(OpThread* th)
{
    Operation* op = NULL;
    bool isStopping = false;

    pthread_mutex_lock(&theMutex);
    while(!isStopping && pending.size() == 0) {
        pthread_cond_wait(&theCond, &theMutex);
        isStopping = (th->getStatus() == STOPPING);
    }
    if (!isStopping) {
        op = pending.front();
        pending.pop_front();
    }
    running.push_back(op);
    pthread_mutex_unlock(&theMutex);
    return op;
}



void OpQueue::operationFinished(Operation* op)
{
    pthread_mutex_lock(&theMutex);
    running.remove(op);
    pthread_cond_broadcast(&theCond);
    pthread_mutex_unlock(&theMutex);
}



void OpQueue::growOrPrunePool()
{
    OpThread* mythread;

    // If threadpool is too small, spawn some threads.
    while (threadpool.size() < max_threads) {
        mythread = new OpThread(this);
        threadpool.push_back(mythread);
    }

    // If threadpool is too big, ask some threads to terminate
    //  when they next aren't busy.
    bool didChange = false;
    while (threadpool.size() > max_threads) {
        mythread = threadpool.front();
        mythread->requestTermination();
        threadpool.pop_front();
        didChange = true;
    }
    // Let terminated threads wake up so they can finish.
    if (didChange) {
        pthread_mutex_lock(&theMutex);
        pthread_cond_broadcast(&theCond);
        pthread_mutex_unlock(&theMutex);
    }
}



void OpQueue::addOperation(Operation *op)
{
    pthread_mutex_lock(&theMutex);
    pending.push_back(op);
    pthread_cond_broadcast(&theCond);
    pthread_mutex_unlock(&theMutex);
    growOrPrunePool();
}



void OpQueue::waitUntilAllOperationsAreFinished()
{
    pthread_mutex_lock(&theMutex);
    while(pending.size() > 0) {
        pthread_cond_wait(&theCond, &theMutex);
    }
    while(running.size() > 0) {
        pthread_cond_wait(&theCond, &theMutex);
    }
    pthread_mutex_unlock(&theMutex);
}



int OpQueue::waitUntilAllOperationsAreFinishedOrTimeout(int microstimeout)
{
    struct timeval tv;
    struct timespec ts;
    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = (tv.tv_usec + microstimeout) * 1000;
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_sec += 1;
        ts.tv_nsec -= 1000000000;
    }

    pthread_mutex_lock(&theMutex);
    int res;
    while(pending.size() > 0) {
        if ((res = pthread_cond_timedwait(&theCond, &theMutex, &ts)) != 0) {
            pthread_mutex_unlock(&theMutex);
            return res;
        }
    }
    while(running.size() > 0) {
        if ((res = pthread_cond_timedwait(&theCond, &theMutex, &ts)) != 0) {
            pthread_mutex_unlock(&theMutex);
            return res;
        }
    }
    pthread_mutex_unlock(&theMutex);
    return 0;
}



void OpQueue::setMaxConcurrentOperationCount(unsigned int maxcnt)
{
    max_threads = maxcnt;
    growOrPrunePool();
}



int OpQueue::operationsRemaining()
{
    pthread_mutex_lock(&theMutex);
    int count = running.size() + pending.size();
    pthread_mutex_unlock(&theMutex);
    return count;
}


// vim: set ts=4 sw=4 nowrap expandtab: settings

