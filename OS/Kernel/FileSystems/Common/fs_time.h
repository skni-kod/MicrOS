#ifndef FS_TIME_H
#define FS_TIME_H

#include <stdint.h>

typedef struct fs_time
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
} fs_time;

#endif