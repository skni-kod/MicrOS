#include "micros_process.h"

void micros_process_exit(int status)
{
    micros_interrupt_1a(0x90, status);
}

uint32_t micros_process_get_processes_count()
{
    return micros_interrupt_0a(0x91);
}