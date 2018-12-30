#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <stdbool.h>
#include "../../Interrupts/IDT/interrupt_state.h"

typedef struct process_info
{
    uint32_t id;
    char name[32];

    uint32_t size_in_memory;
    uint32_t base_heap_page_index;

    void *user_stack;
    void *page_directory;

    interrupt_state state;
} process_info;

#endif