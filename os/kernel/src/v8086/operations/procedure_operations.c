#include <v8086/memory_operations.h>
#include <v8086/stack.h>
#include "procedure_operations.h"

int16_t near_relative_call(v8086* machine)
{
    int16_t immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 2;

    machine->IP.w.ip += machine->internal_state.IPOffset;
    push_word(machine, machine->IP.w.ip);
    machine->IP.w.ip += immediate;
    machine->internal_state.IPOffset = 0;
    return OK;
}

int16_t far_call(v8086 *machine){
    int32_t newIP;
    if (machine->internal_state.operand_32_bit)
    {
        newIP = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip +
                                                                                                 machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 4;
    }
    else
        {
        newIP = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                             machine->IP.w.ip +
                                                                             machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 2;
    }
    int16_t newCS = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip +
                                                                                                    machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 2;
    push_word(machine, machine->sregs.cs);
    push_word(machine, machine->IP.w.ip + machine->internal_state.IPOffset);
    machine->IP.w.ip = newIP;
    machine->sregs.cs = newCS;
    machine->internal_state.IPOffset = 0;
    return OK;
}

int16_t near_ret(v8086* machine)
{
    machine->IP.w.ip = pop_word(machine);
    machine->internal_state.IPOffset = 0;
    return OK;
}

int16_t near_ret_imm(v8086* machine)
{
    uint16_t immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 2;
    machine->IP.w.ip = pop_word(machine);
    machine->regs.w.sp += immediate;
    machine->internal_state.IPOffset = 0;
    return OK;
}

int16_t far_ret(v8086* machine)
{
    machine->IP.w.ip = pop_word(machine);
    machine->sregs.cs = pop_word(machine);
    machine->internal_state.IPOffset = 0;
    return OK;
}

int16_t far_ret_imm(v8086* machine)
{
    uint16_t immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 2;
    machine->IP.w.ip = pop_word(machine);
    machine->sregs.cs = pop_word(machine);
    machine->regs.w.sp += immediate;
    machine->internal_state.IPOffset = 0;
    return OK;
}

int16_t perform_interrupt(v8086* machine, uint8_t interrupt_number)
{
    int16_t newIP = read_word_from_pointer(machine->Memory, get_absolute_address(0, interrupt_number * 4));
    int16_t newCS = read_word_from_pointer(machine->Memory, get_absolute_address(0, interrupt_number * 4 + 2));

    push_word(machine, machine->regs.w.flags);
    push_word(machine, machine->sregs.cs);
    push_word(machine, machine->IP.w.ip);

    machine->IP.w.ip = newIP;
    machine->sregs.cs = newCS;

    machine->internal_state.IPOffset = 0;
    return OK;
}

int16_t perform_iret(v8086* machine)
{
    machine->IP.w.ip = pop_word(machine);
    machine->sregs.cs = pop_word(machine);
    machine->regs.w.flags = pop_word(machine);

    machine->internal_state.IPOffset = 0;
    return OK;
}
