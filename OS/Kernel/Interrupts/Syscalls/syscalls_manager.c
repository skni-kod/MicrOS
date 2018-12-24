#include "syscalls_manager.h"

void (*syscall_handlers[SYSCALLS_MANAGER_MAX_HANDLERS])(interrupt_state *state);

void syscalls_manager_init()
{
    // 0x0X - Heap
    syscalls_manager_attach_handler(0x00, syscall_alloc_memory_call);
    syscalls_manager_attach_handler(0x01, syscall_realloc_memory_call);
    syscalls_manager_attach_handler(0x02, syscall_dealloc_memory_call);
    syscalls_manager_attach_handler(0x03, syscall_verify_heap);

    // 0x1X - VGA

    // 0x2X - Keyboard

    // 0x3X - PCI

    // 0x4X - RTC

    // 0x5X - File system

    // 0x6X - Timer
    syscalls_manager_attach_handler(0x60, syscall_get_system_clock_call);

    // 0x7X - Power

    // 0x8X - PC Speaker
}

void syscalls_manager_attach_handler(uint8_t function_number, void (*handler)())
{
    syscall_handlers[function_number] = handler;
}

void syscalls_manager_detach_handler(uint8_t function_number)
{
    syscall_handlers[function_number] = 0;
}

void syscalls_manager_call(interrupt_state *state)
{
    if (syscall_handlers[state->registers.eax] != 0)
    {
        (*syscall_handlers[state->registers.eax])(state);
    }
}