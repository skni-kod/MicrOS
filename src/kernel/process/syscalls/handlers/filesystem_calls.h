#ifndef FILESYSTEM_CALLS_H
#define FILESYSTEM_CALLS_H

#include "filesystems/filesystem.h"
#include "filesystems/generic/filesystem_file_info.h"
#include "filesystems/generic/filesystem_directory_info.h"
#include "memory/heap/heap.h"

void syscall_filesystem_get_file_info(interrupt_state *state);
void syscall_filesystem_get_directory_info(interrupt_state *state);
void syscall_filesystem_read_file(interrupt_state *state);
void syscall_filesystem_get_entries_count_in_directory(interrupt_state *state);
void syscall_filesystem_get_entries_in_directory(interrupt_state *state);
void syscall_filesystem_is_file(interrupt_state *state);
void syscall_filesystem_is_directory(interrupt_state *state);

void syscall_filesystem_create_file(interrupt_state *state);
void syscall_filesystem_create_directory(interrupt_state *state);
void syscall_filesystem_delete_file(interrupt_state *state);
void syscall_filesystem_delete_directory(interrupt_state *state);
void syscall_filesystem_rename_file(interrupt_state *state);
void syscall_filesystem_rename_directory(interrupt_state *state);
void syscall_filesystem_save_to_file(interrupt_state *state);
void syscall_filesystem_append_to_file(interrupt_state *state);

void syscall_filesystem_get_free_space(interrupt_state *state);
void syscall_filesystem_get_total_space(interrupt_state *state);

#endif