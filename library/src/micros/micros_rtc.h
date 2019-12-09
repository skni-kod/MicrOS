#ifndef MICROS_RTC_H
#define MICROS_RTC_H

#include <stdint.h>
#include "micros_interrupts.h"

//! RTC time structure
typedef struct micros_rtc_time
{
    //! Seconds in RTC time
    unsigned char second;
    
    //! Minutes in RTC time
    unsigned char minute;
    
    //! Hours in RTC time
    unsigned char hour;
    
    //! Days in RTC time
    unsigned char day;
    
    //! Months in RTC time
    unsigned char month;
    
    //! Years in RTC time
    unsigned int year;
} micros_rtc_time;

#ifdef __cplusplus
extern "C" {
#endif

//! Gets current system clock
/*!
    Gets current system clock (milliseconds from MicrOS startup).
    \param time Structure where the read time will be stored.
    \return Milliseconds from MicrOS startup.
*/
void micros_rtc_read_time(micros_rtc_time *time);

//! Gets current system clock
/*!
    Gets current system clock (milliseconds from MicrOS startup).
    \param time Structure which contains time to store in the RTC.
    \return Milliseconds from MicrOS startup.
*/
void micros_rtc_set_time(micros_rtc_time *time);

#ifdef __cplusplus
}
#endif

#endif