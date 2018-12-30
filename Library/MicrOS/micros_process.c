#include "micros_process.h"

void micros_process_exit(int status)
{
    micros_interrupt_1a(0x90, status);
}

uint32_t micros_process_get_processes_count()
{
    return micros_interrupt_0a(0x91);
}

bool micros_process_get_process_info(uint32_t id, micros_process_user_info *user_info)
{
    return micros_interrupt_2a(0x92, id, user_info);
}