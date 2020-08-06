#ifndef SYSCALLS_MANAGER_H
#define SYSCALLS_MANAGER_H

#define SYSCALLS_MANAGER_MAX_HANDLERS 256

#include "cpu/idt/idt.h"
#include "handlers/heap_calls.h"
#include "handlers/time_calls.h"
#include "handlers/terminal_calls.h"
#include "handlers/pc_speaker_calls.h"
#include "handlers/rtc_calls.h"
#include "handlers/filesystem_calls.h"
#include "handlers/process_calls.h"
#include "handlers/memory_calls.h"
#include "handlers/keyboard_calls.h"
#include "handlers/generic_vga.h"
#include "handlers/partitions_calls.h"
#include "handlers/power_calls.h"
#include "handlers/serial_calls.h"

void syscalls_manager_init();
void syscalls_manager_attach_handler(uint8_t function_number, void (*handler)(interrupt_state *state));
void syscalls_manager_detach_handler(uint8_t function_number);
void syscalls_manager_call(interrupt_state *state);

#endif