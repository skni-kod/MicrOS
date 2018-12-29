#ifndef FILESYSTEM_TIME_H
#define FILESYSTEM_TIME_H

#include <stdint.h>

typedef struct filesystem_time
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
} filesystem_time;

#endif