#ifndef V8086_STACK_H
#define V8086_STACK_H

#include "v8086.h"
#include "memory_operations.h"

static inline void push_byte(v8086* machine, uint8_t value)
{
    machine->regs.w.sp -= 1;
    write_byte_to_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp), value);
}

static inline void push_word(v8086* machine, uint16_t value)
{
    machine->regs.w.sp -= 2;
    write_word_to_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp), value);
}

static inline void push_dword(v8086* machine, uint32_t value)
{
    machine->regs.w.sp -= 4;
    write_dword_to_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp), value);
}

static inline uint8_t pop_byte(v8086* machine)
{
    uint8_t v = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp));
    machine->regs.w.sp += 1;
    return v;
}

static inline uint16_t pop_word(v8086* machine)
{
    uint16_t v = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp));
    machine->regs.w.sp += 2;
    return v;
}

static inline uint32_t pop_dword(v8086* machine)
{
    uint32_t v = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp));
    machine->regs.w.sp += 4;
    return v;
}

#endif //V8086_STACK_H
