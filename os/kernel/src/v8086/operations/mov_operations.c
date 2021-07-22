#include <stdint.h>
#include <v8086/v8086.h>
#include <v8086/memory_operations.h>
#include <v8086/mod_rm_parsing.h>
#include "mov_operations.h"
#include "internal_funcs.h"

int16_t perform_mov(v8086* machine, void* source, void* dest, uint8_t width, uint8_t flags_affected) {
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
            return V8086_BAD_WIDTH;
    }

    if(flags_affected){
        bit_clear(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT);
        bit_clear(machine->regs.w.flags, 1u << CARRY_FLAG_BIT);
        bit_clear(machine->regs.w.flags, 1u << SIGN_FLAG_BIT);
        bit_clear(machine->regs.w.flags, 1u << ZERO_FLAG_BIT);
        bit_clear(machine->regs.w.flags, 1u << PARITY_FLAG_BIT);
        bit_clear(machine->regs.w.flags, 1u << AUX_CARRY_FLAG_BIT);
    }

    return V8086_OK;
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
    if(source == NULL) return V8086_UNDEFINED_REGISTER;
    if(dest == NULL) return V8086_UNABLE_GET_MEMORY;
    switch (opcode)
    {
        case 0x88:
        case 0x89:
            return perform_mov(machine, source, dest, width, 0);
        case 0x8a:
        case 0x8b:
            return perform_mov(machine, dest, source, width, 0);
        default:
            return V8086_UNKNOWN_ERROR;
    }
}

int16_t perform_movzx_variable_length(void* source, void* dest, uint8_t source_width, uint8_t dest_width)
{
    if(source_width == 8)
    {
        if(dest_width == 16)
            *((uint16_t *) dest) = *((uint8_t *) source);
        else if(dest_width == 32)
            *((uint32_t *) dest) = *((uint8_t *) source);
        else return V8086_BAD_WIDTH;
    }
    else if(source_width == 16)
    {
        if(dest_width == 32)
            *((uint32_t *) dest) = *((uint16_t *) source);
        else return V8086_BAD_WIDTH;
    } else return V8086_BAD_WIDTH;
    return V8086_OK;
}

int16_t perform_movsx_variable_length(void* source, void* dest, uint8_t source_width, uint8_t dest_width)
{
    if(source_width == 8)
    {
        if(dest_width == 16)
            *((int16_t *) dest) = *((int8_t *) source);
        else if(dest_width == 32)
            *((int32_t *) dest) = *((int8_t *) source);
        else return V8086_BAD_WIDTH;
    }
    else if(source_width == 16)
    {
        if(dest_width == 32)
            *((int32_t *) dest) = *((int16_t *) source);
        else return V8086_BAD_WIDTH;
    } else return V8086_BAD_WIDTH;
    return V8086_OK;
}

int16_t perform_movzx(v8086* machine, uint8_t opcode)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = 32;
    if(!(opcode % 2))
        width = machine->internal_state.operand_32_bit ? 32 : 16;
    void* dest = get_variable_length_register(machine, get_reg(mod_rm), width);

    if(opcode == 0xb6u)
    {
        void* source = get_memory_from_mode(machine, mod_rm, 8);
        return perform_movzx_variable_length(source, dest, 8, width);
    }
    else if(opcode == 0xb7u)
    {
        void* source = get_memory_from_mode(machine, mod_rm, 16);
        return perform_movzx_variable_length(source, dest, 16, width);
    }
    else return V8086_UNKNOWN_ERROR;
}

int16_t perform_movsx(v8086* machine, uint8_t opcode)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = 32;
    if(!(opcode % 2))
        width = machine->internal_state.operand_32_bit ? 32 : 16;
    void* dest = get_variable_length_register(machine, get_reg(mod_rm), width);

    if(opcode == 0xbeu)
    {
        void* source = get_memory_from_mode(machine, mod_rm, 8);
        return perform_movsx_variable_length(source, dest, 8, width);
    }
    else if(opcode == 0xbfu)
    {
        void* source = get_memory_from_mode(machine, mod_rm, 16);
        return perform_movsx_variable_length(source, dest, 16, width);
    }
    else return V8086_UNKNOWN_ERROR;
}

int16_t perform_mov_segment(v8086* machine, uint8_t opcode)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint16_t* source = select_segment_register(machine, get_reg(mod_rm));
    uint16_t* dest = get_memory_from_mode(machine, mod_rm, 16);
    if(source == NULL) return V8086_UNDEFINED_SEGMENT_REGISTER;
    if(dest == NULL) return V8086_UNABLE_GET_MEMORY;

    if(opcode == 0x8c)
        return perform_mov(machine, source, dest, 16, 0);
    else if(opcode == 0x8e)
        return perform_mov(machine, dest, source, 16, 0);
    else return V8086_UNKNOWN_ERROR;
}

uint16_t perform_mov_gpr_imm(v8086* machine, uint8_t opcode)
{
    uint8_t width;
    if(opcode >= 0xb0 && opcode <= 0xb7) width = 8;
    else if(opcode >= 0xb8 && opcode <= 0xbf) width = machine->internal_state.operand_32_bit ? 32 : 16;
    else return V8086_UNKNOWN_ERROR;
    uint8_t reg_selector = (opcode >= 0xb8) ? (opcode - 0xb8u) & 0x7u : opcode & 0x7u;
    void* reg = get_variable_length_register(machine, reg_selector, width);
    if(reg == NULL) return V8086_UNDEFINED_REGISTER;
    void* imm = get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset), width);
    machine->internal_state.IPOffset += width / 8;
    return perform_mov(machine, imm, reg, width, 0);
}

uint16_t perform_mov_rm_imm(v8086* machine, uint8_t opcode)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    if(opcode == 0xc6)
    {
        uint8_t* mem = get_memory_from_mode(machine, mod_rm, 8);
        uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        if(mem == NULL) return V8086_UNABLE_GET_MEMORY;
        *mem = immediate;
    }
    else
    {
        if(machine->internal_state.operand_32_bit)
        {
            uint32_t* mem = get_memory_from_mode(machine, mod_rm, 32);
            uint32_t immediate = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 4;
            if(mem == NULL) return V8086_UNABLE_GET_MEMORY;
            *mem = immediate;
        }
        else
        {
            uint16_t* mem = get_memory_from_mode(machine, mod_rm, 16);
            uint16_t immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 2;
            if(mem == NULL) return V8086_UNABLE_GET_MEMORY;
            *mem = immediate;
        }
    }
    return V8086_OK;
}

int16_t perform_mov_moffset(v8086* machine, uint8_t opcode)
{
    uint16_t offset = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 2;
    uint16_t* segment = machine->internal_state.segment_reg_select == V8086_DEFAULT ? select_segment_register(machine, V8086_DS) : select_segment_register(machine, machine->internal_state.segment_reg_select);
    if(segment == NULL) return V8086_UNDEFINED_SEGMENT_REGISTER;
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
            return V8086_UNKNOWN_ERROR;
    }
    return V8086_OK;
}

int16_t perform_lea(v8086* machine)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    uint16_t segment;
    uint32_t offset;

    if((mod_rm >> 6u) > 3) return V8086_BAD_MOD;

    int16_t r;
    if(machine->internal_state.address_32_bit)
        r = calculate_segment_offset_from_mode_32(machine, mod_rm, &segment, &offset);
    else
        r = calculate_segment_offset_from_mode(machine, mod_rm, &segment, &offset);

    if(r) return r;

    if(machine->internal_state.operand_32_bit)
    {
        uint32_t* reg = get_dword_register(machine, get_reg(mod_rm));
        if(reg == NULL) return V8086_UNDEFINED_REGISTER;
        *reg = offset;
    }
    else
    {
        uint16_t* reg = get_word_register(machine, get_reg(mod_rm));
        if(reg == NULL) return V8086_UNDEFINED_REGISTER;
        *reg = offset;
    }
    return V8086_OK;
}

int16_t convert_byte_to_word(v8086* machine)
{
    if(machine->internal_state.operand_32_bit)
        machine->regs.d.eax = ((int32_t)(machine->regs.w.ax));
    else
        machine->regs.w.ax = ((int16_t)(machine->regs.h.al));
    return V8086_OK;
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
    return V8086_OK;
}

int16_t store_flags(v8086* machine)
{
    for(uint8_t i = 0; i < 8; i++)
        if(i != 1 && i != 3 && i != 5)
            bit_write(machine->regs.w.flags, 1u <<i, bit_get(machine->regs.h.ah, 1u <<i));
    return V8086_OK;
}

int16_t load_flags(v8086* machine)
{
    machine->regs.h.ah = machine->regs.w.flags & 0xFFu;
    return V8086_OK;
}

int16_t perform_load_far_pointer(v8086* machine, segment_register_select segment_op)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    uint16_t* segment_register;
    segment_register = select_segment_register(machine, segment_op);
    if(segment_register == NULL) return V8086_UNDEFINED_SEGMENT_REGISTER;
    uint16_t* source = get_memory_from_mode(machine, mod_rm, 16);
    if(source == NULL) return V8086_UNABLE_GET_MEMORY;

    if(machine->internal_state.operand_32_bit)
    {
        uint32_t* dest = get_dword_register(machine, get_reg(mod_rm));
        *dest = *((uint32_t*) source);
        *segment_register = *(source+2);
    }
    else
    {
        uint16_t* dest = get_word_register(machine, get_reg(mod_rm));
        *dest = *source;
        *segment_register = *(source+1);
    }
    return V8086_OK;
}

int16_t perform_xlat(v8086* machine)
{
    uint8_t tempAL = machine->regs.h.al;
    uint16_t* segment;
    if(machine->internal_state.segment_reg_select != V8086_DEFAULT)
        segment = select_segment_register(machine, machine->internal_state.segment_reg_select);
    else
        segment = select_segment_register(machine, V8086_DS);
    if(segment == NULL) return V8086_UNDEFINED_SEGMENT_REGISTER;
    machine->regs.h.al = read_byte_from_pointer(machine->Memory, get_absolute_address(*segment, machine->regs.w.bx + tempAL));
    return V8086_OK;
}