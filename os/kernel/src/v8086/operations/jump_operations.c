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
    if((tempIP > 0xFFFF)) return RELATIVE_JMP_OVERFLOW;
    machine->IP.w.ip = tempIP;
    return OK;
}

int16_t jump_short_relative_on_condition(v8086* machine, uint8_t opcode)
{
    uint8_t jump = 0;
    if(opcode != 0xe3)
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
                if(!(bit_get(machine->regs.x.flags, 1u <<CARRY_FLAG_BIT) || bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT))) jump = 1;
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
                if(bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT) || !bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT) == !bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) jump = 1;
                break;
        }
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
    return OK;
}