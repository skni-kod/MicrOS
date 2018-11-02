#include "idt.h"

idt_entry idt_entries[IDT_INTERRUPT_DESCRIPTOR_TABLE_LENGTH];
idt_info idt_information;

void idt_init()
{
    // Init Interrupt Descriptor Table descriptor
    idt_information.size = sizeof(idt_entry) * IDT_INTERRUPT_DESCRIPTOR_TABLE_LENGTH - 1;
    idt_information.offset = &idt_entries;

    // Hardware interrupts (IRQ)
    idt_set(32, int0);      // Programmable Interrupt Timer 
    idt_set(33, int1);      // Keyboard
    idt_set(34, int2);      // Cascade
    idt_set(35, int3);      // COM2
    idt_set(36, int4);      // COM1
    idt_set(37, int5);      // LPT2
    idt_set(38, int6);      // Floppy
    idt_set(39, int7);      // LPT1
    idt_set(40, int8);      // CMOS
    idt_set(41, int9);      // Free
    idt_set(42, int10);     // Free
    idt_set(43, int11);     // Free
    idt_set(44, int12);     // Mouse
    idt_set(45, int13);     // FPU
    idt_set(46, int14);     // Primary ATA Hard Disk
    idt_set(47, int15);     // Secondary ATA Hard Disk

    // Software interrupts
    idt_set(48, int48);
    idt_set(49, int49);
    idt_set(50, int50);
    idt_set(51, int51);
    idt_set(52, int52);

    // Load Interrupt Descriptor Table to the register
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

void idt_confirm_master_pic()
{
    // Confirm interrupt in master PIC
    outb(0x20, 0x20);
}

void idt_confirm_master_and_slave_pic()
{
    // Confirm interrupt in master PIC
    outb(0x20, 0x20);

    // Confirm interrupt in slave PIC
    outb(0xA0, 0x20);
}

void int0_handler()
{
    idt_confirm_master_pic();
}
 
void int1_handler()
{
	idt_confirm_master_pic();
}
 
void int2_handler()
{
    idt_confirm_master_pic();
}
 
void int3_handler()
{
    idt_confirm_master_pic();
}
 
void int4_handler()
{
    idt_confirm_master_pic();
}
 
void int5_handler()
{
    idt_confirm_master_pic();
}
 
void int6_handler()
{
    idt_confirm_master_pic();
}
 
void int7_handler()
{
    idt_confirm_master_pic();
}

void int8_handler()
{
    idt_confirm_master_and_slave_pic();
}

void int9_handler()
{
    idt_confirm_master_and_slave_pic();
}

void int10_handler()
{
    idt_confirm_master_and_slave_pic();
}

void int11_handler()
{
    idt_confirm_master_and_slave_pic();
}

void int12_handler()
{
    idt_confirm_master_and_slave_pic();
}

void int13_handler()
{
    idt_confirm_master_and_slave_pic();
}

void int14_handler()
{
    idt_confirm_master_and_slave_pic();
}

void int15_handler()
{
    idt_confirm_master_and_slave_pic();
}

void int48_handler()
{
    
}

void int49_handler()
{
    
}

void int50_handler()
{
    
}

void int51_handler()
{
    
}

void int52_handler()
{
    
}