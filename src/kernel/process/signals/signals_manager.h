#ifndef SIGNALS_MANAGER_H
#define SIGNALS_MANAGER_H

#include "cpu/idt/idt.h"
#include "cpu/idt/state/interrupt_state.h"
#include "process/manager/process_manager.h"
#include "signal_params.h"

void signals_manager_init();
void signals_page_fault(exception_state *state);

#endif