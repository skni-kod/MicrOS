#include <stdint.h>
#include <v8086/v8086.h>
#include <v8086/memory_operations.h>
#include <v8086/mod_rm_parsing.h>
#include <stddef.h>
#include <stdbool.h>
#include "arithmetic_operations.h"
#include "internal_funcs.h"

int16_t perform_adding(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry)
{
    uint64_t result = 0;
    uint32_t dest_before; //for overflow flag checking
    uint32_t source_before;
    if(width == 8){
        dest_before = *((uint8_t*)dest);
        source_before = *((uint8_t*)source);
    } else if(width == 16){
        dest_before = *((uint16_t*)dest);
        source_before = *((uint16_t*)source);
    } else if(width == 32){
        dest_before = *((uint32_t*)dest);
        source_before = *((uint32_t*)source);
    } else return BAD_WIDTH;
    result = dest_before + source_before + carry;
    bit_write(machine->regs.d.eflags, 1u <<CARRY_FLAG_BIT, (result >> width) ? 1 : 0); // CARRY FLAG
    uint8_t parrity = result & 1u;
    for(uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine->regs.d.eflags, 1u <<AUX_CARRY_FLAG_BIT, (((dest_before & 0xfu) + (source_before & 0xfu)) >> 4u) ? 1: 0); //AUX CARRY FLAG
    bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, result >> (width - 1u)); //SIGN FLAG
    bit_write(machine->regs.d.eflags, 1u <<OVERFLOW_FLAG_BIT, ((result >> (width - 1u)) != (dest_before >> (width - 1u)))); //OVERFLOW FLAG
    if(width == 8) *((uint8_t*)dest) = result & 0xFFu;
    else if(width == 16) *((uint16_t*)dest) = result & 0xFFFFu;
    else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
    else return BAD_WIDTH;
    return OK;
}

int16_t perform_subtracting(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry)
{
    uint64_t result = 0;
    uint32_t dest_before; //for overflow flag checking
    uint32_t source_before;
    if(width == 8){
        dest_before = *((uint8_t*)dest);
        source_before = *((uint8_t*)source);
    } else if(width == 16){
        dest_before = *((uint16_t*)dest);
        source_before = *((uint16_t*)source);
    } else if(width == 32){
        dest_before = *((uint32_t*)dest);
        source_before = *((uint32_t*)source);
    } else return BAD_WIDTH;
    result = dest_before - (source_before + carry);
    bit_write(machine->regs.d.eflags, 1u <<CARRY_FLAG_BIT, (result >> width) ? 1 : 0); // CARRY FLAG
    bit_write(machine->regs.d.eflags, 1u <<AUX_CARRY_FLAG_BIT, (((dest_before & 0xfu) - (source_before & 0xfu)) >> 4u) ? 1: 0); //AUX CARRY FLAG
    uint8_t parrity = result & 1u;
    for(uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, result >> (width - 1u)); //SIGN FLAG
    bit_write(machine->regs.d.eflags, 1u <<OVERFLOW_FLAG_BIT, ((result >> (width - 1u)) != (dest_before >> (width - 1u)))); //OVERFLOW FLAG
    if(width == 8) *((uint8_t*)dest) = result & 0xFFu;
    else if(width == 16) *((uint16_t*)dest) = result & 0xFFFFu;
    else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
    else return BAD_WIDTH;
    return OK;
}

int16_t perform_or(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry)
{
    uint32_t result = 0;
    if(width == 8)
        result = *((uint8_t*)dest) | *((uint8_t*)source);
    else if(width == 16)
        result = *((uint16_t*)dest) | *((uint16_t*)source);
    else if(width == 32)
        result = *((uint32_t*)dest) | *((uint32_t*)source);
    else return BAD_WIDTH;
    bit_write(machine->regs.d.eflags, 1u <<CARRY_FLAG_BIT, 0); // CARRY FLAG
    bit_write(machine->regs.d.eflags, 1u <<OVERFLOW_FLAG_BIT, 0); // OVERFLOW FLAG
    uint8_t parrity = result & 1u;
    for(uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, result >> (width - 1u)); //SIGN FLAG
    //AUX MARKED AS UNDEFINED IN INTEL DOCUMENTATION
    if(width == 8) *((uint8_t*)dest) = result & 0xFFu;
    else if(width == 16) *((uint16_t*)dest) = result & 0xFFFFu;
    else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
    return OK;
}

int16_t perform_and(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry)
{
    uint32_t result = 0;
    if(width == 8)
        result = *((uint8_t*)dest) & *((uint8_t*)source);
    else if(width == 16)
        result = *((uint16_t*)dest) & *((uint16_t*)source);
    else if(width == 32)
        result = *((uint32_t*)dest) & *((uint32_t*)source);
    else return BAD_WIDTH;
    bit_write(machine->regs.d.eflags, 1u <<CARRY_FLAG_BIT, 0); // CARRY FLAG
    bit_write(machine->regs.d.eflags, 1u <<OVERFLOW_FLAG_BIT, 0); // OVERFLOW FLAG
    uint8_t parrity = result & 1u;
    for(uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, result >> (width - 1u)); //SIGN FLAG
    //AUX MARKED AS UNDEFINED IN INTEL DOCUMENTATION
    if(width == 8) *((uint8_t*)dest) = result & 0xFFu;
    else if(width == 16) *((uint16_t*)dest) = result & 0xFFFFu;
    else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
    return OK;
}

int16_t perform_xor(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry)
{
    uint32_t result = 0;
    if(width == 8)
        result = *((uint8_t*)dest) ^ *((uint8_t*)source);
    else if(width == 16)
        result = *((uint16_t*)dest) ^ *((uint16_t*)source);
    else if(width == 32)
        result = *((uint32_t*)dest) ^ *((uint32_t*)source);
    else return BAD_WIDTH;
    bit_write(machine->regs.d.eflags, 1u <<CARRY_FLAG_BIT, 0); // CARRY FLAG
    bit_write(machine->regs.d.eflags, 1u <<OVERFLOW_FLAG_BIT, 0); // OVERFLOW FLAG
    uint8_t parrity = result & 1u;
    for(uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, result >> (width - 1u)); //SIGN FLAG
    //AUX MARKED AS UNDEFINED IN INTEL DOCUMENTATION
    if(width == 8) *((uint8_t*)dest) = result & 0xFFu;
    else if(width == 16) *((uint16_t*)dest) = result & 0xFFFFu;
    else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
    return OK;
}

int16_t perform_cmp(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry)
{
    uint64_t result = 0;
    uint32_t dest_before; //for overflow flag checking
    uint32_t source_before;
    if(width == 8){
        dest_before = *((uint8_t*)dest);
        source_before = *((uint8_t*)source);
    } else if(width == 16){
        dest_before = *((uint16_t*)dest);
        source_before = *((uint16_t*)source);
    } else if(width == 32){
        dest_before = *((uint32_t*)dest);
        source_before = *((uint32_t*)source);
    } else return BAD_WIDTH;
    result = dest_before - source_before;
    bit_write(machine->regs.d.eflags, 1u <<CARRY_FLAG_BIT, (result >> width) ? 1 : 0); // CARRY FLAG
    bit_write(machine->regs.d.eflags, 1u <<AUX_CARRY_FLAG_BIT, (((dest_before & 0xfu) - (source_before & 0xfu)) >> 4u) ? 1: 0); //AUX CARRY FLAG
    uint8_t parrity = result & 1u;
    for(uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, result >> (width - 1u)); //SIGN FLAG
    bit_write(machine->regs.d.eflags, 1u <<OVERFLOW_FLAG_BIT, ((result >> (width - 1u)) != (dest_before >> (width - 1u)))); //OVERFLOW FLAG
    return OK;
}

int16_t perform_ror(v8086* machine, void* dest, uint8_t arg, uint8_t width)
{
    uint16_t temp_flags = 0;
    if(arg == 0) return OK;
    if(width == 8)
            __asm__ __volatile__("rorb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t*) dest)) : "a" (*((uint8_t*) dest)), "c" (arg));
    else if(width == 16)
            __asm__ __volatile__("rorw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t*) dest)) : "a" (*((uint16_t*) dest)), "c" (arg));
    else if(width == 32)
            __asm__ __volatile__("rorl %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t*) dest)) : "a" (*((uint32_t*) dest)), "c" (arg));
    else return BAD_WIDTH;
    if(arg == 1) bit_write(machine->regs.w.flags, 1u <<OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u <<OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT, bit_get(temp_flags, 1u <<CARRY_FLAG_BIT) != 0);
    return OK;
}

int16_t perform_rol(v8086* machine, void* dest, uint8_t arg, uint8_t width)
{
    uint16_t temp_flags;
    if(arg == 0) return OK;
    if(width == 8)
            __asm__ __volatile__("rolb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t*) dest)) : "a" (*((uint8_t*) dest)), "c" (arg));
    else if(width == 16)
            __asm__ __volatile__("rolw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t*) dest)) : "a" (*((uint16_t*) dest)), "c" (arg));
    else if(width == 32)
            __asm__ __volatile__("roll %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t*) dest)) : "a" (*((uint32_t*) dest)), "c" (arg));
    if(arg == 1) bit_write(machine->regs.w.flags, 1u <<OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u <<OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT, bit_get(temp_flags, 1u <<CARRY_FLAG_BIT) != 0);
    return OK;
}

int16_t perform_rcl(v8086* machine, void* dest, uint8_t arg, uint8_t width)
{
    uint16_t temp_flags;
    if(arg == 0) return OK;
    if(width == 8)
            __asm__ __volatile__("rclb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t*) dest)) : "a" (*((uint8_t*) dest)), "c" (arg));
    else if(width == 16)
            __asm__ __volatile__("rclw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t*) dest)) : "a" (*((uint16_t*) dest)), "c" (arg));
    else if(width == 32)
            __asm__ __volatile__("rcll %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t*) dest)) : "a" (*((uint32_t*) dest)), "c" (arg));
    else return BAD_WIDTH;
    if(arg == 1) bit_write(machine->regs.w.flags, 1u <<OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u <<OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT, bit_get(temp_flags, 1u <<CARRY_FLAG_BIT) != 0);
    return OK;
}

int16_t perform_rcr(v8086* machine, void* dest, uint8_t arg, uint8_t width)
{
    uint16_t temp_flags;
    if(arg == 0) return OK;
    if(width == 8)
            __asm__ __volatile__("rcrb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t*) dest)) : "a" (*((uint8_t*) dest)), "c" (arg));
    else if(width == 16)
            __asm__ __volatile__("rcrw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t*) dest)) : "a" (*((uint16_t*) dest)), "c" (arg));
    else if(width == 32)
            __asm__ __volatile__("rcrl %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t*) dest)) : "a" (*((uint32_t*) dest)), "c" (arg));
    else return BAD_WIDTH;
    if(arg == 1) bit_write(machine->regs.w.flags, 1u <<OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u <<OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT, bit_get(temp_flags, 1u <<CARRY_FLAG_BIT) != 0);
    return OK;
}

int16_t perform_shl(v8086* machine, void* dest, uint8_t arg, uint8_t width)
{
    uint16_t temp_flags;
    if(arg == 0) return OK;
    if(width == 8)
            __asm__ __volatile__("shlb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t*) dest)) : "a" (*((uint8_t*) dest)), "c" (arg));
    else if(width == 16)
            __asm__ __volatile__("shlw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t*) dest)) : "a" (*((uint16_t*) dest)), "c" (arg));
    else if(width == 32)
            __asm__ __volatile__("shll %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t*) dest)) : "a" (*((uint32_t*) dest)), "c" (arg));
    else return BAD_WIDTH;
    if(arg == 1) bit_write(machine->regs.w.flags, 1u <<OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u <<OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT, bit_get(temp_flags, 1u <<CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<SIGN_FLAG_BIT, bit_get(temp_flags, 1u <<SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<ZERO_FLAG_BIT, bit_get(temp_flags, 1u <<ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<PARITY_FLAG_BIT, bit_get(temp_flags, 1u <<PARITY_FLAG_BIT) != 0);
    return OK;
}

int16_t perform_shr(v8086* machine, void* dest, uint8_t arg, uint8_t width)
{
    uint16_t temp_flags;
    if(arg == 0) return OK;
    if(width == 8)
            __asm__ __volatile__("shrb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t*) dest)) : "a" (*((uint8_t*) dest)), "c" (arg));
    else if(width == 16)
            __asm__ __volatile__("shrw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t*) dest)) : "a" (*((uint16_t*) dest)), "c" (arg));
    else if(width == 32)
            __asm__ __volatile__("shrl %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t*) dest)) : "a" (*((uint32_t*) dest)), "c" (arg));
    else return BAD_WIDTH;
    if(arg == 1) bit_write(machine->regs.w.flags, 1u <<OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u <<OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT, bit_get(temp_flags, 1u <<CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<SIGN_FLAG_BIT, bit_get(temp_flags, 1u <<SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<ZERO_FLAG_BIT, bit_get(temp_flags, 1u <<ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<PARITY_FLAG_BIT, bit_get(temp_flags, 1u <<PARITY_FLAG_BIT) != 0);
    return OK;
}

int16_t perform_sar(v8086* machine, void* dest, uint8_t arg, uint8_t width)
{
    uint16_t temp_flags;
    if(arg == 0) return OK;
    if(width == 8)
            __asm__ __volatile__("sarb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t*) dest)) : "a" (*((uint8_t*) dest)), "c" (arg));
    else if(width == 16)
            __asm__ __volatile__("sarw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t*) dest)) : "a" (*((uint16_t*) dest)), "c" (arg));
    else if(width == 32)
            __asm__ __volatile__("sarl %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t*) dest)) : "a" (*((uint32_t*) dest)), "c" (arg));
    else return BAD_WIDTH;
    if(arg == 1) bit_write(machine->regs.w.flags, 1u <<OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u <<OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT, bit_get(temp_flags, 1u <<CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<SIGN_FLAG_BIT, bit_get(temp_flags, 1u <<SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<ZERO_FLAG_BIT, bit_get(temp_flags, 1u <<ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<PARITY_FLAG_BIT, bit_get(temp_flags, 1u <<PARITY_FLAG_BIT) != 0);
    return OK;
}

int16_t perform_neg(v8086* machine, void* source, uint8_t width)
{
    uint16_t temp_flags;
    if(width == 8)
            __asm__ __volatile__("negb %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t*) source)) : "a" (*((uint8_t*) source)));
    else if(width == 16)
            __asm__ __volatile__("negw %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t*) source)) : "a" (*((uint16_t*) source)));
    else if(width == 32)
            __asm__ __volatile__("negl %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t*) source)) : "a" (*((uint16_t*) source)));
    else return BAD_WIDTH;
    bit_write(machine->regs.w.flags, 1u <<OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u <<OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT, bit_get(temp_flags, 1u <<CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<SIGN_FLAG_BIT, bit_get(temp_flags, 1u <<SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<ZERO_FLAG_BIT, bit_get(temp_flags, 1u <<ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<PARITY_FLAG_BIT, bit_get(temp_flags, 1u <<PARITY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<AUX_CARRY_FLAG_BIT, bit_get(temp_flags, 1u <<AUX_CARRY_FLAG_BIT) != 0);

    return OK;
}

int16_t perform_multiplication(v8086* machine, void* source, uint8_t signed_mul, uint8_t width)
{
    uint16_t temp_flags;
    if(signed_mul)
    {
        if(width == 8)
        {
            __asm__ __volatile__(
            "movb %%dl, %%al; imul %%cl; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.w.ax) : "d" (machine->regs.h.al), "c" (*((uint8_t*) source))
            );
        }
        else if(width == 16)
        {
            __asm__ __volatile__(
            "movw %%dx, %%ax; imul %%cx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.d.eax) : "d" (machine->regs.w.ax), "c" (*((uint16_t*) source))
            );
        }
        else if(width == 32)
        {
            __asm__ __volatile__(
            "movl %%edx, %%eax; imul %%ecx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.d.eax), "=d"(machine->regs.d.edx) : "d" (machine->regs.d.eax), "c" (*((uint32_t*) source))
            );
        }
        else return BAD_WIDTH;
    }
    else
    {
        if(width == 8)
        {
            __asm__ __volatile__(
            "movb %%dl, %%al; mul %%cl; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.w.ax) : "d" (machine->regs.h.al), "c" (*((uint8_t*) source))
            );
        }
        else if(width == 16)
        {
            __asm__ __volatile__(
            "movw %%dx, %%ax; mul %%cx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.d.eax) : "d" (machine->regs.w.ax), "c" (*((uint16_t*) source))
            );
        }
        else if(width == 32)
        {
            __asm__ __volatile__(
            "movl %%edx, %%eax; mul %%ecx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.d.eax), "=d"(machine->regs.d.edx) : "d" (machine->regs.d.eax), "c" (*((uint32_t*) source))
            );
        }
        else return BAD_WIDTH;
    }
    bit_write(machine->regs.w.flags, 1u <<OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u <<OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT, bit_get(temp_flags, 1u <<CARRY_FLAG_BIT) != 0);
    return OK;
}

int16_t perform_division(v8086* machine, void* source, uint8_t signed_div, uint8_t width)
{
    uint16_t temp_flags;
    if(signed_div)
    {
        if(width == 8)
        {
            __asm__ __volatile__(
            "movb %%dl, %%al; idiv %%cl; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.w.ax) : "d" (machine->regs.h.al), "c" (*((uint8_t*) source))
            );
        }
        else if(width == 16)
        {
            __asm__ __volatile__(
            "movw %%dx, %%ax; idiv %%cx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.d.eax) : "d" (machine->regs.w.ax), "c" (*((uint16_t*) source))
            );
        }
        else if(width == 32)
        {
            __asm__ __volatile__(
            "movl %%edx, %%eax; idiv %%ecx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.d.eax), "=d"(machine->regs.d.edx) : "d" (machine->regs.d.eax), "c" (*((uint32_t*) source))
            );
        }
        else return BAD_WIDTH;
    }
    else
    {
        if(width == 8)
        {
            __asm__ __volatile__(
            "movb %%dl, %%al; div %%cl; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.w.ax) : "d" (machine->regs.h.al), "c" (*((uint8_t*) source))
            );
        }
        else if(width == 16)
        {
            __asm__ __volatile__(
            "movw %%dx, %%ax; div %%cx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.d.eax) : "d" (machine->regs.w.ax), "c" (*((uint16_t*) source))
            );
        }
        else if(width == 32)
        {
            __asm__ __volatile__(
            "movl %%edx, %%eax; div %%ecx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.d.eax), "=d"(machine->regs.d.edx) : "d" (machine->regs.d.eax), "c" (*((uint32_t*) source))
            );
        }
        else return BAD_WIDTH;
    }
    return OK;
}

int16_t perform_test(v8086* machine, void* source, void* dest, uint8_t width)
{
    uint32_t result;

    if(width == 8)
        result = *((uint8_t*) source) & *((uint8_t*) dest);
    else if(width == 16)
        result = *((uint16_t*) source) & *((uint16_t*) dest);
    else if(width == 32)
        result = *((uint32_t*) source) & *((uint32_t*) dest);
    else return BAD_WIDTH;

    uint8_t parrity = result & 1u;
    for(uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, result >> (width - 1u)); //SIGN FLAG
    return OK;
}

int16_t perform_inc(v8086* machine, void* dest, uint8_t width)
{
    uint64_t result = 0;
    uint32_t dest_before;

    if (width == 8) dest_before = *((uint8_t*) dest);
    else if(width == 16) dest_before = *((uint16_t*)dest);
    else if(width == 32) dest_before = *((uint32_t*)dest);
    else return BAD_WIDTH;

    result = dest_before + 1;

    uint8_t parrity = result & 1u;
    for(uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine->regs.d.eflags, 1u <<AUX_CARRY_FLAG_BIT, (((dest_before & 0xfu) + 1u) >> 4u) ? 1: 0); //AUX CARRY FLAG
    bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, result >> (width - 1u)); //SIGN FLAG
    bit_write(machine->regs.d.eflags, 1u <<OVERFLOW_FLAG_BIT, ((result >> (width - 1u)) != (dest_before >> (width - 1u)))); //OVERFLOW FLAG

    if(width == 8) *((uint8_t*)dest) = result & 0xFFu;
    else if(width == 16) *((uint16_t*)dest) = result & 0xFFFFu;
    else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
    return OK;
}

int16_t perform_dec(v8086* machine, void* dest, uint8_t width)
{
    uint64_t result = 0;
    uint32_t dest_before;

    if (width == 8) dest_before = *((uint8_t*) dest);
    else if(width == 16) dest_before = *((uint16_t*)dest);
    else if(width == 32) dest_before = *((uint32_t*)dest);
    else return BAD_WIDTH;

    result = dest_before - 1;

    uint8_t parrity = result & 1u;
    for(uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine->regs.d.eflags, 1u <<AUX_CARRY_FLAG_BIT, (((dest_before & 0xfu) - 1u) >> 4u) ? 1: 0); //AUX CARRY FLAG
    bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, result >> (width - 1u)); //SIGN FLAG
    bit_write(machine->regs.d.eflags, 1u <<OVERFLOW_FLAG_BIT, ((result >> (width - 1u)) != (dest_before >> (width - 1u)))); //OVERFLOW FLAG

    if(width == 8) *((uint8_t*)dest) = result & 0xFFu;
    else if(width == 16) *((uint16_t*)dest) = result & 0xFFFFu;
    else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
    return OK;
}

int16_t perform_artihmetic_or_logical_instruction(v8086* machine, uint8_t recalculated_opcode, uint32_t carry, int16_t (*operation)(v8086*, void*, void*, uint8_t, uint32_t))
{
    //Maybe Mod/RM, Can be Immediate
    uint8_t mod_rm_or_immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t reg = get_reg(mod_rm_or_immediate);
    uint8_t width = 16;
    void* source = NULL;
    void* dest = NULL;
    if(!(recalculated_opcode % 2)) width = 8; //Odd Opcode means 16 or 32 bit operands
    else if(machine->internal_state.operand_32_bit) width = 32;
    switch(recalculated_opcode)
    {
        case 0: //OPERATION r/m8, r8
            source = get_byte_register(machine, reg);
            dest = get_memory_from_mode(machine, mod_rm_or_immediate, 8);
            if(source == NULL) return UNDEFINED_REGISTER;
            if(dest == NULL) return UNABLE_GET_MEMORY;
            break;
        case 1: //OPERATION r/m32, r32 or OPERATION r/m16, r16
            source = get_variable_length_register(machine, reg, width);
            dest = get_memory_from_mode(machine, mod_rm_or_immediate, width);
            if(source == NULL) return UNDEFINED_REGISTER;
            if(dest == NULL) return UNABLE_GET_MEMORY;
            break;
        case 2: //OPERATION r8, r/m8
            dest = get_byte_register(machine, reg);
            source = get_memory_from_mode(machine, mod_rm_or_immediate, 8);
            if(dest == NULL) return UNDEFINED_REGISTER;
            if(source == NULL) return UNABLE_GET_MEMORY;
            break;
        case 3: //OPERATION r32, r/m32 or OPERATION r16, r/m16
            dest = get_variable_length_register(machine, reg, width);
            source = get_memory_from_mode(machine, mod_rm_or_immediate, width);
            if(dest == NULL) return UNDEFINED_REGISTER;
            if(source == NULL) return UNABLE_GET_MEMORY;
            break;
        case 4: //OPERATION AL, imm8
            dest = get_byte_register(machine, AL);
            source = &(mod_rm_or_immediate);
            if(dest == NULL) return UNDEFINED_REGISTER;
            break;
        case 5: //OPERATION EAX, imm32 or OPERATION AX, imm16
            dest = get_variable_length_register(machine, AX, width);
            if(dest == NULL) return UNDEFINED_REGISTER;
            if(width == 32)
                source = get_dword_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip - 1 + machine->internal_state.IPOffset));
            else
                source = get_word_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip - 1 + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += ((width / 8) - 1);
            break;
        default:
            return UNDEFINED_RECALCULATED_OPCODE;
    }
    int16_t status = operation(machine, dest, source, width, carry);
    if(status) return status;
    return OK;
}
int16_t perform_arithmetic_or_logical_instruction_group(v8086* machine, uint8_t recalculated_opcode, uint8_t mod_rm, uint32_t carry, int16_t (*operation)(v8086*, void*, void*, uint8_t, uint32_t))
{
    void* dest = NULL;
    uint8_t width;
    uint32_t immediate;
    int32_t signed_immediate;
    switch(recalculated_opcode)
    {
        case 0: //OPERATION rm8, imm8
        case 2: //OPERATION rm8, imm8
            width = 8;
            dest = get_memory_from_mode(machine, mod_rm, width);
            immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 1;
            break;
        case 1: //OPERATION rm16, imm16 or rm32, imm32
            if(machine->internal_state.operand_32_bit) {
                width = 32;
                immediate = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 4;
            }
            else
            {
                width = 16;
                immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 2;
            }
            dest = get_memory_from_mode(machine, mod_rm, width);
            break;
        case 3: //OPERATION rm16, imm8, or rm32, imm8
            if(machine->internal_state.operand_32_bit) width = 32;
            else width = 16;
            signed_immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 1;
            dest = get_memory_from_mode(machine, mod_rm, width);
            break;
        default:
            return UNDEFINED_RECALCULATED_OPCODE;
    }
    if(dest == NULL) return UNABLE_GET_MEMORY;
    int16_t status;
    if(recalculated_opcode == 3) status = operation(machine, dest, &signed_immediate, width, carry);
    else status = operation(machine, dest, &immediate, width, carry);
    if(status) return status;
    return OK;
}

int16_t perform_inc_dec(v8086* machine, uint8_t opcode, bool dec)
{
    uint8_t width = 16;
    void* dest = NULL;
    if(machine->internal_state.operand_32_bit) width=32;
    dest = get_variable_length_register(machine, opcode & 7u, width);
    if(dest == NULL) return UNDEFINED_REGISTER;

    if(!dec)
        return perform_inc(machine, dest, width);
    return perform_dec(machine, dest, width);
}

int16_t execute_test(v8086* machine, uint8_t opcode)
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
    return perform_test(machine, source, dest, width);
}