#include "time.h"

clock_t clock()
{
    return micros_timer_get_system_clock();
}

time_t mktime(tm * timeptr)
{
    static const int days[2][13] = {
        {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
        {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}
    };
    
    uint32_t leap = (timeptr->tm_year % 4 == 0 && timeptr->tm_year % 100 != 0) || (timeptr->tm_year % 400 == 0);
    uint32_t leap_years_count = ((timeptr->tm_year - 1970) - 2) / 4;
    uint32_t day_of_year = days[leap][timeptr->tm_mon] + timeptr->tm_mday;
    
    return ((timeptr->tm_year - 1970) * 365 * 24 * 60 * 60) + 
           (leap_years_count * 24 * 60 * 60) +
           (day_of_year * 24 * 60 * 60) +
           (timeptr->tm_hour * 60 * 60) +
           (timeptr->tm_min * 60) +
           (timeptr->tm_sec);
}

time_t time(time_t* timer)
{
    micros_rtc_time rtc_time;
    micros_rtc_read_time(&rtc_time);
    
    tm time;
    time.tm_year = rtc_time.year;
    time.tm_mon = rtc_time.month;
    time.tm_mday = rtc_time.day;
    time.tm_hour = rtc_time.hour;
    time.tm_min = rtc_time.minute;
    time.tm_sec = rtc_time.second;
    
    return mktime(&time);
}