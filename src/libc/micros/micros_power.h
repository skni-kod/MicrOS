#ifndef MICROS_POWER_H
#define MICROS_POWER_H

#include <stdint.h>
#include "micros_interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Reboots operating system
/*!
    Reboots operating system by calling PS/2 controller.
*/
void micros_power_reboot();

//! Shutdowns operating system
/*!
    Shutdowns operating system (works only in QEMU, VirtualBox and Bochs).
*/
void micros_power_shutdown();

#ifdef __cplusplus
}
#endif

#endif