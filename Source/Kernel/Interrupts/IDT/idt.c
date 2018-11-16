#include "idt.h"
#include "../../Drivers/Keyboard/keyboard.h"

volatile idt_entry idt_entries[IDT_INTERRUPT_DESCRIPTOR_TABLE_LENGTH];
volatile idt_info idt_information;
volatile interrupt_handler_definition interrupt_handlers[IDT_MAX_INTERRUPT_HANDLERS];

void idt_init()
{
    // Init Interrupt Descriptor Table descriptor
    idt_information.size = sizeof(idt_entry) * IDT_INTERRUPT_DESCRIPTOR_TABLE_LENGTH - 1;
    idt_information.offset = (uint32_t)&idt_entries;

    // Hardware interrupts (IRQ)
    idt_set(32, int32);      // Programmable Interrupt Timer 
    idt_set(33, int33);      // Keyboard
    idt_set(34, int34);      // Cascade
    idt_set(35, int35);      // COM2
    idt_set(36, int36);      // COM1
    idt_set(37, int37);      // LPT2
    idt_set(38, int38);      // Floppy
    idt_set(39, int39);      // LPT1
    idt_set(40, int40);      // CMOS
    idt_set(41, int41);      // Free
    idt_set(42, int42);      // Free
    idt_set(43, int43);      // Free
    idt_set(44, int44);      // Mouse
    idt_set(45, int45);      // FPU
    idt_set(46, int46);      // Primary ATA Hard Disk
    idt_set(47, int47);      // Secondary ATA Hard Disk

    // Software interrupts
    idt_set(48, int48);
    idt_set(49, int49);
    idt_set(50, int50);
    idt_set(51, int51);
    idt_set(52, int52);

    // Load Interrupt Descriptor Table to the register
    __asm__ ("lidt %0" :: "m"(idt_information));
}

void idt_set(uint8_t index, uint32_t (*handler)())
{
    uint32_t handler_address = (uint32_t)handler;

    idt_entries[index].offset_0_15 = handler_address & 0xFFFF;
    idt_entries[index].offset_16_31 = (handler_address >> 16) & 0xFFFF;
    idt_entries[index].present = 1;
    idt_entries[index].selector = 8;
    idt_entries[index].type = Interrupt_32Bit;
}

void idt_unset(uint8_t index)
{
    idt_entries[index].present = 0;
}

void idt_attach_interrupt_handler(uint8_t interrupt_number, void (*handler)())
{
    for(int i=0; i<IDT_MAX_INTERRUPT_HANDLERS; i++)
    {
        if(interrupt_handlers[i].handler == 0)
        {
            interrupt_handlers[i].interrupt_number = interrupt_number + 32;
            interrupt_handlers[i].handler = handler;
            break;
        }
    }
}

void idt_detach_interrupt_handler(uint8_t interrupt_number, void (*handler)())
{
    for(int i=0; i<IDT_MAX_INTERRUPT_HANDLERS; i++)
    {
        if(interrupt_handlers[i].interrupt_number == interrupt_number + 32 && interrupt_handlers[i].handler == handler)
        {
            interrupt_handlers[i].handler = 0;
            break;
        }
    }
}

void global_int_handler(interrupt_state state)
{
    for(int i=0; i<IDT_MAX_INTERRUPT_HANDLERS; i++)
    {
        if(interrupt_handlers[i].interrupt_number == state.interrupt_number && interrupt_handlers[i].handler != 0)
        {
            interrupt_handlers[i].handler();
        }
    }

    // This is temporary code which imitates software interrupt and handles sleep function (which is required for
    // floppy driver). It will be moved to the separate files in da future.
    if(state.interrupt_number == 48)
    {
        state.eax = timer_get_system_clock();
    }

    state.interrupt_number - 32 < 8 ? pic_confirm_master() : pic_confirm_master_and_slave();
}