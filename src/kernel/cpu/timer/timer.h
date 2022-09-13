#ifndef TIMER_H
#define TIMER_H

#define CHANNEL0_PORT 0x40
#define MODE_COMMAND_REGISTER 0x43

#define TIMER_FREQUENCY_HZ 1193182
#define TARGET_FREQUENCY 1000

#include "cpu/pic/pic.h"
#include "cpu/idt/idt.h"
#include "assembly/io.h"

void timer_init();
uint16_t timer_get_reload_value();
uint32_t timer_get_expected_clock_fraction();
uint32_t timer_get_system_clock();
bool timer_interrupt();

//! Function makes sleep by checking how much time elapsed.
/*!
    \param time Time for sleep in miliseonds.
*/
void timer_sleep(uint32_t time);

//! Measures if enought time elapsed from start.
/*!
    \param start_time Time from which we start count in miliseconds.
    \param time_to_elapsed Time in miliseonds that should elapsed since \a start_time.
    \return true if \a time_to_elapsed elapsed since \a start_time
*/
bool timer_does_elapsed(uint32_t start_time, uint32_t time_to_elapsed);

#endif