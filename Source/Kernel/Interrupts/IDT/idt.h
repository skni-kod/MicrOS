#pragma once
#define IDT_INTERRUPTS_COUNT 64

#include "idt_entry.h"
#include "idt_info.h"
#include "../../Assembly/io.h"

void idt_init();
void idt_set(uint8_t index, void (*handler)());
void idt_unset(int index);

extern int IRQ0();
extern int IRQ1();
extern int IRQ2();
extern int IRQ3();
extern int IRQ4();
extern int IRQ5();
extern int IRQ6();
extern int IRQ7();
extern int IRQ8();
extern int IRQ9();
extern int IRQ10();
extern int IRQ11();
extern int IRQ12();
extern int IRQ13();
extern int IRQ14();
extern int IRQ15();
extern int IRQ16();
extern int IRQ17();
extern int IRQ18();
extern int IRQ19();
extern int IRQ20();
extern int IRQ21();
extern int IRQ22();
extern int IRQ23();
extern int IRQ24();
extern int IRQ25();
extern int IRQ26();
extern int IRQ27();
extern int IRQ28();
extern int IRQ29();
extern int IRQ30();
extern int IRQ31();