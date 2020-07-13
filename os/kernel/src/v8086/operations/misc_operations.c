#include <stdint.h>
#include <v8086/memory_operations.h>
#include <v8086/mod_rm_parsing.h>
#include <v8086/stack.h>
#include "misc_operations.h"
#include "internal_funcs.h"
#include "arithmetic_operations.h"

int16_t perform_group_4(v8086* machine)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = 8;

    void* dest = get_memory_from_mode(machine, mod_rm, width);
    if(dest == NULL) return UNABLE_GET_MEMORY;
    switch(get_reg(mod_rm))
    {
        case 0: //INC rm8
            return perform_inc(machine, dest, width);
            break;
        case 1: //DEC rm8
            return  perform_dec(machine, dest, width);
            break;
        default:
            return UNDEFINED_OPCODE;
    }
}

int16_t perform_group_5(v8086* machine)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = 16;
    if(machine->internal_state.operand_32_bit) width = 32;

    void* dest = get_memory_from_mode(machine, mod_rm, width);
    if(dest == NULL) return UNABLE_GET_MEMORY;
    switch(get_reg(mod_rm))
    {
        case 0: //INC rm8
            return perform_inc(machine, dest, width);
        case 1: //DEC rm8
            return perform_dec(machine, dest, width);
        case 2: //Near absolute indirect call
            machine->IP.w.ip += machine->internal_state.IPOffset;
            push_word(machine, machine->IP.w.ip);
            if(width == 16)
                machine->IP.w.ip += *((uint16_t*) dest);
            else return BAD_WIDTH;
            machine->internal_state.IPOffset = 0;
            return OK;
        case 3: // Far absolute indirect call
            machine->IP.w.ip += machine->internal_state.IPOffset;
            push_word(machine, machine->sregs.cs);
            push_word(machine, machine->IP.w.ip);
            machine->IP.w.ip = *((uint16_t*) dest);
            machine->sregs.cs = *(((uint16_t*)dest)+1);
            machine->internal_state.IPOffset = 0;
            return OK;
        case 4: //Near absolute indirect jmp
            if(width == 16)
                machine->IP.w.ip += *((uint16_t*) dest);
            else return BAD_WIDTH;
            machine->internal_state.IPOffset = 0;
            return OK;
        case 5: //Far absolute indirect jmp
            machine->IP.w.ip = *((uint16_t*) dest);
            machine->sregs.cs = *(((uint16_t*)dest)+1);
            machine->internal_state.IPOffset = 0;
            return OK;
        case 6:
            if(width == 16) push_word(machine, *((uint16_t*)dest));
            else push_dword(machine, *((uint32_t*)dest));
            return OK;
        default:
            return UNDEFINED_OPCODE;
    }
    return UNKNOWN_ERROR;
}

int16_t setting_and_clearing_flags(v8086* machine, uint8_t opcode)
{
    //FLAG Setting and clearing Group
    //CMC
    if(opcode == 0xf5)
        bit_flip(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT);
        //CLC
    else if(opcode == 0xf8)
        bit_clear(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT);
        //STC
    else if(opcode == 0xf9)
        bit_set(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT);
        //CLI
    else if(opcode == 0xfa)
        bit_clear(machine->regs.w.flags, 1u <<INTERRUPT_FLAG_BIT);
        //STI
    else if(opcode == 0xfb)
        bit_set(machine->regs.w.flags, 1u <<INTERRUPT_FLAG_BIT);
        //CLD
    else if(opcode == 0xfc)
        bit_clear(machine->regs.w.flags, 1u <<DIRECTION_FLAG_BIT);
        //STD
    else if(opcode == 0xfd)
        bit_set(machine->regs.w.flags, 1u <<DIRECTION_FLAG_BIT);
    else
        return UNKNOWN_ERROR;
    return OK;
}

int16_t check_bounds(v8086* machine)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = 16;
    if(machine->internal_state.operand_32_bit) width = 32;

    void* index = get_variable_length_register(machine, get_reg(mod_rm), width);
    void* bounds = get_memory_from_mode(machine, mod_rm, width);

    if(width==16)
    {
        int16_t* i = (int16_t*) index;
        int16_t* lb = (int16_t*) bounds;
        int16_t* ub = ((int16_t*) bounds + 1);

        if((*i < *lb) || (*i > *ub)) return BOUND_ERROR;
    }
    else
    {
        int32_t* i = (int32_t*) index;
        int32_t* lb = (int32_t*) bounds;
        int32_t* ub = ((int32_t*) bounds + 1);

        if((*i < *lb) || (*i > *ub)) return BOUND_ERROR;
    }
    return OK;
}