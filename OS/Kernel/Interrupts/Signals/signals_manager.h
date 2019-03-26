#ifndef SIGNALS_MANAGER_H
#define SIGNALS_MANAGER_H

#include "../IDT/idt.h"
#include "../IDT/interrupt_state.h"
#include "../../Process/Manager/process_manager.h"

void signals_manager_init();
void signals_page_fault(exception_state *state);

#endif