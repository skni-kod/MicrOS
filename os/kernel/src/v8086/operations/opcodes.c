#include <stdint.h>
#include <v8086/stack.h>
#include <stdbool.h>
#include "opcodes.h"
#include "arithmetic_operations.h"
#include "ascii_adjustments_operations.h"

#define NO_CARRY 0
#define CARRY_FLAG_AS_NUMBER bit_get(machine->regs.d.eflags, 1u <<CARRY_FLAG_BIT) >> CARRY_FLAG_BIT

OPCODE_PROTO(add)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode, NO_CARRY, perform_adding);
}

OPCODE_PROTO(push_es)
{
    push_word(machine, machine->sregs.es);
    return OK;
}

OPCODE_PROTO(pop_es)
{
    machine->sregs.es = pop_word(machine);
    return OK;
}

OPCODE_PROTO(or)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x08, NO_CARRY, perform_or);
}

OPCODE_PROTO(push_cs)
{
    push_word(machine, machine->sregs.cs);
    return OK;
}

OPCODE_PROTO(adc)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x10, CARRY_FLAG_AS_NUMBER, perform_adding);
}

OPCODE_PROTO(push_ss)
{
    push_word(machine, machine->sregs.ss);
    return OK;
}

OPCODE_PROTO(pop_ss)
{
    machine->sregs.ss = pop_word(machine);
    return OK;
}

OPCODE_PROTO(sbb)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x18, CARRY_FLAG_AS_NUMBER, perform_subtracting);
}

OPCODE_PROTO(push_ds)
{
    push_word(machine, machine->sregs.ds);
    return OK;
}

OPCODE_PROTO(pop_ds)
{
    machine->sregs.ds = pop_word(machine);
    return OK;
}

OPCODE_PROTO(and)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x20, 0, perform_and);
}

OPCODE_PROTO(daa)
{
    return adjust_after_add_sub_packed(machine, false);
}

OPCODE_PROTO(sub)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x28, NO_CARRY, perform_subtracting);
}

OPCODE_PROTO(das)
{
    return adjust_after_add_sub_packed(machine, true);
}

OPCODE_PROTO(xor)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x30, NO_CARRY, perform_xor);
}

OPCODE_PROTO(aaa)
{
    return adjust_after_add_sub(machine, false);
}

OPCODE_PROTO(cmp)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x30, NO_CARRY, perform_cmp);
}

OPCODE_PROTO(aas)
{
    return adjust_after_add_sub(machine, true);
}

OPCODE_PROTO(inc)
{
    return perform_inc_dec(machine, opcode, false);
}

OPCODE_PROTO(dec)
{
    return perform_inc_dec(machine, opcode, true);
}