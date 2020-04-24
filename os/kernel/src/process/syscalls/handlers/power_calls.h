#ifndef POWER_CALLS_H
#define POWER_CALLS_H

#include "power/power.h"
#include "process/manager/process_manager.h"

void syscall_power_reboot(interrupt_state *state);
void syscall_power_shutdown(interrupt_state *state);

#endif