#ifndef BINARY_SEMAPHORE_H
#define BINARY_SEMAPHORE_H

#include <stdbool.h>
#include <kvector.h>
#include <string.h>
#include "memory/heap/heap.h"
#include "process/manager/process_manager.h"

typedef uint32_t binary_semaphore;

typedef struct binary_semaphore_data
{
	char* name;
    uint32_t semaphore_id;
    kvector processes;
	kvector blocked_processes;
    int32_t isBlocked;
} binary_semaphore_data;

void binary_semaphore_init();

binary_semaphore binary_semaphore_create_named(char* name);

bool binary_semaphore_acquire(binary_semaphore semaphore);

bool binary_semaphore_try_acquire(binary_semaphore semaphore);

bool binary_semaphore_release(binary_semaphore semaphore);

bool binary_semaphore_destroy(binary_semaphore semaphore);

uint32_t binary_semaphore_available(binary_semaphore semaphore);

bool __bs_get_index_of_binary_semaphore_by_name(kvector* vector, uint32_t* index, char* name);

bool __bs_get_index_of_binary_semaphore_by_id(kvector* vector, uint32_t* index, binary_semaphore id);

bool __bs_get_index_of_process_in_processes(kvector* vector, uint32_t* index, uint32_t process_id);

#endif