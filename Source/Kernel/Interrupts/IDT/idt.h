#ifndef IDT_H
#define IDT_H

#define IDT_INTERRUPT_DESCRIPTOR_TABLE_LENGTH 64
#define IDT_MAX_INTERRUPT_HANDLERS 64

#include "idt_entry.h"
#include "idt_info.h"
#include "interrupt_state.h"
#include "interrupt_handler_definition.h"
#include "../PIC/pic.h"

void idt_init();
void idt_set(uint8_t index, uint32_t (*handler)());
void idt_unset(uint8_t index);
void idt_attach_interrupt_handler(uint8_t interrupt_number, void (*handler)());
void idt_detach_interrupt_handler(uint8_t interrupt_number, void (*handler)());

void global_int_handler();
void global_exc_handler();
void software_interrupt_handler();

// Exceptions
extern uint32_t exc0();
extern uint32_t exc1();
extern uint32_t exc2();
extern uint32_t exc3();
extern uint32_t exc4();
extern uint32_t exc5();
extern uint32_t exc6();
extern uint32_t exc7();
extern uint32_t exc8();
extern uint32_t exc9();
extern uint32_t exc10();
extern uint32_t exc11();
extern uint32_t exc12();
extern uint32_t exc13();
extern uint32_t exc14();
extern uint32_t exc15();
extern uint32_t exc16();
extern uint32_t exc17();
extern uint32_t exc18();
extern uint32_t exc19();
extern uint32_t exc20();
extern uint32_t exc21();
extern uint32_t exc22();
extern uint32_t exc23();
extern uint32_t exc24();
extern uint32_t exc25();
extern uint32_t exc26();
extern uint32_t exc27();
extern uint32_t exc28();
extern uint32_t exc29();
extern uint32_t exc30();
extern uint32_t exc31();

// Hardware interrupts
extern uint32_t int32();
extern uint32_t int33();
extern uint32_t int34();
extern uint32_t int35();
extern uint32_t int36();
extern uint32_t int37();
extern uint32_t int38();
extern uint32_t int39();
extern uint32_t int40();
extern uint32_t int41();
extern uint32_t int42();
extern uint32_t int43();
extern uint32_t int44();
extern uint32_t int45();
extern uint32_t int46();
extern uint32_t int47();

// Software interrupts
extern uint32_t int48();
extern uint32_t int49();
extern uint32_t int50();
extern uint32_t int51();
extern uint32_t int52();

#endif