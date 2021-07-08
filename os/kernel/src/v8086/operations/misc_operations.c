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
    if(dest == NULL) return V8086_UNABLE_GET_MEMORY;
    switch(get_reg(mod_rm))
    {
        case 0: //INC rm8
            return perform_inc(machine, dest, width);
        case 1: //DEC rm8
            return  perform_dec(machine, dest, width);
        default:
            return V8086_UNDEFINED_OPCODE;
    }
}

int16_t perform_group_5(v8086* machine)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = 16;
    if(machine->internal_state.operand_32_bit) width = 32;

    void* dest = get_memory_from_mode(machine, mod_rm, width);
    if(dest == NULL) return V8086_UNABLE_GET_MEMORY;
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
                machine->IP.w.ip = *((uint16_t*) dest);
            else return V8086_BAD_WIDTH;
            machine->internal_state.IPOffset = 0;
            return V8086_OK;
        case 3: // Far absolute indirect call
            machine->IP.w.ip += machine->internal_state.IPOffset;
            push_word(machine, machine->sregs.cs);
            push_word(machine, machine->IP.w.ip);
            machine->IP.w.ip = *((uint16_t*) dest);
            machine->sregs.cs = *(((uint16_t*)dest)+1);
            machine->internal_state.IPOffset = 0;
            return V8086_OK;
        case 4: //Near absolute indirect jmp
            if(width == 16)
                machine->IP.w.ip = *((uint16_t*) dest);
            else return V8086_BAD_WIDTH;
            machine->internal_state.IPOffset = 0;
            return V8086_OK;
        case 5: //Far absolute indirect jmp
            machine->IP.w.ip = *((uint16_t*) dest);
            machine->sregs.cs = *(((uint16_t*)dest)+1);
            machine->internal_state.IPOffset = 0;
            return V8086_OK;
        case 6:
            if(width == 16) push_word(machine, *((uint16_t*)dest));
            else push_dword(machine, *((uint32_t*)dest));
            return V8086_OK;
        default:
            return V8086_UNDEFINED_OPCODE;
    }
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
        return V8086_UNKNOWN_ERROR;
    return V8086_OK;
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

        if((*i < *lb) || (*i > *ub)) return V8086_BOUND_ERROR;
    }
    else
    {
        int32_t* i = (int32_t*) index;
        int32_t* lb = (int32_t*) bounds;
        int32_t* ub = ((int32_t*) bounds + 1);

        if((*i < *lb) || (*i > *ub)) return V8086_BOUND_ERROR;
    }
    return V8086_OK;
}

int16_t set_byte(v8086* machine, uint8_t opcode)
{
    uint8_t set_byte = 0;
    switch(opcode & 0x0fu)
    {
        case 0: //JO
            if(bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) set_byte = 1;
            break;
        case 1: //JNO
            if(!bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) set_byte = 1;
            break;
        case 2: //JB or JNAE or JC
            if(bit_get(machine->regs.x.flags, 1u <<CARRY_FLAG_BIT)) set_byte = 1;
            break;
        case 3: //JNB or JAE or JNC
            if(!bit_get(machine->regs.x.flags, 1u <<CARRY_FLAG_BIT)) set_byte = 1;
            break;
        case 4: //JZ or JE
            if(bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT)) set_byte = 1;
            break;
        case 5: //JNZ or JNE
            if(!bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT)) set_byte = 1;
            break;
        case 6: //JBE or JNA
            if(bit_get(machine->regs.x.flags, 1u <<CARRY_FLAG_BIT) || bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT)) set_byte = 1;
            break;
        case 7: //JNBE or JA
            if(!(bit_get(machine->regs.x.flags, 1u <<CARRY_FLAG_BIT) || bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT))) set_byte = 1;
            break;
        case 8: //JS
            if(bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT)) set_byte = 1;
            break;
        case 9: //JNS
            if(!bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT)) set_byte = 1;
            break;
        case 0xa: //JP or JPE
            if(bit_get(machine->regs.x.flags, 1u <<PARITY_FLAG_BIT)) set_byte = 1;
            break;
        case 0xb: //JNP or JPO
            if(!bit_get(machine->regs.x.flags, 1u <<PARITY_FLAG_BIT)) set_byte = 1;
            break;
        case 0xc: //JL or JNGE
            if(!bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT) != !bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) set_byte = 1;
            break;
        case 0xd: //JNL or JGE
            if(!bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT) == !bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) set_byte = 1;
            break;
        case 0xe: //JLE or JNG
            if(bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT) || !bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT) != !bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) set_byte = 1;
            break;
        case 0xf: //JNLE or JG
            if(bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT) || !bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT) == !bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) set_byte = 1;
            break;
    }

    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t* memory = (uint8_t*) get_memory_from_mode(machine, mod_rm, 8);
    *memory = set_byte;
    return V8086_OK;
}