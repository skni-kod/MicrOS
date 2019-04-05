#include "time.h"

tm converted_time;

clock_t clock()
{
    return micros_timer_get_system_clock();
}

double difftime(time_t end, time_t beginning)
{
    return end - beginning;
}

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

tm *gmtime(const time_t *timer)
{
    uint32_t s = *timer % 86400;
    uint32_t ts = *timer / 86400;
    uint32_t h = s / 3600;
    uint32_t m = s / 60 % 60;
    s = s % 60;
    uint32_t x = (ts * 4 + 102032) / 146097 + 15;
    uint32_t b = ts + 2442113 + x - (x / 4);
    uint32_t c = (b * 20 - 2442) / 7305;
    uint32_t d = b - 365 * c - c / 4;
    uint32_t e = d * 1000 / 30601;
    uint32_t f = d - e * 30 - e * 601 / 1000;
    
    if(e < 13)
    {
        converted_time.tm_year = c - 4716;
        converted_time.tm_mon = e - 1;
        converted_time.tm_mday = f;
        converted_time.tm_hour = h;
        converted_time.tm_min = m;
        converted_time.tm_sec = s;
    }
    else
    {
        converted_time.tm_year = c - 4715;
        converted_time.tm_mon = e - 13;
        converted_time.tm_mday = f;
        converted_time.tm_hour = h;
        converted_time.tm_min = m;
        converted_time.tm_sec = s;
    }
    
    converted_time.tm_wday = __time_get_day_of_week(converted_time.tm_mday, converted_time.tm_mon, converted_time.tm_year);
    converted_time.tm_yday = __time_get_day_of_year(converted_time.tm_mday, converted_time.tm_mon, converted_time.tm_year);
    
    return &converted_time;
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