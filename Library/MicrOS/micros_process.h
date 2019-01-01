#ifndef MICROS_PROCESS_H
#define MICROS_PROCESS_H

#include <stdbool.h>
#include "micros_interrupts.h"

typedef enum micros_process_status
{
    micros_process_status_ready,
    micros_process_status_working,
    micros_process_status_waiting_sleep
} micros_process_status;

typedef struct micros_process_user_info
{
    uint32_t id;
    char name[32];

    micros_process_status status;
    float cpu_usage;
    uint32_t memory_usage;
} __attribute__((packed)) micros_process_user_info;

void micros_process_exit(int status);
uint32_t micros_process_get_processes_count();
void micros_process_get_current_process_info(micros_process_user_info *user_info);
bool micros_process_get_process_info(uint32_t id, micros_process_user_info *user_info);
void micros_process_get_all_processes_info(micros_process_user_info *user_info);
void micros_process_set_current_process_name(char *name);
void micros_process_current_process_sleep(uint32_t milliseconds);

#endif