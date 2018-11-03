#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include "../../Assembly/io.h"

unsigned char second;
unsigned char minute;
unsigned char hour;
unsigned char day;
unsigned char month;
unsigned int year;

void read_rtc();


#endif