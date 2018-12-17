#ifndef PROCESS_HEADER_H
#define PROCESS_HEADER_H

typedef struct process_header
{
    uint32_t id;
    char name[32];

    uint32_t size_in_memory;
    uint32_t base_heap_page_index;

    void *stack;
    void *page_directory;
} process_header;

#endif