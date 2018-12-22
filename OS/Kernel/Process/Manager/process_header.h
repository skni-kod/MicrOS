#ifndef PROCESS_HEADER_H
#define PROCESS_HEADER_H

#include <stdbool.h>
#include "../../Interrupts/IDT/interrupt_state.h"

typedef struct process_header
{
    uint32_t id;
    char name[32];

    uint32_t size_in_memory;
    uint32_t base_heap_page_index;

    void *stack;
    void *page_directory;

    interrupt_state state;
} process_header;

#endif