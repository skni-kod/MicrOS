#pragma once
#define IDT_INTERRUPT_DESCRIPTOR_TABLE_LENGTH 64

#include "idt_entry.h"
#include "idt_info.h"
#include "../../Assembly/io.h"
#include "../PIC/pic.h"

void idt_init();
void idt_set(uint8_t index, void (*handler)());
void idt_unset(int index);

// Hardware interrupts
extern int int0();
extern int int1();
extern int int2();
extern int int3();
extern int int4();
extern int int5();
extern int int6();
extern int int7();
extern int int8();
extern int int9();
extern int int10();
extern int int11();
extern int int12();
extern int int13();
extern int int14();
extern int int15();

// Software interrupts
extern int int48();
extern int int49();
extern int int50();
extern int int51();
extern int int52();