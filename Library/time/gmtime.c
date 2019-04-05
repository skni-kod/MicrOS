#include "../time.h"

extern tm converted_time;

tm *gmtime(const time_t *timer)
{
    // Source of algorithm: http://ptspts.blogspot.com/2009/11/how-to-convert-unix-timestamp-to-civil.html
    
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