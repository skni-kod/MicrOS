#ifndef TIME_H
#define TIME_H

#define CLOCKS_PER_SEC 1000

#include <stdint.h>
#include "micros.h"

typedef uint32_t clock_t;
typedef uint64_t time_t;

typedef struct tm
{
    uint32_t tm_sec;
    uint32_t tm_min;
    uint32_t tm_hour;
    uint32_t tm_mday;
    uint32_t tm_mon;
    uint32_t tm_year;
    uint32_t tm_wday;
    uint32_t tm_yday;
    uint32_t tm_isdst;
} tm;

tm converted_time;

#ifdef __cplusplus
extern "C" {
#endif

clock_t clock();
double difftime(time_t end, time_t beginning);
time_t mktime(tm * timeptr);
time_t time(time_t* timer);

tm *gmtime(const time_t *timer);

uint32_t __time_get_day_of_year(uint32_t day, uint32_t month, uint32_t year);
uint32_t __time_get_day_of_week(uint32_t day, uint32_t month, uint32_t year);

#ifdef __cplusplus
}
#endif

#endif