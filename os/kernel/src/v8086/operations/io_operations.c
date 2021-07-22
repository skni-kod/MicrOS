#include <v8086/memory_operations.h>
#include <assembly/io.h>
#include <v8086/mod_rm_parsing.h>
#include "io_operations.h"

int16_t perform_in_imm(v8086* machine, uint8_t width)
{
    uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    if(width == 8) machine->regs.h.al = io_in_byte(immediate);
    else if(width == 16) machine->regs.w.ax = io_in_word(immediate);
    else if(width == 32) machine->regs.d.eax = io_in_long(immediate);
    else return V8086_BAD_WIDTH;
    return V8086_OK;
}

int16_t perform_in_dx(v8086* machine, uint8_t width)
{
    if(width == 8) machine->regs.h.al = io_in_byte(machine->regs.x.dx);
    else if(width == 16) machine->regs.w.ax = io_in_word(machine->regs.x.dx);
    else if(width == 32) machine->regs.d.eax = io_in_long(machine->regs.x.dx);
    else return V8086_BAD_WIDTH;
    return V8086_OK;
}

int16_t perform_out_imm(v8086* machine, uint8_t width)
{
    uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    if(width == 8) io_out_byte(immediate, machine->regs.h.al);
    else if(width == 16) io_out_word(immediate, machine->regs.w.ax);
    else if(width == 32) io_out_long(immediate, machine->regs.d.eax);
    else return V8086_BAD_WIDTH;

    return V8086_OK;
}

int16_t perform_out_dx(v8086* machine, uint8_t width)
{
    if(width == 8) io_out_byte(machine->regs.x.dx, machine->regs.h.al);
    else if(width == 16) io_out_word(machine->regs.x.dx, machine->regs.w.ax);
    else if(width == 32) io_out_long(machine->regs.x.dx, machine->regs.d.eax);
    else return V8086_BAD_WIDTH;

    return V8086_OK;
}

int16_t perform_ins_dx(v8086* machine, uint8_t width)
{
    uint16_t* dest_segment = select_segment_register(machine, V8086_ES);

    //if repeat and number of repats == 0 -> dont copy anything
    if(machine->internal_state.rep_prefix == V8086_REP_REPE && machine->regs.w.cx == 0) return V8086_OK;

    do{
        void* dest = NULL;
        if(machine->internal_state.address_32_bit)
            dest = get_variable_length_pointer(machine->Memory, get_absolute_address(*dest_segment, machine->regs.d.esi), width);
        else
            dest = get_variable_length_pointer(machine->Memory, get_absolute_address(*dest_segment, machine->regs.w.si), width);

        if(width == 8) *((uint8_t*)dest) = io_in_byte(machine->regs.x.dx);
        else if(width == 16) *((uint16_t*)dest) = io_in_word(machine->regs.x.dx);
        else if(width == 32) *((uint32_t*)dest)= io_in_long(machine->regs.x.dx);
        else return V8086_BAD_WIDTH;
        int8_t offset = width / 8;
        if(machine->internal_state.address_32_bit)
            machine->regs.d.esi += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -offset : offset;
        else
            machine->regs.w.si += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -offset : offset;
    } while(machine->internal_state.rep_prefix == V8086_REP_REPE && --(machine->regs.w.cx));

    return V8086_OK;
}

int16_t perform_outs_dx(v8086* machine, uint8_t width)
{
    uint16_t* dest_segment = select_segment_register(machine, V8086_ES);

    //if repeat and number of repats == 0 -> dont copy anything
    if(machine->internal_state.rep_prefix == V8086_REP_REPE && machine->regs.w.cx == 0) return V8086_OK;

    do{
        void* dest = NULL;
        if(machine->internal_state.address_32_bit)
            dest = get_variable_length_pointer(machine->Memory, get_absolute_address(*dest_segment, machine->regs.d.esi), width);
        else
            dest = get_variable_length_pointer(machine->Memory, get_absolute_address(*dest_segment, machine->regs.w.si), width);

        if(width == 8) io_out_byte(machine->regs.x.dx, *((uint8_t*)dest));
        else if(width == 16) io_out_word(machine->regs.x.dx, *((uint16_t*)dest));
        else if(width == 32) io_out_long(machine->regs.x.dx, *((uint32_t*)dest));
        else return V8086_BAD_WIDTH;
        int8_t offset = width / 8;
        if(machine->internal_state.address_32_bit)
            machine->regs.d.esi += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -offset : offset;
        else
            machine->regs.w.si += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -offset : offset;
    } while(machine->internal_state.rep_prefix == V8086_REP_REPE && --(machine->regs.w.cx));

    return V8086_OK;
}