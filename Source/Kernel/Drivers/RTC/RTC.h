#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include "../../Assembly/io.h"

typedef struct rtc_time
{
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day;
    unsigned char month;
    unsigned int year;
} rtc_time;

void read_rtc(rtc_time* time);
uint8_t read_second();
uint8_t read_minute();
uint8_t read_hour();
uint8_t read_day();
uint8_t read_month();
uint32_t read_year();

#endif