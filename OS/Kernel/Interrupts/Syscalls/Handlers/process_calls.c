#include "process_calls.h"

void syscall_process_exit(interrupt_state *state)
{
    process_manager_close_current_process(state);
}

void syscall_process_get_processes_count(interrupt_state *state)
{
    state->registers.eax = process_manager_get_processes_count();
}

void syscall_process_get_current_process_info(interrupt_state *state)
{
    process_manager_get_current_process_user_info((process_user_info *)state->registers.ebx);
}

void syscall_process_get_process_info(interrupt_state *state)
{
    state->registers.eax = process_manager_get_process_user_info(state->registers.ebx, (process_user_info *)state->registers.ecx);
}

void syscall_process_get_all_processes_info(interrupt_state *state)
{
    process_manager_get_all_processes_user_info((process_user_info *)state->registers.ebx);
}

void syscall_process_set_current_process_name(interrupt_state *state)
{
    state->registers.eax = process_manager_set_current_process_name((char *)state->registers.ebx);
}

void syscall_process_current_process_sleep(interrupt_state *state)
{
    process_manager_current_process_sleep(state->registers.ebx);
}

void syscall_process_start_process(interrupt_state *state)
{
    uint32_t parent_id = (bool)state->registers.edx ? process_manager_get_current_process()->id : process_manager_get_root_process();
    process_manager_create_process((char *)state->registers.ebx, (char *)state->registers.ecx, parent_id);
}

bool syscall_process_set_current_process_signal_handler(interrupt_state *state)
{
    process_manager_set_current_process_signal_handler(state->registers.ebx);
}

void syscall_process_finish_signal_handler(interrupt_state *state)
{
    process_manager_finish_signal_handler(state->registers.ebx);
}