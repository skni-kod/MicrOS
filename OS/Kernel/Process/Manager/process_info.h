#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <stdbool.h>
#include "../../Interrupts/IDT/interrupt_state.h"

typedef enum process_status
{
    process_status_ready,
    process_status_working,
    process_status_waiting_sleep
} process_status;

typedef struct process_info
{
    uint32_t id;
    char name[32];

    process_status status;
    uint32_t size_in_memory;

    void *user_stack;
    void *page_directory;
    void *heap;

    float cpu_usage;
    uint32_t sleep_deadline;

    interrupt_state state;
} process_info;

#endif