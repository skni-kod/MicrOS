#include "../time.h"

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