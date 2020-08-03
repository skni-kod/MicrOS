#include "binary_semaphore.h"

uint32_t next_binary_semaphore_id = 0;
kvector binary_semaphores;

void binary_semaphore_init()
{
    kvector_init(&binary_semaphores);
}

binary_semaphore create_named_binary_semaphore(char* name)
{
    binary_semaphore semaphore_index;    
    if(__get_index_of_binary_semaphore_by_name(&binary_semaphores, &semaphore_index, name) == true)
    {
        binary_semaphore_data* sem = (binary_semaphore_data*)(binary_semaphores.data[semaphore_index]);
        // Add process id to vector
        uint32_t* process_id = heap_kernel_alloc(sizeof(uint32_t), 0);
        *process_id = process_manager_get_current_process()->id;
        kvector_add(&sem->processes, &process_id);

        return sem->semaphore_id;
    }
    else
    {
        binary_semaphore_data* new_semaphore = heap_kernel_alloc(sizeof(binary_semaphore_data), 0);
        new_semaphore->blocked = 0;
        new_semaphore->semaphore_id = next_binary_semaphore_id++;
        kvector_init(&(new_semaphore->processes));
        kvector_init(&(new_semaphore->blocked_processes));

        // Copy name
        size_t size = strlen(name);
        new_semaphore->name = heap_kernel_alloc(sizeof(char) * (size + 1), 0);
        strcpy(new_semaphore->name, name);

        // Add process id to vector
        uint32_t* process_id = heap_kernel_alloc(sizeof(uint32_t), 0);
        *process_id = process_manager_get_current_process()->id;
        kvector_add(&new_semaphore->processes, &process_id);

        // Add new semaphore to vector
        kvector_add(&binary_semaphores, new_semaphore);
        return new_semaphore->semaphore_id;
    }
}

void acquire(binary_semaphore semaphore)
{
    binary_semaphore semaphore_index;
    if(__get_index_of_binary_semaphore_by_id(&binary_semaphores, &semaphore_index, semaphore) == true)
    {
        // This ends when process gets semaphore. Otherwise it's blocked until it's get semaphore
        while(true)
        {
            binary_semaphore_data* sem = (binary_semaphore_data*)(binary_semaphores.data[semaphore_index]);
            int32_t value = 0;
            __asm__ (
            "movl $0x00, %%eax \n" \
            "movl $0x01, %%edx \n" \
            "cmpxchgl %%edx, %0 \n" \
            "movl %%eax, %1"
            :: "m"(sem->blocked), "m"(value)
            : "%eax", "%edx");
            if(value == 0)
            {
                // You got semaphore
                return;
            }
            else
            {
                // Block proccess
                // Add blocked process to list
                uint32_t* process_id = heap_kernel_alloc(sizeof(uint32_t), 0);
                *process_id = process_manager_get_current_process()->id;
                kvector_add(&sem->blocked_processes, &process_id);
            }
        }
        
    }
    else
    {
        // Trying to acquire non-existing semaphore, nice try
        return;
    }
}

bool __get_index_of_binary_semaphore_by_name(kvector* vector, uint32_t* index, char* name)
{
    for(uint32_t i = 0; i < vector->count; i++)
    {
        if(strcmp(((binary_semaphore_data*)(vector->data[i]))->name, name) == 0)
        {
            *index = i;
            return true;
        }
    }
    return false;
}

bool __get_index_of_binary_semaphore_by_id(kvector* vector, uint32_t* index, binary_semaphore id)
{
    for(uint32_t i = 0; i < vector->count; i++)
    {
        if(((binary_semaphore_data*)(vector->data[i]))->semaphore_id == id)
        {
            *index = i;
            return true;
        }
    }
    return false;
}