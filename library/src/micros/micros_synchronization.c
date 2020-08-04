#include "micros_synchronization.h"

binary_semaphore micros_binary_semaphore_create_named(char* name)
{
    return micros_interrupt_1a(0xE0, (uint32_t) name);
}

bool micros_binary_semaphore_acquire(binary_semaphore semaphore)
{
    return micros_interrupt_1a(0xE1, semaphore);
}

bool micros_binary_semaphore_try_acquire(binary_semaphore semaphore)
{
    return micros_interrupt_1a(0xE2, semaphore);
}

bool micros_binary_semaphore_release(binary_semaphore semaphore)
{
    return micros_interrupt_1a(0xE3, semaphore);
}

bool micros_binary_semaphore_destroy(binary_semaphore semaphore)
{
    return micros_interrupt_1a(0xE4, semaphore);
}

uint32_t micros_binary_semaphore_available(binary_semaphore semaphore)
{
    return micros_interrupt_1a(0xE5, semaphore);
}