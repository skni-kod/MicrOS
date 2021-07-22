#include <stdint.h>
#include <stdbool.h>
#include <v8086/v8086.h>
#include <v8086/memory_operations.h>
#include "ascii_adjustments_operations.h"

int16_t adjust_after_add_sub(v8086* machine, bool sub)
{
    if(((machine->regs.h.al & 0x0fu) > 9u) || bit_get(machine->regs.w.flags, 1u <<AUX_CARRY_FLAG_BIT))
    {
        if(!sub) machine->regs.x.ax += 0x106;
        else
        {
            machine->regs.h.al -= 0x6;
            machine->regs.h.ah -= 1;
        }
        bit_set(machine->regs.w.flags, 1u <<AUX_CARRY_FLAG_BIT);
        bit_set(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT);
    }
    else
    {
        bit_clear(machine->regs.w.flags, 1u <<AUX_CARRY_FLAG_BIT);
        bit_clear(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT);
    }
    machine->regs.h.al &= 0x0fu;
    return V8086_OK;
}

int16_t adjust_after_add_sub_packed(v8086* machine, bool sub)
{
    uint8_t old_AL = machine->regs.h.al;
    uint16_t old_cf = bit_get(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT);
    bit_clear(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT);
    if(((machine->regs.h.al & 0x0fu) > 9u) || bit_get(machine->regs.w.flags, 1u <<AUX_CARRY_FLAG_BIT))
    {
        uint16_t temp_ax = machine->regs.h.al + (!sub ? 6 : -6);
        machine->regs.h.al = temp_ax & 0xFFu;
        bit_write(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT, (old_cf || (temp_ax > 0xFF)) ? 1 : 0);
        bit_set(machine->regs.w.flags, 1u <<AUX_CARRY_FLAG_BIT);
    }
    else bit_clear(machine->regs.w.flags, 1u <<AUX_CARRY_FLAG_BIT);

    if((old_AL > 0x99) || old_cf)
    {
        machine->regs.h.al += sub ? 0x60 : -0x60;
        bit_set(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT);
    }
    else if(!sub) bit_clear(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT);

    uint8_t parrity = 0;
    for(uint8_t i = 0; i < 8; i++) parrity += (uint8_t)(machine->regs.h.al >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u << PARITY_FLAG_BIT, ((parrity % 2 == 0) ? 1: 0)); //PARRITY FLAG
    /*uint8_t parrity = machine->regs.h.al & 1u;
    for(uint8_t i = 1; i < 8; i++) parrity ^= (uint8_t)(machine->regs.h.al >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG*/
    bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, machine->regs.h.al == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, machine->regs.h.al >> (7u)); //SIGN FLAG
    return V8086_OK;
}

int16_t adjust_after_mul_div(v8086* machine, bool div)
{
    uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t tempAL = machine->regs.h.al;
    uint8_t tempAH = machine->regs.h.ah;

    if(!div) {
        if(immediate == 0) return V8086_DIVISION_BY_ZERO;
        machine->regs.h.ah = tempAL / immediate;
        machine->regs.h.al = tempAL % immediate;
    }
    else {
        machine->regs.h.al = (uint8_t)(tempAL + (tempAH * immediate)) & 0xFFu;
        machine->regs.h.ah = 0;
    }

    uint8_t parrity = 0;
    for(uint8_t i = 0; i < 8; i++) parrity += (uint8_t)(machine->regs.h.al >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u << PARITY_FLAG_BIT, ((parrity % 2 == 0) ? 1: 0)); //PARRITY FLAG
    /*
    uint8_t parrity = machine->regs.h.al & 1u;
    for(uint8_t i = 1; i < 8; i++) parrity ^= (uint8_t)(machine->regs.h.al >> i) & 1u;
     bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0);
     */
    bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, machine->regs.h.al == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, machine->regs.h.al >> (7u)); //SIGN FLAG

    return V8086_OK;
}