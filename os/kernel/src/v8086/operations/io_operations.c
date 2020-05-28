#include <v8086/memory_operations.h>
#include <assembly/io.h>
#include "io_operations.h"

int16_t perform_in_imm(v8086* machine, uint8_t width)
{
    uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    if(width == 8) machine->regs.h.al = io_in_byte(immediate);
    else if(width == 16) machine->regs.w.ax = io_in_word(immediate);
    else if(width == 32) machine->regs.d.eax = io_in_long(immediate);
    else return BAD_WIDTH;
    return OK;
}

int16_t perform_in_dx(v8086* machine, uint8_t width)
{
    if(width == 8) machine->regs.h.al = io_in_byte(machine->regs.x.dx);
    else if(width == 16) machine->regs.w.ax = io_in_word(machine->regs.x.dx);
    else if(width == 32) machine->regs.d.eax = io_in_long(machine->regs.x.dx);
    else return BAD_WIDTH;
    return OK;
}

int16_t perform_out_imm(v8086* machine, uint8_t width)
{
    uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    if(width == 8) io_out_byte(immediate, machine->regs.h.al);
    else if(width == 16) io_out_word(immediate, machine->regs.w.ax);
    else if(width == 32) io_out_long(immediate, machine->regs.d.eax);
    else return BAD_WIDTH;

    return OK;
}

int16_t perform_out_dx(v8086* machine, uint8_t width)
{
    if(width == 8) io_out_byte(machine->regs.x.dx, machine->regs.h.al);
    else if(width == 16) io_out_word(machine->regs.x.dx, machine->regs.w.ax);
    else if(width == 32) io_out_long(machine->regs.x.dx, machine->regs.d.eax);
    else return BAD_WIDTH;

    return OK;
}