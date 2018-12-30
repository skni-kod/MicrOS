#ifndef MICROS_PROCESS_H
#define MICROS_PROCESS_H

#include <stdbool.h>
#include "micros_interrupts.h"

typedef struct micros_process_user_info
{
    uint32_t id;
    char name[32];
} micros_process_user_info;

void micros_process_exit(int status);
uint32_t micros_process_get_processes_count();
bool micros_process_get_process_info(uint32_t id, micros_process_user_info *user_info);

#endif