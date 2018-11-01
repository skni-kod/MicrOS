#include "idt.h"

idt_entry idt_entries[IDT_INTERRUPTS_COUNT];
idt_info idt_information;

void idt_init()
{
    idt_information.size = sizeof(idt_entry) * IDT_INTERRUPTS_COUNT - 1;
    idt_information.offset = &idt_entries;

    idt_set(32, IRQ0);
    idt_set(33, IRQ1);
    idt_set(34, IRQ2);
    idt_set(35, IRQ3);
    idt_set(36, IRQ4);
    idt_set(37, IRQ5);
    idt_set(38, IRQ6);
    idt_set(39, IRQ7);
    idt_set(40, IRQ8);
    idt_set(41, IRQ9);
    idt_set(42, IRQ10);
    idt_set(43, IRQ11);
    idt_set(44, IRQ12);
    idt_set(45, IRQ13);
    idt_set(46, IRQ14);
    idt_set(47, IRQ15);
    idt_set(48, IRQ16);
    idt_set(49, IRQ17);
    idt_set(50, IRQ18);
    idt_set(51, IRQ19);
    idt_set(52, IRQ20);
    idt_set(53, IRQ21);
    idt_set(54, IRQ22);
    idt_set(55, IRQ23);
    idt_set(56, IRQ24);
    idt_set(57, IRQ25);
    idt_set(58, IRQ26);
    idt_set(59, IRQ27);
    idt_set(60, IRQ28);
    idt_set(61, IRQ29);
    idt_set(62, IRQ30);
    idt_set(63, IRQ31);

    __asm__ ("lidt %0" :: "m"(idt_information));
}

void idt_set(uint8_t index, void (*handler)())
{
    int handler_address = handler;

    idt_entries[index].offset_0_15 = handler_address & 0xFFFF;
    idt_entries[index].offset_16_31 = (handler_address >> 16) & 0xFFFF;
    idt_entries[index].present = 1;
    idt_entries[index].selector = 8;
    idt_entries[index].type = Interrupt_32Bit;
}

void idt_unset(int index)
{
    idt_entries[index].present = 0;
}

void IRQ0_handler(void)
{
    outb(0x20, 0x20);
}
 
void IRQ1_handler(void)
{
	outb(0x20, 0x20);
}
 
void IRQ2_handler(void)
{
    outb(0x20, 0x20);
}
 
void IRQ3_handler(void)
{
    outb(0x20, 0x20);
}
 
void IRQ4_handler(void)
{
    outb(0x20, 0x20);
}
 
void IRQ5_handler(void)
{
    outb(0x20, 0x20);
}
 
void IRQ6_handler(void)
{
    outb(0x20, 0x20);
}
 
void IRQ7_handler(void)
{
    outb(0x20, 0x20);
}
 
void IRQ8_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);          
}
 
void IRQ9_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}
 
void IRQ10_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}
 
void IRQ11_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}
 
void IRQ12_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}
 
void IRQ13_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}
 
void IRQ14_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}
 
void IRQ15_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ16_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ17_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ18_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ19_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ20_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ21_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ22_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ23_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ24_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ25_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ26_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ27_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ28_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ29_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ30_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void IRQ31_handler(void)
{
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}