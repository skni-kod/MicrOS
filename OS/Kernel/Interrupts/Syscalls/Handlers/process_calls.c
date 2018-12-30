#include "process_calls.h"

void syscall_process_exit(interrupt_state *state)
{
    process_manager_close_current_process(state);
}

void syscall_process_get_processes_count(interrupt_state *state)
{
    state->registers.eax = process_manager_get_processes_count();
}

void syscall_process_get_process_info(interrupt_state *state)
{
    state->registers.eax = process_manager_get_process_user_info(state->registers.ebx, state->registers.ecx);
}