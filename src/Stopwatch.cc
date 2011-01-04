#include <stdio.h>
#include "Stopwatch.h"



void Stopwatch::start()
{
    gettimeofday(&startTime, NULL);
    phaseTime = startTime;
}



void Stopwatch::checkpoint(const char *phaseName)
{
    static struct timeval now;
    gettimeofday(&now, NULL);
    long secs = now.tv_sec-phaseTime.tv_sec;
    long usecs = now.tv_usec-phaseTime.tv_usec;
    phaseTime = now;
    if (usecs < 0) {
        secs--;
        usecs += 1000000;
    }
    double dtime = secs + usecs/1.0e6;
    printf("%s in %.3f seconds\n", phaseName, dtime);
}



void Stopwatch::finish()
{
    static struct timeval now;
    gettimeofday(&now, NULL);
    long secs = now.tv_sec-startTime.tv_sec;
    long usecs = now.tv_usec-startTime.tv_usec;
    if (usecs < 0) {
        secs--;
        usecs += 1000000;
    }
    double dtime = secs + usecs/1.0e6;
    printf("Total elapsed time is %.3f seconds\n", dtime);
}



