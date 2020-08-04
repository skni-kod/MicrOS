#ifndef MICROS_SYNCHRONIZATION_H
#define MICROS_SYNCHRONIZATION_H

#include <stdbool.h>
#include "micros_interrupts.h"

typedef uint32_t binary_semaphore;

#ifdef __cplusplus
extern "C" {
#endif

binary_semaphore micros_binary_semaphore_create_named(char* name);

bool micros_binary_semaphore_acquire(binary_semaphore semaphore);

bool micros_binary_semaphore_try_acquire(binary_semaphore semaphore);

bool micros_binary_semaphore_release(binary_semaphore semaphore);

bool micros_binary_semaphore_destroy(binary_semaphore semaphore);

uint32_t micros_binary_semaphore_available(binary_semaphore semaphore);

#ifdef __cplusplus
}
#endif

#endif