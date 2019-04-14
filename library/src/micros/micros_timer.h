#ifndef MICROS_TIMER_H
#define MICROS_TIMER_H

#include <stdint.h>
#include "micros_interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t micros_timer_get_system_clock();

#ifdef __cplusplus
}
#endif

#endif