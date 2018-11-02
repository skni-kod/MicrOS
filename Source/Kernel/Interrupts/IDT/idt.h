#pragma once
#define IDT_INTERRUPT_DESCRIPTOR_TABLE_LENGTH 64

#include "idt_entry.h"
#include "idt_info.h"
#include "../../Assembly/io.h"

void idt_init();
void idt_set(uint8_t index, void (*handler)());
void idt_unset(int index);
void idt_confirm_hardware_interrupt();
void idt_confirm_software_interrupt();

// Hardware interrupts
extern int int0();
extern int int1();
extern int int2();
extern int int3();
extern int int4();
extern int int5();
extern int int6();
extern int int7();

// Software interrupts
extern int int48();
extern int int49();
extern int int50();
extern int int51();
extern int int52();