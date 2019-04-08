#ifndef FILESYSTEM_CALLS_H
#define FILESYSTEM_CALLS_H

#include "FileSystems/filesystem.h"
#include "FileSystems/Generic/filesystem_file_info.h"
#include "FileSystems/Generic/filesystem_directory_info.h"
#include "Memory/Manager/Heap/heap.h"

void syscall_filesystem_get_file_info(interrupt_state *state);
void syscall_filesystem_get_directory_info(interrupt_state *state);
void syscall_filesystem_read_file(interrupt_state *state);
void syscall_filesystem_get_entries_count_in_directory(interrupt_state *state);
void syscall_filesystem_get_entries_in_directory(interrupt_state *state);
void syscall_filesystem_is_file(interrupt_state *state);
void syscall_filesystem_is_directory(interrupt_state *state);

#endif