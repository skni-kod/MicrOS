#ifndef SYSCALLS_MANAGER_H
#define SYSCALLS_MANAGER_H

#define SYSCALLS_MANAGER_MAX_HANDLERS 256

#include "../IDT/idt.h"
#include "Handlers/memory_calls.h"
#include "Handlers/time_calls.h"
#include "Handlers/vga_calls.h"
#include "Handlers/pc_speaker_calls.h"
#include "Handlers/rtc_calls.h"
#include "Handlers/filesystem_calls.h"
#include "Handlers/process_calls.h"

void syscalls_manager_init();
void syscalls_manager_attach_handler(uint8_t function_number, void (*handler)(interrupt_state *state));
void syscalls_manager_detach_handler(uint8_t function_number);
void syscalls_manager_call(interrupt_state *state);

#endif