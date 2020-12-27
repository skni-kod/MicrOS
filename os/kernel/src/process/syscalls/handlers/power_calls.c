#include "power_calls.h"

void syscall_power_reboot(interrupt_state *state)
{
    power_reboot();
}

void syscall_power_shutdown(interrupt_state *state)
{
    power_shutdown();
}