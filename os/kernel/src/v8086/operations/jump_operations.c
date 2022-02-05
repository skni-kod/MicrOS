#include <stdint.h>
#include <v8086/v8086.h>
#include <v8086/memory_operations.h>
#include "jump_operations.h"

int16_t jump_short_relative(v8086* machine)
{
    int8_t offset = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint32_t tempIP = machine->IP.w.ip;
    tempIP += offset;
    if((tempIP > 0xFFFF)) return V8086_RELATIVE_JMP_OVERFLOW;
    machine->IP.w.ip = tempIP;
    return V8086_OK;
}

int16_t jump_long_relative(v8086* machine, uint8_t width)
{
    int32_t offset;
    if(width == 32)
    {
        offset = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 4;
    }
    else if(width == 16)
    {
        offset = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 2;
    }
    else return V8086_BAD_WIDTH;

    uint32_t tempIP = machine->IP.w.ip;
    tempIP += offset;
    if(width == 16)
        tempIP &= 0xffffu;

    if((tempIP > 0xFFFF)) return V8086_RELATIVE_JMP_OVERFLOW;
    machine->IP.w.ip = tempIP;
    return V8086_OK;
}

int16_t jump_near_relative(v8086* machine)
{
    int16_t offset = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 2;
    machine->IP.w.ip += offset;
    return V8086_OK;
}

int16_t jump_far(v8086* machine)
{
    int16_t newIP = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 2;
    int16_t newCS = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 2;
    machine->sregs.cs = newCS;
    machine->IP.w.ip = newIP;
    machine->internal_state.IPOffset = 0;
    return V8086_OK;
}

int16_t jump_on_condition(v8086* machine, uint8_t opcode, uint8_t width)
{
    uint8_t jump = 0;
    switch(opcode & 0x0fu)
    {
        case 0: //JO
            if(bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) jump = 1;
            break;
        case 1: //JNO
            if(!bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) jump = 1;
            break;
        case 2: //JB or JNAE or JC
            if(bit_get(machine->regs.x.flags, 1u <<CARRY_FLAG_BIT)) jump = 1;
            break;
        case 3: //JNB or JAE or JNC
            if(!bit_get(machine->regs.x.flags, 1u <<CARRY_FLAG_BIT)) jump = 1;
            break;
        case 4: //JZ or JE
            if(bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT)) jump = 1;
            break;
        case 5: //JNZ or JNE
            if(!bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT)) jump = 1;
            break;
        case 6: //JBE or JNA
            if(bit_get(machine->regs.x.flags, 1u <<CARRY_FLAG_BIT) || bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT)) jump = 1;
            break;
        case 7: //JNBE or JA
            if(!bit_get(machine->regs.x.flags, 1u <<CARRY_FLAG_BIT) && !bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT)) jump = 1;
            break;
        case 8: //JS
            if(bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT)) jump = 1;
            break;
        case 9: //JNS
            if(!bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT)) jump = 1;
            break;
        case 0xa: //JP or JPE
            if(bit_get(machine->regs.x.flags, 1u <<PARITY_FLAG_BIT)) jump = 1;
            break;
        case 0xb: //JNP or JPO
            if(!bit_get(machine->regs.x.flags, 1u <<PARITY_FLAG_BIT)) jump = 1;
            break;
        case 0xc: //JL or JNGE
            if(!bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT) != !bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) jump = 1;
            break;
        case 0xd: //JNL or JGE
            if(!bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT) == !bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) jump = 1;
            break;
        case 0xe: //JLE or JNG
            if(bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT) || !bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT) != !bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) jump = 1;
            break;
        case 0xf: //JNLE or JG
            if(!bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT) && !bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT) == !bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) jump = 1;
            break;
    }

    if(jump){
        if(width == 8)
            return jump_short_relative(machine);
        else return jump_long_relative(machine, width);
    }
    else
    {
        machine->internal_state.IPOffset += width/8;
    }
    return V8086_OK;
}

int16_t jump_short_relative_on_condition(v8086* machine, uint8_t opcode)
{
    uint8_t jump = 0;
    if(opcode != 0xe3)
        return jump_on_condition(machine, opcode, 8);
    else
    {
        if(machine->internal_state.operand_32_bit)
        {
            if(!machine->regs.d.ecx) jump = 1;
        }
        else
        if(!machine->regs.w.cx) jump = 1;
    }
    if(jump) return jump_short_relative(machine);
    else
    {
        machine->internal_state.IPOffset += 1;
    }
    return V8086_OK;
}

int16_t perform_loop_loopne(v8086* machine, uint8_t opcode)
{
    uint8_t jump = 0;
    int8_t offset = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    if(machine->internal_state.operand_32_bit) machine->regs.d.ecx--;
    else machine->regs.w.cx--;

    switch (opcode)
    {
        case 0xe0:
            if(machine->internal_state.operand_32_bit){
                if(machine->regs.d.ecx && !bit_get(machine->regs.w.flags, 1u << ZERO_FLAG_BIT)) jump = 1;
            }
            else
            if(machine->regs.w.cx && !bit_get(machine->regs.w.flags, 1u << ZERO_FLAG_BIT)) jump = 1;
            break;
        case 0xe1:
            if(machine->internal_state.operand_32_bit){
                if(machine->regs.d.ecx && bit_get(machine->regs.w.flags, 1u << ZERO_FLAG_BIT)) jump = 1;
            }
            else
            if(machine->regs.w.cx && bit_get(machine->regs.w.flags, 1u << ZERO_FLAG_BIT)) jump = 1;
            break;
        case 0xe2:
            if(machine->internal_state.operand_32_bit){
                if(machine->regs.d.ecx) jump = 1;
            }
            else
            if(machine->regs.w.cx) jump = 1;
            break;
        default:
            return V8086_UNKNOWN_ERROR;
    }

    if(jump)
        machine->IP.w.ip += offset;

    return V8086_OK;
}