#include "time.h"

clock_t clock()
{
    return micros_timer_get_system_clock();
}

time_t mktime(tm * timeptr)
{
    uint32_t leap_years_count = ((timeptr->tm_year - 1970) - 2) / 4;
    
    timeptr->tm_wday = __time_get_day_of_week(timeptr->tm_mday + 4, timeptr->tm_mon, timeptr->tm_year);
    timeptr->tm_yday = __time_get_day_of_year(timeptr->tm_mday, timeptr->tm_mon, timeptr->tm_year);
    
    return ((timeptr->tm_year - 1970) * 365 * 24 * 60 * 60) + 
           (leap_years_count * 24 * 60 * 60) +
           (timeptr->tm_yday * 24 * 60 * 60) +
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
    
    time_t current_time = mktime(&time);
    if(timer != 0)
    {
        *timer = current_time;
    }
    
    return mktime(&time);
}

uint32_t __time_get_day_of_year(uint32_t day, uint32_t month, uint32_t year)
{
    static const int days[2][13] = {
        {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
        {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}
    };
    
    uint32_t leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    uint32_t day_of_year = days[leap][month] + day;
    
    return day_of_year;
}

uint32_t __time_get_day_of_week(uint32_t day, uint32_t month, uint32_t year)
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    year -= month < 3;
    
    return (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;
}