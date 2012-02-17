#include "OpThread.hh"


static void* start_thread(void *obj)
{
    //All we do here is call the do_work() function
    OpThread *mythread = reinterpret_cast<OpThread *>(obj);
    mythread->doWork();
    delete mythread;
    return 0;
}



OpThread::OpThread(OpQueue* opQ)
{
    parent = opQ;
    terminating = false;
    currentOp = NULL;
    status = INIT;
    pthread_mutex_init(&theMutex, 0);
    pthread_create(&theThread, 0, start_thread, this);
}



OpThread::~OpThread()
{
    pthread_mutex_destroy(&theMutex);
}



void OpThread::doWork()
{
    while (!terminating) {
        setStatus(READY);
        currentOp = parent->waitForOperation(this);
        if (currentOp) {
            setStatus(BUSY);
            currentOp->main();
            parent->operationFinished(currentOp);
        }
    }
}



void OpThread::setStatus(OpThreadStatus stat)
{
    pthread_mutex_lock(&theMutex);
    status = stat;
    pthread_mutex_unlock(&theMutex);
}



OpThreadStatus OpThread::getStatus()
{
    OpThreadStatus stat;
    pthread_mutex_lock(&theMutex);
    stat = status;
    pthread_mutex_unlock(&theMutex);
    return stat;
}


void OpThread::requestTermination()
{
    terminating = true;
    setStatus(STOPPING);
}


// vim: set ts=4 sw=4 nowrap expandtab: settings

