#ifndef MICROS_PROCESS_H
#define MICROS_PROCESS_H

#include <stdbool.h>
#include "micros_interrupts.h"

//! Process status retrieved from the kernel
typedef enum micros_process_status
{
    //! Process is ready to work and waits in queue
    micros_process_status_ready,
    
    //! Process is currently working
    micros_process_status_working,
    
    //! Process has called sleep function and is stopped for the specified amount of time
    micros_process_status_waiting_sleep,
    
    //! Process has called blocking keyboard function and waits for any pressed key
    micros_process_status_waiting_key_press,
    
    //! Process waits for the end of the another process (like shell is waiting for the end of the child process)
    micros_process_status_waiting_for_process
} micros_process_status;

//! Process information retrieved from the kernel
typedef struct micros_process_user_info
{
    //! Process ID
    uint32_t id;
    
    //! Parent process ID
    uint32_t parent_id;
    
    //! Process name (ended with 0)
    char name[32];

    //! Current process status
    micros_process_status status;
    
    //! Current process CPU usage calculated by the kernel for the last second
    float cpu_usage;
    
    //! Current process memory usage calculated by the kernel
    uint32_t memory_usage;
} __attribute__((packed)) micros_process_user_info;

//! Process registers state
typedef struct micros_signal_registers_state
{
    //! EDI register state
    uint32_t edi;
    
    //! ESI register state
    uint32_t esi;
    
    //! EBP (base pointer) register state
    uint32_t ebp;
    
    //! ESP (stack pointer) register state, contains invalid address so not really usable
    uint32_t esp_unused;
    
    //! EBX register state
    uint32_t ebx;
    
    //! EDX register state
    uint32_t edx;
    
    //! ECX register state
    uint32_t ecx;
    
    //! EAX register state
    uint32_t eax;
} micros_signal_registers_state;

//! Process FPU state
typedef struct micros_signal_fpu_state
{
    //! FPU control word
    uint32_t control_word;
    
    //! FPU status word
    uint32_t status_word;
    
    //! FPU tag word
    uint32_t tag_word;
    
    //! FPU instruction pointer offset
    uint32_t instruction_pointer_offset;
    
    //! FPU instruction pointer selector
    uint16_t instruction_pointer_selector;
    
    //! FPU opcode
    uint16_t opcode;
    
    //! FPU operand pointer offset
    uint32_t operand_pointer_offset;
    
    //! FPU operand pointer selector
    uint32_t operand_pointer_selector;
    
    //! FPU all registers state
    uint8_t registers[80];
} micros_signal_fpu_state;

//! Structure containig data passed when the signal is executed (from the point where the last instruction was executed before signal)
typedef struct micros_signal_params
{
    //! Number of the interrupt which caused the signal
    uint32_t interrupt_number;
    
    //! CR2 register state (contains address which caused page fault)
    uint32_t cr2;

    //! Saved registers state 
    micros_signal_registers_state registers;
    
    //! Saved FPU registers state
    micros_signal_fpu_state fpu_state;

    //! EIP (index pointer) register state
    uint32_t eip;
    
    //! CS (code segment) register state
    uint32_t cs;
    
    //! EFLAGS register state
    uint32_t eflags;
    
    //! ESP (stack pointer) register state
    uint32_t esp;
    
    //! SS (stack segment) register state
    uint32_t ss;
} micros_signal_params;

#ifdef __cplusplus
extern "C" {
#endif

void micros_process_exit(int status);
uint32_t micros_process_get_processes_count();
void micros_process_get_current_process_info(micros_process_user_info *user_info);
bool micros_process_get_process_info(uint32_t id, micros_process_user_info *user_info);
void micros_process_get_all_processes_info(micros_process_user_info *user_info);
void micros_process_set_current_process_name(char *name);
void micros_process_current_process_sleep(uint32_t milliseconds);
uint32_t micros_process_start_process(char *path, char *arguments, bool child, bool active);
void micros_process_set_current_process_signal_handler(void (*signal_handler)(micros_signal_params*));
void micros_process_finish_signal_handler(micros_signal_params *old_state);
void micros_process_wait_for_process(uint32_t process_id_to_wait);

#ifdef __cplusplus
}
#endif

#endif