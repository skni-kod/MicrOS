#include "filesystem_calls.h"

void syscall_filesystem_get_file_info(interrupt_state *state)
{
    state->registers.eax = filesystem_get_file_info((char *)state->registers.ebx, (filesystem_file_info *)state->registers.ecx);
}

void syscall_filesystem_get_directory_info(interrupt_state *state)
{
    state->registers.eax = filesystem_get_directory_info((char *)state->registers.ebx, (filesystem_directory_info *)state->registers.ecx);
}

void syscall_filesystem_read_file(interrupt_state *state)
{
    state->registers.eax = filesystem_read_file(
        (char *)state->registers.ebx,
        (uint8_t *)state->registers.ecx,
        (uint32_t)state->registers.edx,
        (uint32_t)state->registers.esi);
}

void syscall_filesystem_get_entries_count_in_directory(interrupt_state *state)
{
    state->registers.eax = filesystem_get_entries_count_in_directory((char *)state->registers.ebx);
}

void syscall_filesystem_get_entries_in_directory(interrupt_state *state)
{
    state->registers.eax = filesystem_get_entries_in_directory((char *)state->registers.ebx, (char **)state->registers.ecx);
}

void syscall_filesystem_is_file(interrupt_state *state)
{
    state->registers.eax = filesystem_is_file((char *)state->registers.ebx);
}

void syscall_filesystem_is_directory(interrupt_state *state)
{
    state->registers.eax = filesystem_is_directory((char *)state->registers.ebx);
}

void syscall_filesystem_create_file(interrupt_state *state)
{
    state->registers.eax = filesystem_create_file((char *)state->registers.ebx);
}

void syscall_filesystem_create_directory(interrupt_state *state)
{
    state->registers.eax = filesystem_create_directory((char *)state->registers.ebx);
}

void syscall_filesystem_delete_file(interrupt_state *state)
{
    state->registers.eax = filesystem_delete_file((char *)state->registers.ebx);
}

void syscall_filesystem_delete_directory(interrupt_state *state)
{
    state->registers.eax = filesystem_delete_directory((char *)state->registers.ebx);
}

void syscall_filesystem_rename_file(interrupt_state *state)
{
    state->registers.eax = filesystem_rename_file((char *)state->registers.ebx, (char *)state->registers.ecx);
}

void syscall_filesystem_rename_directory(interrupt_state *state)
{
    state->registers.eax = filesystem_rename_directory((char *)state->registers.ebx, (char *)state->registers.ecx);
}

void syscall_filesystem_save_to_file(interrupt_state *state)
{
    state->registers.eax = filesystem_save_to_file((char *)state->registers.ebx, (char *)state->registers.ecx, (int)state->registers.edx);
}

void syscall_filesystem_append_to_file(interrupt_state *state)
{
    state->registers.eax = filesystem_append_to_file((char *)state->registers.ebx, (char *)state->registers.ecx, (int)state->registers.edx);
}

void syscall_filesystem_get_free_space(interrupt_state *state)
{
    state->registers.eax = filesystem_get_free_space((char)state->registers.ebx);
}

void syscall_filesystem_get_total_space(interrupt_state *state)
{
    state->registers.eax = filesystem_get_total_space((char)state->registers.ebx);
}