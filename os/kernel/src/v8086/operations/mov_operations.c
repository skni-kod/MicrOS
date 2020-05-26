#include <stdint.h>
#include <v8086/v8086.h>
#include <v8086/memory_operations.h>
#include <v8086/mod_rm_parsing.h>
#include "mov_operations.h"
#include "internal_funcs.h"

int16_t perform_mov(void* source, void* dest, uint8_t width) {
    switch (width) {
        case 8:
            *((uint8_t *) dest) = *((uint8_t *) source);
            break;
        case 16:
            *((uint16_t *) dest) = *((uint16_t *) source);
            break;
        case 32:
            *((uint32_t *) dest) = *((uint32_t *) source);
            break;
        default:
            return BAD_WIDTH;
    }
    return OK;
}

int16_t perform_mov_rm(v8086* machine, uint8_t opcode)
{
    //Mod/RM
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = 8;
    if(opcode % 2)
        machine->internal_state.operand_32_bit ? 32 : 16;
    void* source = get_variable_length_register(machine, get_reg(mod_rm), width);
    void* dest = get_memory_from_mode(machine, mod_rm, width);
    if(source == NULL) return UNDEFINED_REGISTER;
    if(dest == NULL) return UNABLE_GET_MEMORY;
    switch (opcode)
    {
        case 0x88:
        case 0x89:
            return perform_mov(source, dest, width);
        case 0x8a:
        case 0x8b:
            return perform_mov(dest, source, width);
        default:
            return UNKNOWN_ERROR;
    }
}

int16_t perform_mov_segment(v8086* machine, uint8_t opcode)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint16_t* source = select_segment_register(machine, get_reg(mod_rm));
    uint16_t* dest = get_memory_from_mode(machine, mod_rm, 16);
    if(source == NULL) return UNDEFINED_SEGMENT_REGISTER;
    if(dest == NULL) return UNABLE_GET_MEMORY;

    if(opcode == 0x8c)
        return perform_mov(source, dest, 16);
    else if(opcode == 0x8e)
        return perform_mov(dest, source, 16);
    else return UNKNOWN_ERROR;
}

int16_t perform_lea(v8086* machine)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    uint16_t segment;
    uint32_t offset;

    if((mod_rm >> 6u) > 3) return BAD_MOD;

    int16_t r;
    if(machine->internal_state.address_32_bit)
        r = calculate_segment_offset_from_mode_32(machine, mod_rm, &segment, &offset);
    else
        r = calculate_segment_offset_from_mode(machine, mod_rm, &segment, &offset);

    if(r) return r;

    if(machine->internal_state.operand_32_bit)
    {
        uint32_t* reg = get_dword_register(machine, get_reg(mod_rm));
        if(reg == NULL) return UNDEFINED_REGISTER;
        *reg = offset;
    }
    else
    {
        uint16_t* reg = get_word_register(machine, get_reg(mod_rm));
        if(reg == NULL) return UNDEFINED_REGISTER;
        *reg = offset;
    }
    return OK;
}