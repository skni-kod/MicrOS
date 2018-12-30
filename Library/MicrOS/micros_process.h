#ifndef MICROS_PROCESS_H
#define MICROS_PROCESS_H

#include "micros_interrupts.h"

void micros_process_exit(int status);
uint32_t micros_process_get_processes_count();

#endif