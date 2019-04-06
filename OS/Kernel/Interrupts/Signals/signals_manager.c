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
        signal_params *params = heap_user_alloc(sizeof(signal_params), 0);

        params->interrupt_number = state->interrupt_number;
        params->cr2 = state->cr2;

        params->eip = state->eip;
        params->cs = state->cs;
        params->eflags = state->eflags;
        params->esp = state->esp;
        params->ss = state->ss;

        memcpy(&params->registers, &state->registers, sizeof(registers_state));
        memcpy(&params->fpu_state, &state->fpu_state, sizeof(fpu_state));

        interrupt_state new_state = {0};
        new_state.eip = (uint32_t)info->signal_handler;
        new_state.cs = state->cs;
        new_state.ss = state->ss;
        new_state.eflags = state->eflags;
        new_state.esp = state->esp - 4;

        uint32_t *stack_ptr = (uint32_t *)state->esp;
        *stack_ptr = (uint32_t)params;

        enter_user_space(&new_state);
    }
}