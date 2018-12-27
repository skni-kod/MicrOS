#include "filesystem_calls.h"

void syscall_filesystem_get_file_info(interrupt_state *state)
{
    state->registers.eax = filesystem_get_file_info((char *)state->registers.ebx, (filesystem_file_info *)state->registers.ecx);
}

void syscall_filesystem_get_directory_info(interrupt_state *state)
{
    state->registers.eax = filesystem_get_directory_info((char *)state->registers.ebx, (filesystem_directory_info *)state->registers.ecx);
}