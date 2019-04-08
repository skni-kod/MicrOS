#ifndef FILESYSTEM_CALLS_H
#define FILESYSTEM_CALLS_H

#include "fileSystems/filesystem.h"
#include "FileSystems/generic/filesystem_file_info.h"
#include "FileSystems/generic/filesystem_directory_info.h"
#include "memory/manager/heap/heap.h"

void syscall_filesystem_get_file_info(interrupt_state *state);
void syscall_filesystem_get_directory_info(interrupt_state *state);
void syscall_filesystem_read_file(interrupt_state *state);
void syscall_filesystem_get_entries_count_in_directory(interrupt_state *state);
void syscall_filesystem_get_entries_in_directory(interrupt_state *state);
void syscall_filesystem_is_file(interrupt_state *state);
void syscall_filesystem_is_directory(interrupt_state *state);

#endif