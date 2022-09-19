#include "micros_process.h"

void micros_process_exit_process(int status)
{
#ifndef __libk 

    micros_interrupt_2a(0x0700, status, false);

#else

#endif
}

uint32_t micros_process_get_processes_count()
{
#ifndef __libk 

    return micros_interrupt_0a(0x0701);

#else

#endif
}

void micros_process_get_current_process_info(micros_process_user_info *user_info)
{
#ifndef __libk 

    micros_interrupt_1a(0x0702, (uint32_t)user_info);

#else

#endif
}

bool micros_process_get_process_info(uint32_t id, micros_process_user_info *user_info)
{
#ifndef __libk 

    return micros_interrupt_2a(0x0703, id, (uint32_t)user_info);

#else

#endif
}

void micros_process_get_all_processes_info(micros_process_user_info *user_info)
{
#ifndef __libk 

    micros_interrupt_1a(0x0704, (uint32_t)user_info);

#else

#endif
}

void micros_process_set_current_process_name(char *name)
{
#ifndef __libk 

    micros_interrupt_1a(0x0705, (uint32_t)name);

#else

#endif
}

void micros_process_current_process_sleep(uint32_t milliseconds)
{
#ifndef __libk 

    micros_interrupt_1a(0x0706, milliseconds);

#else

#endif
}

uint32_t micros_process_start_process(char *path, char *arguments, bool child, bool active)
{
#ifndef __libk 

    return micros_interrupt_4a(0x0707, (uint32_t)path, (uint32_t)arguments, (uint32_t)child, (uint32_t)active);

#else

#endif
}

void micros_process_set_current_process_signal_handler(void (*signal_handler)(micros_signal_params*))
{
#ifndef __libk 

    micros_interrupt_1a(0x0708, (uint32_t)signal_handler);

#else

#endif
}

void micros_process_finish_signal_handler(micros_signal_params *old_state)
{
#ifndef __libk 

    micros_interrupt_1a(0x0709, (uint32_t)old_state);

#else

#endif
}

void micros_process_wait_for_process(uint32_t process_id_to_wait)
{
#ifndef __libk 

    micros_interrupt_1a(0x070A, process_id_to_wait);

#else

#endif
}

uint32_t micros_process_start_thread(void *entry_point, void *param)
{
#ifndef __libk 

    uint32_t stack_size = 1024 * 1024 * 4;
    
    uint32_t *stack_area = (uint32_t *)malloc(stack_size);
    uint32_t *stack = stack_area + stack_size / 4;
    
    *(stack - 1) = (uint32_t)param;
    *(stack - 2) = (uint32_t)micros_process_abort_thread;
    
    return micros_interrupt_2a(0x070B, (uint32_t)entry_point, (uint32_t)stack);

#else

#endif
}

void micros_process_abort_thread()
{
#ifndef __libk 

    micros_interrupt_2a(0x0700, 0, true);

#else

#endif
}