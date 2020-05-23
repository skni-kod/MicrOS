#ifndef V8086_STACK_H
#define V8086_STACK_H

static inline void push_word(v8086* machine, uint16_t value)
{
    write_word_to_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp -= 2), value);
}

static inline void push_dword(v8086* machine, uint32_t value)
{
    write_dword_to_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp -= 4), value);
}

static inline uint16_t pop_word(v8086* machine)
{
    uint16_t v = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp));
    machine->regs.w.sp += 2;
    return v;
}

static inline uint32_t pop_dword(v8086* machine)
{
    uint16_t v = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp));
    machine->regs.w.sp += 4;
    return v;
}

#endif //V8086_STACK_H
