#include "../time.h"

uint32_t __time_get_day_of_week(uint32_t day, uint32_t month, uint32_t year)
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    year -= month < 3;
    
    return (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;
}