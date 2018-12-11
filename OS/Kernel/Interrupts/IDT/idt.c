#include "idt.h"
#include "../../Drivers/Keyboard/keyboard.h"

volatile idt_entry idt_entries[IDT_INTERRUPT_DESCRIPTOR_TABLE_LENGTH];
volatile idt_info idt_information;
volatile interrupt_handler_definition interrupt_handlers[IDT_MAX_INTERRUPT_HANDLERS];

exception_definition exceptions[] =
{
    { .interrupt_number = 0,  .description = "Divide-by-zero error", },
    { .interrupt_number = 1,  .description = "Debug", },
    { .interrupt_number = 2,  .description = "Non-maskable Interrupt", },
    { .interrupt_number = 3,  .description = "Breakpoint", },
    { .interrupt_number = 4,  .description = "Overflow", },
    { .interrupt_number = 5,  .description = "Bound Range Exceeded", },
    { .interrupt_number = 6,  .description = "Invalid Opcode", },
    { .interrupt_number = 7,  .description = "Device Not Available", },
    { .interrupt_number = 8,  .description = "Double Fault", },
    { .interrupt_number = 9,  .description = "Coprocessor Segment Overrun", },
    { .interrupt_number = 10, .description = "Invalid TSS", },
    { .interrupt_number = 11, .description = "Segment Not Present", },
    { .interrupt_number = 12, .description = "Stack-Segment Fault", },
    { .interrupt_number = 13, .description = "General Protection Fault", },
    { .interrupt_number = 14, .description = "Page Fault", },
    { .interrupt_number = 15, .description = "Reserved", },
    { .interrupt_number = 16, .description = "x87 Floating-Point Exception", },
    { .interrupt_number = 17, .description = "Alignment Check", },
    { .interrupt_number = 18, .description = "Machine Check", },
    { .interrupt_number = 19, .description = "SIMD Floating-Point Exception", },
    { .interrupt_number = 20, .description = "Virtualization Exception", },
    { .interrupt_number = 21, .description = "Reserved", },
    { .interrupt_number = 22, .description = "Reserved", },
    { .interrupt_number = 23, .description = "Reserved", },
    { .interrupt_number = 24, .description = "Reserved", },
    { .interrupt_number = 25, .description = "Reserved", },
    { .interrupt_number = 26, .description = "Reserved", },
    { .interrupt_number = 27, .description = "Reserved", },
    { .interrupt_number = 28, .description = "Reserved", },
    { .interrupt_number = 29, .description = "Reserved", },
    { .interrupt_number = 30, .description = "Security Exception", },
    { .interrupt_number = 31, .description = "Reserved", }
};

void idt_init()
{
    // Init Interrupt Descriptor Table descriptor
    idt_information.size = sizeof(idt_entry) * IDT_INTERRUPT_DESCRIPTOR_TABLE_LENGTH - 1;
    idt_information.offset = (uint32_t)&idt_entries;

    // Exceptions
    idt_set(0, exc0);
    idt_set(1, exc1);
    idt_set(2, exc2);
    idt_set(3, exc3);
    idt_set(4, exc4);
    idt_set(5, exc5);
    idt_set(6, exc6);
    idt_set(7, exc7);
    idt_set(8, exc8);
    idt_set(9, exc9);
    idt_set(10, exc10);
    idt_set(11, exc11);
    idt_set(12, exc12);
    idt_set(13, exc13);
    idt_set(14, exc14);
    idt_set(15, exc15);
    idt_set(16, exc16);
    idt_set(17, exc17);
    idt_set(18, exc18);
    idt_set(19, exc19);
    idt_set(20, exc20);
    idt_set(21, exc21);
    idt_set(22, exc22);
    idt_set(23, exc23);
    idt_set(24, exc24);
    idt_set(25, exc25);
    idt_set(26, exc26);
    idt_set(27, exc27);
    idt_set(28, exc28);
    idt_set(29, exc29);
    idt_set(30, exc30);
    idt_set(31, exc31);

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

    // Add system calls interrupt handler
    idt_attach_interrupt_handler(16, syscalls_interrupt_handler);
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
            interrupt_handlers[i].handler(&state);
        }
    }

    state.interrupt_number - 32 < 8 ? pic_confirm_master() : pic_confirm_master_and_slave();
}

void global_exc_handler(interrupt_state state, uint32_t error_code)
{
    for(int i=0; i<32; i++)
    {
        if(exceptions[i].interrupt_number == state.interrupt_number)
        {
            char exception_string[64];
            char error_code_msg[] = ". Error code: ";
            char error_code_str[10];

            memset(exception_string, 0, 64);
            memset(error_code_str, 0, 10);

            uint32_t description_length = strlen(exceptions[i].description);
            uint32_t error_code_msg_length = strlen(error_code_msg);
            
            memcpy(exception_string, exceptions[i].description, description_length);
            memcpy(exception_string + description_length, error_code_msg, error_code_msg_length);

            itoa(error_code, error_code_str, 16);
            memcpy(exception_string + description_length + error_code_msg_length, error_code_str, 10);

            showPanicScreen(state.interrupt_number, exception_string);
            break;
        }
    }
}

void syscalls_interrupt_handler(interrupt_state* state)
{
    switch(state->eax)
    {
        case 0x01: get_system_clock_call(state); break;
        case 0x02: alloc_memory_call(state); break;
        case 0x03: dealloc_memory_call(state); break;
    }
}