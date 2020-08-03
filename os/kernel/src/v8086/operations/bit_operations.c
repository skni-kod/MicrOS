#include <v8086/memory_operations.h>
#include <v8086/mod_rm_parsing.h>
#include "bit_operations.h"
#include "internal_funcs.h"

int16_t bit_test(v8086* machine, uint8_t width)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    if(width == 16)
    {
        uint16_t* memory = (uint16_t*) get_memory_from_mode(machine, mod_rm, width);
        uint16_t* reg = (uint16_t*) get_variable_length_register(machine, get_reg(mod_rm), width);

        bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(*reg % 16u)));
    }
    else if(width == 32)
    {
        uint32_t* memory = (uint32_t*) get_memory_from_mode(machine, mod_rm, width);
        uint32_t* reg = (uint32_t*) get_variable_length_register(machine, get_reg(mod_rm), width);

        bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(*reg % 32u)));
    }
    else return BAD_WIDTH;
    return OK;
}

int16_t bit_test_set(v8086* machine, uint8_t width)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    if(width == 16)
    {
        uint16_t* memory = (uint16_t*) get_memory_from_mode(machine, mod_rm, width);
        uint16_t* reg = (uint16_t*) get_variable_length_register(machine, get_reg(mod_rm), width);

        bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(*reg % 16u)));
        bit_set(*memory, 1u<<(*reg % 16u));
    }
    else if(width == 32)
    {
        uint32_t* memory = (uint32_t*) get_memory_from_mode(machine, mod_rm, width);
        uint32_t* reg = (uint32_t*) get_variable_length_register(machine, get_reg(mod_rm), width);

        bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(*reg % 32u)));
        bit_set(*memory, 1u<<(*reg % 32u));
    }
    else return BAD_WIDTH;
    return OK;
}

int16_t bit_test_reset(v8086* machine, uint8_t width)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    if(width == 16)
    {
        uint16_t* memory = (uint16_t*) get_memory_from_mode(machine, mod_rm, width);
        uint16_t* reg = (uint16_t*) get_variable_length_register(machine, get_reg(mod_rm), width);

        bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(*reg % 16u)));
        bit_clear(*memory, 1u<<(*reg % 16u));
    }
    else if(width == 32)
    {
        uint32_t* memory = (uint32_t*) get_memory_from_mode(machine, mod_rm, width);
        uint32_t* reg = (uint32_t*) get_variable_length_register(machine, get_reg(mod_rm), width);

        bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(*reg % 32u)));
        bit_clear(*memory, 1u<<(*reg % 32u));
    }
    else return BAD_WIDTH;
    return OK;
}

int16_t bit_test_complement(v8086* machine, uint8_t width)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    if(width == 16)
    {
        uint16_t* memory = (uint16_t*) get_memory_from_mode(machine, mod_rm, width);
        uint16_t* reg = (uint16_t*) get_variable_length_register(machine, get_reg(mod_rm), width);

        bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(*reg % 16u)));
        bit_flip(*memory, 1u<<(*reg % 16u));
    }
    else if(width == 32)
    {
        uint32_t* memory = (uint32_t*) get_memory_from_mode(machine, mod_rm, width);
        uint32_t* reg = (uint32_t*) get_variable_length_register(machine, get_reg(mod_rm), width);

        bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(*reg % 32u)));
        bit_flip(*memory, 1u<<(*reg % 32u));
    }
    else return BAD_WIDTH;
    return OK;
}

int16_t perform_group_8(v8086* machine, uint8_t width)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    switch(get_reg(mod_rm))
    {
        case 4: // BT rm, imm8
            if(width == 16)
            {
                uint16_t* memory = (uint16_t*) get_memory_from_mode(machine, mod_rm, width);
                uint8_t imm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 1;

                bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(imm % 16u)));
            }
            else if(width == 32)
            {
                uint32_t* memory = (uint32_t*) get_memory_from_mode(machine, mod_rm, width);
                uint8_t imm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 1;

                bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(imm % 32u)));
            }
            else return BAD_WIDTH;
            return OK;
        case 5: // BTS rm, imm8
            if(width == 16)
            {
                uint16_t* memory = (uint16_t*) get_memory_from_mode(machine, mod_rm, width);
                uint8_t imm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 1;

                bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(imm % 16u)));
                bit_set(*memory, 1u<<(imm % 16u));
            }
            else if(width == 32)
            {
                uint32_t* memory = (uint32_t*) get_memory_from_mode(machine, mod_rm, width);
                uint8_t imm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 1;

                bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(imm % 32u)));
                bit_set(*memory, 1u<<(imm % 32u));
            }
            else return BAD_WIDTH;
            return OK;
        case 6: //BTR rm, imm8
            if(width == 16)
            {
                uint16_t* memory = (uint16_t*) get_memory_from_mode(machine, mod_rm, width);
                uint8_t imm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 1;

                bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(imm % 16u)));
                bit_clear(*memory, 1u<<(imm % 16u));
            }
            else if(width == 32)
            {
                uint32_t* memory = (uint32_t*) get_memory_from_mode(machine, mod_rm, width);
                uint8_t imm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 1;

                bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(imm % 32u)));
                bit_clear(*memory, 1u<<(imm % 32u));
            }
            else return BAD_WIDTH;
            return OK;
        case 7: //BTC rm, imm8
            if(width == 16)
            {
                uint16_t* memory = (uint16_t*) get_memory_from_mode(machine, mod_rm, width);
                uint8_t imm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 1;

                bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(imm % 16u)));
                bit_flip(*memory, 1u<<(imm % 16u));
            }
            else if(width == 32)
            {
                uint32_t* memory = (uint32_t*) get_memory_from_mode(machine, mod_rm, width);
                uint8_t imm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 1;

                bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(*memory, 1u<<(imm % 32u)));
                bit_flip(*memory, 1u<<(imm % 32u));
            }
            else return BAD_WIDTH;
            return OK;
        default:
            return UNDEFINED_OPCODE;
    }
}