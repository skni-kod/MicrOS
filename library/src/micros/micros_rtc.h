#ifndef MICROS_RTC_H
#define MICROS_RTC_H

#include <stdint.h>
#include "micros_interrupts.h"

typedef struct micros_rtc_time
{
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day;
    unsigned char month;
    unsigned int year;
} micros_rtc_time;

#ifdef __cplusplus
extern "C" {
#endif

void micros_rtc_read_time(micros_rtc_time *time);

#ifdef __cplusplus
}
#endif

#endif