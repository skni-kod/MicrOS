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
        width = machine->internal_state.operand_32_bit ? 32 : 16;
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

uint16_t perform_mov_gpr_imm(v8086* machine, uint8_t opcode)
{
    uint8_t width;
    if(opcode >= 0xb0 && opcode <= 0xb7) width = 8;
    else if(opcode >= 0xb8 && opcode <= 0xbf) width = machine->internal_state.operand_32_bit ? 32 : 16;
    else return UNKNOWN_ERROR;
    uint8_t reg_selector = (opcode >= 0xb8) ? (opcode - 0xb8u) & 0x7u : opcode & 0x7u;
    void* reg = get_variable_length_register(machine, reg_selector, width);
    if(reg == NULL) return UNDEFINED_REGISTER;
    void* imm = get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset), width);
    machine->internal_state.IPOffset += width / 8;
    return perform_mov(imm, reg, width);
}

uint16_t perform_mov_rm_imm(v8086* machine, uint8_t opcode)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    if(opcode == 0xc6)
    {
        uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t* mem = get_memory_from_mode(machine, mod_rm, 8);
        if(mem == NULL) return UNABLE_GET_MEMORY;
        *mem = immediate;
    }
    else
    {
        if(machine->internal_state.operand_32_bit)
        {
            uint32_t immediate = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 4;
            uint32_t* mem = get_memory_from_mode(machine, mod_rm, 32);
            if(mem == NULL) return UNABLE_GET_MEMORY;
            *mem = immediate;
        }
        else
        {
            uint16_t immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 2;
            uint16_t* mem = get_memory_from_mode(machine, mod_rm, 16);
            if(mem == NULL) return UNABLE_GET_MEMORY;
            *mem = immediate;
        }
    }
    return OK;
}

int16_t perform_mov_moffset(v8086* machine, uint8_t opcode)
{
    uint16_t offset = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 2;
    uint16_t* segment = machine->internal_state.segment_reg_select == DEFAULT ? select_segment_register(machine, DS) : select_segment_register(machine, machine->internal_state.segment_reg_select);
    if(segment == NULL) return UNDEFINED_SEGMENT_REGISTER;
    switch (opcode)
    {
        case 0xa0:
            machine->regs.h.al = read_byte_from_pointer(machine->Memory, get_absolute_address(*segment, offset));
            break;
        case 0xa1:
            if(machine->internal_state.operand_32_bit) machine->regs.d.eax = read_dword_from_pointer(machine->Memory, get_absolute_address(*segment, offset));
            else machine->regs.x.ax = read_word_from_pointer(machine->Memory, get_absolute_address(*segment, offset));
            break;
        case 0xa2:
            write_byte_to_pointer(machine->Memory, get_absolute_address(*segment, offset), machine->regs.h.al);
            break;
        case 0xa3:
            if(machine->internal_state.operand_32_bit) write_dword_to_pointer(machine->Memory, get_absolute_address(*segment, offset), machine->regs.d.eax);
            else write_word_to_pointer(machine->Memory, get_absolute_address(*segment, offset), machine->regs.x.ax);
            break;
        default:
            return UNKNOWN_ERROR;
    }
    return OK;
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

int16_t convert_byte_to_word(v8086* machine)
{
    if(machine->internal_state.operand_32_bit)
        machine->regs.d.eax = ((int32_t)(machine->regs.w.ax));
    else
        machine->regs.w.ax = ((int16_t)(machine->regs.h.al));
    return OK;
}

int16_t convert_word_to_double(v8086* machine)
{
    if(machine->internal_state.operand_32_bit){
        int64_t t = machine->regs.d.eax;
        machine->regs.d.edx = ((uint64_t)t >> 32u);
    }
    else{
        int32_t t = machine->regs.w.ax;
        machine->regs.w.dx = ((uint32_t)t >> 16u);
    }
    return OK;
}

int16_t store_flags(v8086* machine)
{
    for(uint8_t i = 0; i < 8; i++)
        if(i != 1 && i != 3 && i != 5)
            bit_write(machine->regs.w.flags, 1u <<i, bit_get(machine->regs.h.ah, 1u <<i));
    return OK;
}

int16_t load_flags(v8086* machine)
{
    machine->regs.h.ah = machine->regs.w.flags & 0xFFu;
    return OK;
}