#ifndef IDT_ENTRY_H
#define IDT_ENTRY_H

#include <stdint.h>

/*
IDF entry gate types:
 - task - hardware multitasking things
 - interrupt - jump to the specified function
 - trap - similar to interrupt, but without disabling other interrupts
*/
typedef enum idt_entrygatetype
{
    Task_16Bit = 0x5,
    Interrupt_16Bit = 0x6,
    Trap_16Bit = 0x7,

    Interrupt_32Bit = 0xE,
    Trap_32Bit = 0xF
} idt_entrygatetype;

typedef struct idt_entry
{
    uint16_t offset_0_15;
    uint16_t selector;
    uint8_t reserved;
    uint8_t type : 4;
    uint8_t storage_segment : 1;
    uint8_t privilege_level : 2;
    uint8_t present : 1;
    uint16_t offset_16_31;
} __attribute__((packed)) idt_entry;

#endif