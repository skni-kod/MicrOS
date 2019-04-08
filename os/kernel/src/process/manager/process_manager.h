#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <stdlib.h>
#include <kvector.h>
#include "process_info.h"
#include "process_user_info.h"
#include "memory/paging/paging.h"
#include "process/elf/parser/elf_header.h"
#include "process/elf/Loader/elf_loader.h"
#include "filesystems/fat/fat.h"
#include "assembly/io.h"
#include "interrupts/idt/idt.h"
#include "interrupts/signals/signals_manager.h"
#include "interrupts/signals/signal_params.h"
#include "memory/manager/heap/heap.h"

void process_manager_init();
uint32_t process_manager_create_process(char *path, char *parameters, uint32_t parent_id);
process_info *process_manager_get_process(uint32_t process_id);
process_info *process_manager_get_current_process();

void process_manager_set_root_process(uint32_t process_id);
uint32_t process_manager_get_root_process();

void process_manager_save_current_process_state(interrupt_state *state, uint32_t delta);
void process_manager_switch_to_next_process();
void process_manager_close_current_process();
void process_manager_close_process(uint32_t process_id);
uint32_t process_manager_get_processes_count();
uint32_t process_manager_get_process_index(uint32_t process_id);
process_info *process_manager_get_process_info(uint32_t id);
void process_manager_get_current_process_user_info(process_user_info *user_info);
bool process_manager_get_process_user_info(uint32_t id, process_user_info *user_info);
void process_manager_get_all_processes_user_info(process_user_info *user_info);
bool process_manager_set_current_process_name(char *name);
bool process_manager_set_current_process_signal_handler(void (*signal_handler)(int));
void process_manager_finish_signal_handler(signal_params *old_state);

void process_manager_current_process_sleep(uint32_t milliseconds);
void process_manager_current_process_wait_for_key_press();

void process_manager_convert_process_info_to_user_info(process_info *process, process_user_info *user_info);
uint32_t process_manager_get_process_memory_usage(process_info *process);

void process_manager_interrupt_handler(interrupt_state *state);
void process_manager_keyboard_interrupt_handler(interrupt_state *state);
void process_manager_run();

#endif