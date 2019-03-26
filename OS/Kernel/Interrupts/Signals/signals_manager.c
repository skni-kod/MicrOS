#include "signals_manager.h"

void signals_manager_init()
{
    idt_attach_exception_handler(14, signals_page_fault);
}

void signals_page_fault(exception_state *state)
{
    __asm__("nop");
}