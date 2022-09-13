#ifndef MICROS_TIMER_H
#define MICROS_TIMER_H

#include <stdint.h>
#include "micros_interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Gets current system clock
/*!
    Gets current system clock (milliseconds from MicrOS startup).
    \return Milliseconds from MicrOS startup.
*/
uint32_t micros_timer_get_system_clock();

#ifdef __cplusplus
}
#endif

#endif