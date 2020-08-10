#include "synchronization.h"

void syscall_binary_semaphore_create_named(interrupt_state *state)
{
    state->registers.eax = binary_semaphore_create_named((char*)state->registers.ebx);
}

void syscall_binary_semaphore_acquire(interrupt_state *state)
{
    state->registers.eax = binary_semaphore_acquire((binary_semaphore)state->registers.ebx);
}

void syscall_binary_semaphore_try_acquire(interrupt_state *state)
{
    state->registers.eax = binary_semaphore_try_acquire((binary_semaphore)state->registers.ebx);
}

void syscall_binary_semaphore_release(interrupt_state *state)
{
    state->registers.eax = binary_semaphore_release((binary_semaphore)state->registers.ebx);
}

void syscall_binary_semaphore_destroy(interrupt_state *state)
{
    state->registers.eax = binary_semaphore_destroy((binary_semaphore)state->registers.ebx);
}

void syscall_binary_semaphore_available(interrupt_state *state)
{
    state->registers.eax = binary_semaphore_available((binary_semaphore)state->registers.ebx);
}