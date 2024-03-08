#include "sleep.h"
#include <time.h>


void sleep_sec1(long sec)
{
    struct timespec sleep_time;
    sleep_time.tv_sec = (sec / 1000000);
    sleep_time.tv_nsec = (sec % 1000000) * 1000;
    nanosleep(&sleep_time,NULL);
}

void sleep_sec2(long sec)
{
    sleep_sec1(sec * 1000);
}