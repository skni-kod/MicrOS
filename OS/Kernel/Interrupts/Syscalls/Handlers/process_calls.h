#ifndef PROCESS_CALLS_H
#define PROCESS_CALLS_H

#include "../../../Process/Manager/process_manager.h"

void syscall_process_exit(interrupt_state *state);
void syscall_process_get_processes_count(interrupt_state *state);

#endif