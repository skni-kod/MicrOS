#include "../time.h"

time_t mktime(tm * timeptr)
{
    uint32_t leap_years_count = ((timeptr->tm_year - 1970) - 2) / 4;
    
    timeptr->tm_wday = __time_get_day_of_week(timeptr->tm_mday, timeptr->tm_mon, timeptr->tm_year);
    timeptr->tm_yday = __time_get_day_of_year(timeptr->tm_mday, timeptr->tm_mon, timeptr->tm_year);
    
    return ((timeptr->tm_year - 1970) * 365 * 24 * 60 * 60) + 
           (leap_years_count * 24 * 60 * 60) +
           (timeptr->tm_yday * 24 * 60 * 60) +
           (timeptr->tm_hour * 60 * 60) +
           (timeptr->tm_min * 60) +
           (timeptr->tm_sec);
}