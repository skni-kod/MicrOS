#ifndef IDT_H
#define IDT_H

#define IDT_INTERRUPT_DESCRIPTOR_TABLE_LENGTH 64
#define IDT_MAX_INTERRUPT_HANDLERS 64

#include <stdlib.h>
#include <string.h>
#include "idt_entry.h"
#include "idt_info.h"
#include "state/interrupt_state.h"
#include "state/exception_state.h"
#include "interrupts/interrupt_handler_definition.h"
#include "exceptions/exception_handler_definition.h"
#include "exceptions/exception_definition.h"
#include "cpu/pic/pic.h"
#include "cpu/panic/panic_screen.h"
#include "drivers/keyboard/keyboard.h"

void idt_init();
void idt_set(uint8_t index, uint32_t (*handler)(interrupt_state *state), bool user_interrupt);
void idt_unset(uint8_t index);

void idt_attach_interrupt_handler(uint8_t interrupt_number, bool (*handler)(interrupt_state *state));
void idt_detach_interrupt_handler(uint8_t interrupt_number, bool (*handler)(interrupt_state *state));

void idt_attach_exception_handler(uint8_t exception_number, void (*handler)(exception_state *state));
void idt_detach_exception_handler(uint8_t exception_number, void (*handler)(exception_state *state));

void idt_attach_process_manager(void (*handler)(interrupt_state *state));
void idt_attach_syscalls_manager(void (*handler)(interrupt_state *state));

void idt_global_int_handler(interrupt_state *state);
void idt_global_exc_handler(exception_state *state);
void idt_software_interrupt_handler();
bool idt_syscalls_interrupt_handler(interrupt_state *state);

// Exceptions
extern uint32_t idt_exc0();
extern uint32_t idt_exc1();
extern uint32_t idt_exc2();
extern uint32_t idt_exc3();
extern uint32_t idt_exc4();
extern uint32_t idt_exc5();
extern uint32_t idt_exc6();
extern uint32_t idt_exc7();
extern uint32_t idt_exc8();
extern uint32_t idt_exc9();
extern uint32_t idt_exc10();
extern uint32_t idt_exc11();
extern uint32_t idt_exc12();
extern uint32_t idt_exc13();
extern uint32_t idt_exc14();
extern uint32_t idt_exc15();
extern uint32_t idt_exc16();
extern uint32_t idt_exc17();
extern uint32_t idt_exc18();
extern uint32_t idt_exc19();
extern uint32_t idt_exc20();
extern uint32_t idt_exc21();
extern uint32_t idt_exc22();
extern uint32_t idt_exc23();
extern uint32_t idt_exc24();
extern uint32_t idt_exc25();
extern uint32_t idt_exc26();
extern uint32_t idt_exc27();
extern uint32_t idt_exc28();
extern uint32_t idt_exc29();
extern uint32_t idt_exc30();
extern uint32_t idt_exc31();

// Hardware interrupts
extern uint32_t idt_int32();
extern uint32_t idt_int33();
extern uint32_t idt_int34();
extern uint32_t idt_int35();
extern uint32_t idt_int36();
extern uint32_t idt_int37();
extern uint32_t idt_int38();
extern uint32_t idt_int39();
extern uint32_t idt_int40();
extern uint32_t idt_int41();
extern uint32_t idt_int42();
extern uint32_t idt_int43();
extern uint32_t idt_int44();
extern uint32_t idt_int45();
extern uint32_t idt_int46();
extern uint32_t idt_int47();

// Software interrupts
extern uint32_t idt_int50();

#endif