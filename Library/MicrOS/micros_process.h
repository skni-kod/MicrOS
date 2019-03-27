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

typedef struct micros_signal_registers_state
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_unused;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
} micros_signal_registers_state;

typedef struct micros_signal_fpu_state
{
    uint32_t control_word;
    uint32_t status_word;
    uint32_t tag_word;
    uint32_t instruction_pointer_offset;
    uint16_t instruction_pointer_selector;
    uint16_t opcode;
    uint32_t operand_pointer_offset;
    uint32_t operand_pointer_selector;
    uint8_t registers[80];
} micros_signal_fpu_state;

typedef struct micros_signal_params
{
    uint32_t interrupt_number;
    uint32_t cr2;

    micros_signal_registers_state registers;
    micros_signal_fpu_state fpu_state;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
} micros_signal_params;

void micros_process_exit(int status);
uint32_t micros_process_get_processes_count();
void micros_process_get_current_process_info(micros_process_user_info *user_info);
bool micros_process_get_process_info(uint32_t id, micros_process_user_info *user_info);
void micros_process_get_all_processes_info(micros_process_user_info *user_info);
void micros_process_set_current_process_name(char *name);
void micros_process_current_process_sleep(uint32_t milliseconds);
void micros_process_start_process(char *path, char *arguments);
bool micros_process_set_current_process_signal_handler(void (*signal_handler)(int));
void micros_process_finish_signal_handler(micros_signal_params *old_state);

#endif