#ifndef SYNCHRONIZATION_SYSCALL_H
#define SYNCHRONIZATION_SYSCALL_H

#include "cpu/idt/state/interrupt_state.h"
#include "process/synchronization/binary_semaphore.h"

void syscall_binary_semaphore_create_named(interrupt_state *state);
void syscall_binary_semaphore_acquire(interrupt_state *state);
void syscall_binary_semaphore_try_acquire(interrupt_state *state);
void syscall_binary_semaphore_release(interrupt_state *state);
void syscall_binary_semaphore_destroy(interrupt_state *state);
void syscall_binary_semaphore_available(interrupt_state *state);

#endif