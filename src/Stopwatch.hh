#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <sys/time.h>

class Stopwatch {
public:
    struct timeval startTime;
    struct timeval phaseTime;

    Stopwatch() {
        startTime.tv_sec  = 0;
        startTime.tv_usec = 0;
        phaseTime.tv_sec  = 0;
        phaseTime.tv_usec = 0;
    }
    
    void start();
    void checkpoint(const char *phaseName);
    void finish();
};


#endif
// vim: set ts=4 sw=4 nowrap expandtab: settings

