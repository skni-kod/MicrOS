#include "micros_process.h"

void micros_process_exit(int status)
{
    micros_interrupt_1a(0x90, status);
}

uint32_t micros_process_get_processes_count()
{
    return micros_interrupt_0a(0x91);
}

void micros_process_get_current_process_info(micros_process_user_info *user_info)
{
    micros_interrupt_1a(0x92, (uint32_t)user_info);
}

bool micros_process_get_process_info(uint32_t id, micros_process_user_info *user_info)
{
    return micros_interrupt_2a(0x93, id, (uint32_t)user_info);
}

void micros_process_get_all_processes_info(micros_process_user_info *user_info)
{
    micros_interrupt_1a(0x94, (uint32_t)user_info);
}

void micros_process_set_current_process_name(char *name)
{
    micros_interrupt_1a(0x95, (uint32_t)name);
}

void micros_process_current_process_sleep(uint32_t milliseconds)
{
    micros_interrupt_1a(0x96, milliseconds);
}

void micros_process_start_process(char *path, char *arguments)
{
    micros_interrupt_2a(0x97, (uint32_t)path, (uint32_t)arguments);
}

bool micros_process_set_current_process_signal_handler(void (*signal_handler)(int))
{
    micros_interrupt_1a(0x98, (uint32_t)signal_handler);
}