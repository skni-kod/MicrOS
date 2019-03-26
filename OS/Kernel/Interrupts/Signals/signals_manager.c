#include "signals_manager.h"

extern void enter_user_space(interrupt_state *address);

void signals_manager_init()
{
    idt_attach_exception_handler(14, signals_page_fault);
}

void signals_page_fault(exception_state *state)
{
    process_info *info = process_manager_get_current_process();
    if (info->signal_handler != 0)
    {
        interrupt_state new_state = {0};
        new_state.registers.ecx = state->interrupt_number;
        new_state.registers.edx = state->cr2;
        new_state.eip = info->signal_handler;
        new_state.cs = state->cs;
        new_state.ss = state->ss;
        new_state.eflags = state->eflags;
        new_state.esp = state->esp;

        enter_user_space(&new_state);
    }
}