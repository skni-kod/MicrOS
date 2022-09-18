#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include "assembly/io.h"

typedef struct rtc_time
{
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day;
    unsigned char month;
    unsigned int year;
} rtc_time;

void rtc_read(rtc_time *time);
uint8_t rtc_read_second();
uint8_t rtc_read_minute();
uint8_t rtc_read_hour();
uint8_t rtc_read_day();
uint8_t rtc_read_month();
uint32_t rtc_read_year();

void rtc_set(rtc_time *time);
void rtc_enable_IRQ8();
void rtc_disable_IRQ8();
void rtc_change_interrupt_rate(uint8_t rate);

#endif
