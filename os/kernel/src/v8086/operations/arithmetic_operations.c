#include <stdint.h>
#include <v8086/v8086.h>
#include <v8086/memory_operations.h>
#include <v8086/mod_rm_parsing.h>
#include <stddef.h>
#include <stdbool.h>
#include "arithmetic_operations.h"
#include "internal_funcs.h"

int16_t perform_adding(v8086 *machine, void *dest, void *source, uint8_t width, uint32_t carry) {
    uint16_t temp_flags = 0;
    if (width == 8)
        __asm__ __volatile__("clc; test %%edx, %%edx; jz calculate%=; stc; calculate%=:adcb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t *) dest)) : "a" (*((uint8_t *) dest)), "c" (*((uint8_t *) source)), "d" (carry));
    else if (width == 16)
        __asm__ __volatile__("clc; test %%edx, %%edx; jz calculate%=; stc; calculate%=:adcw %%cx, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) dest)) : "a" (*((uint16_t *) dest)), "c" (*((uint16_t *) source)), "d" (carry));
    else if (width == 32)
        __asm__ __volatile__("clc; test %%edx, %%edx; jz calculate%=; stc; calculate%=:adcl %%ecx, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) dest)) : "a" (*((uint32_t *) dest)), "c" (*((uint32_t *) source)), "d" (carry));
    else return V8086_BAD_WIDTH;
    bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << AUX_CARRY_FLAG_BIT, bit_get(temp_flags, 1u << AUX_CARRY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_subtracting(v8086 *machine, void *dest, void *source, uint8_t width, uint32_t carry) {
    uint16_t temp_flags = 0;
    if (width == 8)
        __asm__ __volatile__("clc; test %%edx, %%edx; jz calculate%=; stc; calculate%=:sbbb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t *) dest)) : "a" (*((uint8_t *) dest)), "c" (*((uint8_t *) source)), "d" (carry));
    else if (width == 16)
        __asm__ __volatile__("clc; test %%edx, %%edx; jz calculate%=; stc; calculate%=:sbbw %%cx, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) dest)) : "a" (*((uint16_t *) dest)), "c" (*((uint16_t *) source)), "d" (carry));
    else if (width == 32)
        __asm__ __volatile__("clc; test %%edx, %%edx; jz calculate%=; stc; calculate%=:sbbl %%ecx, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) dest)) : "a" (*((uint32_t *) dest)), "c" (*((uint32_t *) source)), "d" (carry));
    else return V8086_BAD_WIDTH;
    bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << AUX_CARRY_FLAG_BIT, bit_get(temp_flags, 1u << AUX_CARRY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_or(v8086 *machine, void *dest, void *source, uint8_t width, uint32_t carry) {
    uint16_t temp_flags = 0;
    if (width == 8)
        __asm__ __volatile__("orb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t *) dest)) : "a" (*((uint8_t *) dest)), "c" (*((uint8_t *) source)));
    else if (width == 16)
        __asm__ __volatile__("orw %%cx, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) dest)) : "a" (*((uint16_t *) dest)), "c" (*((uint16_t *) source)));
    else if (width == 32)
        __asm__ __volatile__("orl %%ecx, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) dest)) : "a" (*((uint32_t *) dest)), "c" (*((uint32_t *) source)));
    else return V8086_BAD_WIDTH;
    bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << AUX_CARRY_FLAG_BIT, bit_get(temp_flags, 1u << AUX_CARRY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_and(v8086 *machine, void *dest, void *source, uint8_t width, uint32_t carry) {
    uint16_t temp_flags = 0;
    if (width == 8)
        __asm__ __volatile__("andb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t *) dest)) : "a" (*((uint8_t *) dest)), "c" (*((uint8_t *) source)));
    else if (width == 16)
        __asm__ __volatile__("andw %%cx, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) dest)) : "a" (*((uint16_t *) dest)), "c" (*((uint16_t *) source)));
    else if (width == 32)
        __asm__ __volatile__("andl %%ecx, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) dest)) : "a" (*((uint32_t *) dest)), "c" (*((uint32_t *) source)));
    else return V8086_BAD_WIDTH;
    bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << AUX_CARRY_FLAG_BIT, bit_get(temp_flags, 1u << AUX_CARRY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_xor(v8086 *machine, void *dest, void *source, uint8_t width, uint32_t carry) {
    uint16_t temp_flags = 0;
    if (width == 8)
        __asm__ __volatile__("xorb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t *) dest)) : "a" (*((uint8_t *) dest)), "c" (*((uint8_t *) source)));
    else if (width == 16)
        __asm__ __volatile__("xorw %%cx, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) dest)) : "a" (*((uint16_t *) dest)), "c" (*((uint16_t *) source)));
    else if (width == 32)
        __asm__ __volatile__("xorl %%ecx, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) dest)) : "a" (*((uint32_t *) dest)), "c" (*((uint32_t *) source)));
    else return V8086_BAD_WIDTH;
    bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << AUX_CARRY_FLAG_BIT, bit_get(temp_flags, 1u << AUX_CARRY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_cmp(v8086 *machine, void *dest, void *source, uint8_t width, uint32_t carry) {
    uint16_t temp_flags = 0;
    if (width == 8)
            __asm__ __volatile__("cmpb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags) : "a" (*((uint8_t *) dest)), "c" (*((uint8_t *) source)));
    else if (width == 16)
            __asm__ __volatile__("cmpw %%cx, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags) : "a" (*((uint16_t *) dest)), "c" (*((uint16_t *) source)));
    else if (width == 32)
            __asm__ __volatile__("cmpl %%ecx, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags) : "a" (*((uint32_t *) dest)), "c" (*((uint32_t *) source)));
    else return V8086_BAD_WIDTH;
    bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << AUX_CARRY_FLAG_BIT, bit_get(temp_flags, 1u << AUX_CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_ror(v8086 *machine, void *dest, uint8_t arg, uint8_t width) {
    uint16_t temp_flags = 0;
    if (arg == 0) return V8086_OK;
    if (width == 8)
            __asm__ __volatile__("rorb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t *) dest)) : "a" (*((uint8_t *) dest)), "c" (arg));
    else if (width == 16)
            __asm__ __volatile__("rorw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) dest)) : "a" (*((uint16_t *) dest)), "c" (arg));
    else if (width == 32)
            __asm__ __volatile__("rorl %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) dest)) : "a" (*((uint32_t *) dest)), "c" (arg));
    else return V8086_BAD_WIDTH;
    if (arg == 1)
        bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_rol(v8086 *machine, void *dest, uint8_t arg, uint8_t width) {
    uint16_t temp_flags;
    if (arg == 0) return V8086_OK;
    if (width == 8)
            __asm__ __volatile__("rolb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t *) dest)) : "a" (*((uint8_t *) dest)), "c" (arg));
    else if (width == 16)
            __asm__ __volatile__("rolw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) dest)) : "a" (*((uint16_t *) dest)), "c" (arg));
    else if (width == 32)
            __asm__ __volatile__("roll %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) dest)) : "a" (*((uint32_t *) dest)), "c" (arg));
    else return V8086_BAD_WIDTH;
    if (arg == 1)
        bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_rcl(v8086 *machine, void *dest, uint8_t arg, uint8_t width) {
    uint16_t temp_flags;
    if (arg == 0) return V8086_OK;

    __asm__("pushfw; pop %%bx" : "=b" (temp_flags));
    bit_write(temp_flags, 1u << CARRY_FLAG_BIT, (bit_get(machine->regs.w.flags, 1u << CARRY_FLAG_BIT) ? 1 : 0));

    if (width == 8)
            __asm__ __volatile__("push %%bx; popfw; rclb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t *) dest)) : "a" (*((uint8_t *) dest)), "b"(temp_flags), "c" (arg));
    else if (width == 16)
            __asm__ __volatile__("push %%bx; popfw; rclw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) dest)) : "a" (*((uint16_t *) dest)), "b"(temp_flags), "c" (arg));
    else if (width == 32)
            __asm__ __volatile__("push %%bx; popfw; rcll %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) dest)) : "a" (*((uint32_t *) dest)), "b"(temp_flags), "c" (arg));
    else return V8086_BAD_WIDTH;
    if (arg == 1)
        bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_rcr(v8086 *machine, void *dest, uint8_t arg, uint8_t width) {
    uint16_t temp_flags;
    if (arg == 0) return V8086_OK;

    __asm__("pushfw; pop %%bx" : "=b" (temp_flags));
    bit_write(temp_flags, 1u << CARRY_FLAG_BIT, (bit_get(machine->regs.w.flags, 1u << CARRY_FLAG_BIT) ? 1 : 0));

    if (width == 8)
            __asm__ __volatile__("push %%bx; popfw; rcrb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t *) dest)) : "a" (*((uint8_t *) dest)), "b"(temp_flags), "c" (arg));
    else if (width == 16)
            __asm__ __volatile__("push %%bx; popfw; rcrw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) dest)) : "a" (*((uint16_t *) dest)), "b"(temp_flags), "c" (arg));
    else if (width == 32)
            __asm__ __volatile__("push %%bx; popfw; rcrl %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) dest)) : "a" (*((uint32_t *) dest)), "b"(temp_flags), "c" (arg));
    else return V8086_BAD_WIDTH;
    if (arg == 1)
        bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_shl(v8086 *machine, void *dest, uint8_t arg, uint8_t width) {
    uint16_t temp_flags;
    if (arg == 0) return V8086_OK;
    if (width == 8)
            __asm__ __volatile__("shlb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t *) dest)) : "a" (*((uint8_t *) dest)), "c" (arg));
    else if (width == 16)
            __asm__ __volatile__("shlw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) dest)) : "a" (*((uint16_t *) dest)), "c" (arg));
    else if (width == 32)
            __asm__ __volatile__("shll %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) dest)) : "a" (*((uint32_t *) dest)), "c" (arg));
    else return V8086_BAD_WIDTH;
    if((arg & 0b11111) == 0) return 0;
    if (arg == 1)
        bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_shr(v8086 *machine, void *dest, uint8_t arg, uint8_t width) {
    uint16_t temp_flags;
    if (arg == 0) return V8086_OK;
    if (width == 8)
            __asm__ __volatile__("shrb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t *) dest)) : "a" (*((uint8_t *) dest)), "c" (arg));
    else if (width == 16)
            __asm__ __volatile__("shrw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) dest)) : "a" (*((uint16_t *) dest)), "c" (arg));
    else if (width == 32)
            __asm__ __volatile__("shrl %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) dest)) : "a" (*((uint32_t *) dest)), "c" (arg));
    else return V8086_BAD_WIDTH;
    if((arg & 0b11111) == 0) return 0;
    if (arg == 1)
        bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_sar(v8086* machine, void* dest, uint8_t arg, uint8_t width)
{
    uint16_t temp_flags;
    if(arg == 0) return 0;
    if(width == 8)
            __asm__ __volatile__("sarb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t*) dest)) : "a" (*((uint8_t*) dest)), "c" (arg));
    else if(width == 16)
            __asm__ __volatile__("sarw %%cl, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t*) dest)) : "a" (*((uint16_t*) dest)), "c" (arg));
    else if(width == 32)
            __asm__ __volatile__("sarl %%cl, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t*) dest)) : "a" (*((uint32_t*) dest)), "c" (arg));
    else return -1;
    if((arg & 0b11111) == 0) return 0;
    if(arg == 1) bit_write(machine->regs.w.flags, 1u<<OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u<<OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u<<CARRY_FLAG_BIT, bit_get(temp_flags, 1u<<CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u<<SIGN_FLAG_BIT, bit_get(temp_flags, 1u<<SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u<<ZERO_FLAG_BIT, bit_get(temp_flags, 1u<<ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u<<PARITY_FLAG_BIT, bit_get(temp_flags, 1u<<PARITY_FLAG_BIT) != 0);
    return 0;
}

int16_t perform_shld(v8086 *machine, void *rm, void* reg, uint8_t arg, uint8_t width) {
    uint16_t temp_flags;
    if (arg == 0) return V8086_OK;
    if (width == 16)
            __asm__ __volatile__("shldw %%cl, %%dx, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) rm)), "=d" (*((uint16_t *) reg)) : "a" (*((uint16_t *) rm)), "d" (*((uint16_t *) reg)), "c" (arg));
    else if (width == 32)
            __asm__ __volatile__("shld %%cl, %%edx, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) rm)), "=d" (*((uint16_t *) reg)) : "a" (*((uint32_t *) rm)), "d" (*((uint16_t *) reg)), "c" (arg));
    else return V8086_BAD_WIDTH;
    if (arg == 1)
        bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_shrd(v8086 *machine, void *rm, void* reg, uint8_t arg, uint8_t width) {
    uint16_t temp_flags;
    if (arg == 0) return V8086_OK;
    if (width == 16)
            __asm__ __volatile__("shrdw %%cl, %%dx, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) rm)), "=d" (*((uint16_t *) reg)) : "a" (*((uint16_t *) rm)), "d" (*((uint16_t *) reg)), "c" (arg));
    else if (width == 32)
            __asm__ __volatile__("shrd %%cl, %%edx, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) rm)), "=d" (*((uint16_t *) reg)) : "a" (*((uint32_t *) rm)), "d" (*((uint16_t *) reg)), "c" (arg));
    else return V8086_BAD_WIDTH;
    if (arg == 1)
        bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_neg(v8086 *machine, void *source, uint8_t width) {
    uint16_t temp_flags;
    if (width == 8)
            __asm__ __volatile__("negb %%al; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint8_t *) source)) : "a" (*((uint8_t *) source)));
    else if (width == 16)
            __asm__ __volatile__("negw %%ax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint16_t *) source)) : "a" (*((uint16_t *) source)));
    else if (width == 32)
            __asm__ __volatile__("negl %%eax; pushfw; pop %%bx;" : "=b" (temp_flags), "=a" (*((uint32_t *) source)) : "a" (*((uint32_t *) source)));
    else return V8086_BAD_WIDTH;
    bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << AUX_CARRY_FLAG_BIT, bit_get(temp_flags, 1u << AUX_CARRY_FLAG_BIT) != 0);

    return V8086_OK;
}

int16_t perform_multiplication(v8086 *machine, void *source, uint8_t signed_mul, uint8_t width) {
    uint16_t temp_flags;
    if (signed_mul) {
        if (width == 8) {
            __asm__ __volatile__(
            "movb %%dl, %%al; imul %%cl; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.w.ax) : "d" (machine->regs.h.al), "c" (*((uint8_t *) source))
            );
        } else if (width == 16) {
            __asm__ __volatile__(
            "movw %%dx, %%ax; imul %%cx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.w.ax), "=d"(machine->regs.x.dx) : "d" (machine->regs.w.ax), "c" (*((uint16_t *) source))
            );
        } else if (width == 32) {
            __asm__ __volatile__(
            "movl %%edx, %%eax; imul %%ecx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.d.eax), "=d"(machine->regs.d.edx) : "d" (machine->regs.d.eax), "c" (*((uint32_t *) source))
            );
        } else return V8086_BAD_WIDTH;
    } else {
        if (width == 8) {
            __asm__ __volatile__(
            "movb %%dl, %%al; mul %%cl; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.w.ax) : "d" (machine->regs.h.al), "c" (*((uint8_t *) source))
            );
        } else if (width == 16) {
            __asm__ __volatile__(
            "movw %%dx, %%ax; mul %%cx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.w.ax), "=d"(machine->regs.x.dx) : "d" (machine->regs.w.ax), "c" (*((uint16_t *) source))
            );
        } else if (width == 32) {
            __asm__ __volatile__(
            "movl %%edx, %%eax; mul %%ecx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=a" (machine->regs.d.eax), "=d"(machine->regs.d.edx) : "d" (machine->regs.d.eax), "c" (*((uint32_t *) source))
            );
        } else return V8086_BAD_WIDTH;
    }
    bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t
perform_multiplication_3_byte(v8086 *machine, void *dest, void *source, void *imm, uint8_t signed_mul, uint8_t width,
                              uint8_t second_width) {
    uint16_t temp_flags;
    if (width == 16) {
        if (second_width == 8) {
            int16_t a_imm = *((int8_t *) imm);
            __asm__ __volatile__(
            "imul %%cx, %%dx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=d" (*((uint16_t *) dest)) : "d" (*((uint16_t *) source)), "c" (a_imm)
            );
        } else if (second_width == 16) {
            int16_t a_imm = *((int16_t *) imm);
            __asm__ __volatile__(
            "imul %%cx, %%dx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=d" (*((uint16_t *) dest)) : "d" (*((uint16_t *) source)), "c" (a_imm)
            );
        } else return V8086_BAD_WIDTH;
    } else if (width == 32) {
        if (second_width == 8) {
            int32_t a_imm = *((int8_t *) imm);
            __asm__ __volatile__(
            "imul %%ecx, %%edx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=d" (*((uint32_t *) dest)) : "d" (*((uint32_t *) source)), "c" (a_imm)
            );
        } else if (second_width == 32) {
            int32_t a_imm = *((int32_t *) imm);
            __asm__ __volatile__(
            "imul %%ecx, %%edx; pushfw; pop %%bx;"
            : "=b" (temp_flags), "=d" (*((uint32_t *) dest)) : "d" (*((uint32_t *) source)), "c" (a_imm)
            );
        } else return V8086_BAD_WIDTH;
    } else return V8086_BAD_WIDTH;

    bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_division(v8086 *machine, void *source, uint8_t signed_div, uint8_t width) {
    uint16_t temp_flags;
    if (signed_div) {
        if (width == 8) {
            if(*((int8_t *) source) == 0)
                return V8086_DIVISION_BY_ZERO;
            int16_t temp = (int16_t) machine->regs.w.ax / (int16_t)(*((int8_t *) source));
            if((temp > (int8_t)0x7f) || (temp < (int8_t)0x80)) return V8086_DIVISION_OVERFLOW;
            machine->regs.h.ah = (int16_t) machine->regs.w.ax % *((int8_t *) source);
            machine->regs.h.al = temp;
        } else if (width == 16) {
            if(*((uint16_t *) source) == 0)
                return V8086_DIVISION_BY_ZERO;
            int32_t dividend = ((uint32_t)(machine->regs.x.dx) << 16u) | machine->regs.x.ax;
            int32_t temp = dividend / *((int16_t *) source);
            if((temp > (int16_t)0x7fff) || (temp < (int16_t)0x8000)) return V8086_DIVISION_OVERFLOW;
            machine->regs.x.dx = dividend % *((int16_t *) source);
            machine->regs.x.ax = temp;
        } else if (width == 32) {
            if(*((uint32_t *) source) == 0)
                return V8086_DIVISION_BY_ZERO;
            int64_t dividend = ((uint64_t)(machine->regs.d.edx) << 32u) | machine->regs.d.eax;
            int64_t temp = dividend / *((int32_t *) source);
            if((temp > (int32_t)0x7fffffff) || (temp < (int32_t)0x80000000)) return V8086_DIVISION_OVERFLOW;
            machine->regs.d.edx = dividend % *((int32_t *) source);
            machine->regs.d.eax = temp;
        } else return V8086_BAD_WIDTH;
    } else {
        if (width == 8) {
            if(*((uint8_t *) source) == 0)
                return V8086_DIVISION_BY_ZERO;
            uint16_t temp = machine->regs.w.ax / *((uint8_t *) source);
            if(temp > 0xff) return V8086_DIVISION_OVERFLOW;
            machine->regs.h.ah = ((uint16_t)machine->regs.w.ax % (*((uint8_t *) source)));
            machine->regs.h.al = temp;
        } else if (width == 16) {
            if(*((uint16_t *) source) == 0)
                return V8086_DIVISION_BY_ZERO;
            uint32_t dividend = ((uint32_t)(machine->regs.x.dx) << 16u) | machine->regs.x.ax;
            uint32_t temp = dividend / *((uint16_t *) source);
            if(temp > 0xffff) return V8086_DIVISION_OVERFLOW;
            machine->regs.x.dx = dividend % *((uint16_t *) source);
            machine->regs.x.ax = temp;
        } else if (width == 32) {
            if(*((uint32_t *) source) == 0) {

                return V8086_DIVISION_BY_ZERO;
            }
            uint64_t dividend = ((uint64_t)(machine->regs.d.edx) << 32u) | machine->regs.d.eax;
            uint64_t temp = dividend / *((uint32_t *) source);
            if(temp > 0xffffffff) return V8086_DIVISION_OVERFLOW;
            machine->regs.d.edx = dividend % *((uint32_t *) source);
            machine->regs.d.eax = temp;
        } else return V8086_BAD_WIDTH;
    }
    return V8086_OK;
}

int16_t perform_test(v8086 *machine, void *source, void *dest, uint8_t width) {
    uint16_t temp_flags = 0;
    if (width == 8)
        __asm__ __volatile__("testb %%cl, %%al; pushfw; pop %%bx;" : "=b" (temp_flags) : "a" (*((uint8_t *) dest)), "c" (*((uint8_t *) source)));
    else if (width == 16)
        __asm__ __volatile__("testw %%cx, %%ax; pushfw; pop %%bx;" : "=b" (temp_flags) : "a" (*((uint16_t *) dest)), "c" (*((uint16_t *) source)));
    else if (width == 32)
        __asm__ __volatile__("testl %%ecx, %%eax; pushfw; pop %%bx;" : "=b" (temp_flags) : "a" (*((uint32_t *) dest)), "c" (*((uint32_t *) source)));
    else return V8086_BAD_WIDTH;
    bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << AUX_CARRY_FLAG_BIT, bit_get(temp_flags, 1u << AUX_CARRY_FLAG_BIT) != 0);
    return V8086_OK;
}

int16_t perform_inc(v8086 *machine, void *dest, uint8_t width) {
    uint16_t temp_flags = 0;
    if (width == 8)
        __asm__ __volatile__("incb %%al; pushfw; pop %%bx;" : "=a" (*((uint8_t *) dest)), "=b" (temp_flags) : "a" (*((uint8_t *) dest)));
    else if (width == 16)
        __asm__ __volatile__("incw %%ax; pushfw; pop %%bx;" : "=a" (*((uint16_t *) dest)), "=b" (temp_flags) : "a" (*((uint16_t *) dest)));
    else if (width == 32)
        __asm__ __volatile__("incl %%eax; pushfw; pop %%bx;" : "=a" (*((uint32_t *) dest)), "=b" (temp_flags) : "a" (*((uint32_t *) dest)));
    else return V8086_BAD_WIDTH;
    bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    //bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << AUX_CARRY_FLAG_BIT, bit_get(temp_flags, 1u << AUX_CARRY_FLAG_BIT) != 0);
    return V8086_OK;
    /*uint64_t result = 0;
    uint32_t dest_before;

    if (width == 8) dest_before = *((uint8_t *) dest);
    else if (width == 16) dest_before = *((uint16_t *) dest);
    else if (width == 32) dest_before = *((uint32_t *) dest);
    else return V8086_BAD_WIDTH;

    result = dest_before + 1;

    uint8_t parrity = result & 1u;
    for (uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u << PARITY_FLAG_BIT, (parrity) ? 1 : 0); //PARRITY FLAG
    bit_write(machine->regs.d.eflags, 1u << AUX_CARRY_FLAG_BIT,
              (((dest_before & 0xfu) + 1u) >> 4u) ? 1 : 0); //AUX CARRY FLAG
    bit_write(machine->regs.d.eflags, 1u << ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1u << SIGN_FLAG_BIT, (result >> (width - 1u)) & 1u); //SIGN FLAG
    bit_write(machine->regs.d.eflags, 1u << OVERFLOW_FLAG_BIT,
              ((result >> (width - 1u)) != (dest_before >> (width - 1u)))); //OVERFLOW FLAG

    if (width == 8) *((uint8_t *) dest) = result & 0xFFu;
    else if (width == 16) *((uint16_t *) dest) = result & 0xFFFFu;
    else if (width == 32) *((uint32_t *) dest) = result & 0xFFFFFFFF;
    return V8086_OK;*/
}

int16_t perform_dec(v8086 *machine, void *dest, uint8_t width) {
    uint16_t temp_flags = 0;
    if (width == 8)
        __asm__ __volatile__("decb %%al; pushfw; pop %%bx;" : "=a" (*((uint8_t *) dest)), "=b" (temp_flags) : "a" (*((uint8_t *) dest)));
    else if (width == 16)
        __asm__ __volatile__("decw %%ax; pushfw; pop %%bx;" : "=a" (*((uint16_t *) dest)), "=b" (temp_flags) : "a" (*((uint16_t *) dest)));
    else if (width == 32)
        __asm__ __volatile__("decl %%eax; pushfw; pop %%bx;" : "=a" (*((uint32_t *) dest)), "=b" (temp_flags) : "a" (*((uint32_t *) dest)));
    else return V8086_BAD_WIDTH;
    bit_write(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT, bit_get(temp_flags, 1u << OVERFLOW_FLAG_BIT) != 0);
    //bit_write(machine->regs.w.flags, 1u << CARRY_FLAG_BIT, bit_get(temp_flags, 1u << CARRY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << SIGN_FLAG_BIT, bit_get(temp_flags, 1u << SIGN_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << ZERO_FLAG_BIT, bit_get(temp_flags, 1u << ZERO_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << PARITY_FLAG_BIT, bit_get(temp_flags, 1u << PARITY_FLAG_BIT) != 0);
    bit_write(machine->regs.w.flags, 1u << AUX_CARRY_FLAG_BIT, bit_get(temp_flags, 1u << AUX_CARRY_FLAG_BIT) != 0);
    return V8086_OK;
    /*uint64_t result = 0;
    uint32_t dest_before;

    if (width == 8) dest_before = *((uint8_t *) dest);
    else if (width == 16) dest_before = *((uint16_t *) dest);
    else if (width == 32) dest_before = *((uint32_t *) dest);
    else return V8086_BAD_WIDTH;

    result = dest_before - 1;

    uint8_t parrity = result & 1u;
    for (uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
    bit_write(machine->regs.d.eflags, 1u << PARITY_FLAG_BIT, (parrity) ? 1 : 0); //PARRITY FLAG
    bit_write(machine->regs.d.eflags, 1u << AUX_CARRY_FLAG_BIT,
              (((dest_before & 0xfu) - 1u) >> 4u) ? 1 : 0); //AUX CARRY FLAG
    bit_write(machine->regs.d.eflags, 1u << ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1u << SIGN_FLAG_BIT, (result >> (width - 1u)) & 1u); //SIGN FLAG
    bit_write(machine->regs.d.eflags, 1u << OVERFLOW_FLAG_BIT,
              ((result >> (width - 1u)) != (dest_before >> (width - 1u)))); //OVERFLOW FLAG

    if (width == 8) *((uint8_t *) dest) = result & 0xFFu;
    else if (width == 16) *((uint16_t *) dest) = result & 0xFFFFu;
    else if (width == 32) *((uint32_t *) dest) = result & 0xFFFFFFFF;
    return V8086_OK;*/
}

int16_t perform_artihmetic_or_logical_instruction(v8086 *machine, uint8_t recalculated_opcode, uint32_t carry,
                                                  int16_t (*operation)(v8086 *, void *, void *, uint8_t, uint32_t)) {
    //Maybe Mod/RM, Can be Immediate
    uint8_t mod_rm_or_immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                                               machine->IP.w.ip +
                                                                                               machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t reg = get_reg(mod_rm_or_immediate);
    uint8_t width = 16;
    void *source = NULL;
    void *dest = NULL;
    if (!(recalculated_opcode % 2)) width = 8; //Odd Opcode means 16 or 32 bit operands
    else if (machine->internal_state.operand_32_bit) width = 32;
    switch (recalculated_opcode) {
        case 0: //OPERATION r/m8, r8
            source = get_byte_register(machine, reg);
            dest = get_memory_from_mode(machine, mod_rm_or_immediate, 8);
            if (source == NULL) return V8086_UNDEFINED_REGISTER;
            if (dest == NULL) return V8086_UNABLE_GET_MEMORY;
            break;
        case 1: //OPERATION r/m32, r32 or OPERATION r/m16, r16
            source = get_variable_length_register(machine, reg, width);
            dest = get_memory_from_mode(machine, mod_rm_or_immediate, width);
            if (source == NULL) return V8086_UNDEFINED_REGISTER;
            if (dest == NULL) return V8086_UNABLE_GET_MEMORY;
            break;
        case 2: //OPERATION r8, r/m8
            dest = get_byte_register(machine, reg);
            source = get_memory_from_mode(machine, mod_rm_or_immediate, 8);
            if (dest == NULL) return V8086_UNDEFINED_REGISTER;
            if (source == NULL) return V8086_UNABLE_GET_MEMORY;
            break;
        case 3: //OPERATION r32, r/m32 or OPERATION r16, r/m16
            dest = get_variable_length_register(machine, reg, width);
            source = get_memory_from_mode(machine, mod_rm_or_immediate, width);
            if (dest == NULL) return V8086_UNDEFINED_REGISTER;
            if (source == NULL) return V8086_UNABLE_GET_MEMORY;
            break;
        case 4: //OPERATION AL, imm8
            dest = get_byte_register(machine, V8086_AL);
            source = &(mod_rm_or_immediate);
            if (dest == NULL) return V8086_UNDEFINED_REGISTER;
            break;
        case 5: //OPERATION EAX, imm32 or OPERATION AX, imm16
            dest = get_variable_length_register(machine, V8086_AX, width);
            if (dest == NULL) return V8086_UNDEFINED_REGISTER;
            if (width == 32)
                source = get_dword_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                                 machine->IP.w.ip - 1 +
                                                                                 machine->internal_state.IPOffset));
            else
                source = get_word_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                                machine->IP.w.ip - 1 +
                                                                                machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += ((width / 8) - 1);
            break;
        default:
            return V8086_UNDEFINED_RECALCULATED_OPCODE;
    }
    int16_t status = operation(machine, dest, source, width, carry);
    if (status) return status;
    return V8086_OK;
}

int16_t perform_arithmetic_or_logical_instruction_group(v8086 *machine, uint8_t recalculated_opcode, uint8_t mod_rm,
                                                        uint32_t carry,
                                                        int16_t (*operation)(v8086 *, void *, void *, uint8_t,
                                                                             uint32_t)) {
    void *dest = NULL;
    uint8_t width;
    uint32_t immediate;
    int32_t signed_immediate;
    switch (recalculated_opcode) {
        case 0: //OPERATION rm8, imm8
        case 2: //OPERATION rm8, imm8
            width = 8;
            dest = get_memory_from_mode(machine, mod_rm, width);
            immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                                     machine->IP.w.ip +
                                                                                     machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 1;
            break;
        case 1: //OPERATION rm16, imm16 or rm32, imm32
            width = machine->internal_state.operand_32_bit ? 32 : 16;
            dest = get_memory_from_mode(machine, mod_rm, width);
            if (machine->internal_state.operand_32_bit) {
                immediate = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                                          machine->IP.w.ip +
                                                                                          machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 4;
            } else {
                immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                                         machine->IP.w.ip +
                                                                                         machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 2;
            }
            
            break;
        case 3: //OPERATION rm16, imm8, or rm32, imm8
            if (machine->internal_state.operand_32_bit) width = 32;
            else width = 16;
            dest = get_memory_from_mode(machine, mod_rm, width);
            signed_immediate = (int8_t)read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                                            machine->IP.w.ip +
                                                                                            machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 1;
            break;
        default:
            return V8086_UNDEFINED_RECALCULATED_OPCODE;
    }
    if (dest == NULL) return V8086_UNABLE_GET_MEMORY;
    int16_t status;
    if (recalculated_opcode == 3) status = operation(machine, dest, &signed_immediate, width, carry);
    else status = operation(machine, dest, &immediate, width, carry);
    if (status) return status;
    return V8086_OK;
}

int16_t perform_inc_dec(v8086 *machine, uint8_t opcode, bool dec) {
    uint8_t width = 16;
    void *dest = NULL;
    if (machine->internal_state.operand_32_bit) width = 32;
    dest = get_variable_length_register(machine, opcode & 7u, width);
    if (dest == NULL) return V8086_UNDEFINED_REGISTER;

    if (!dec)
        return perform_inc(machine, dest, width);
    return perform_dec(machine, dest, width);
}

int16_t execute_test(v8086 *machine, uint8_t opcode) {
    //Mod/RM
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip +
                                                                                                     machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = 8;
    if (opcode % 2)
        width = machine->internal_state.operand_32_bit ? 32 : 16;
    void *source = get_variable_length_register(machine, get_reg(mod_rm), width);
    void *dest = get_memory_from_mode(machine, mod_rm, width);

    if (source == NULL) return V8086_UNDEFINED_REGISTER;
    if (dest == NULL) return V8086_UNABLE_GET_MEMORY;
    return perform_test(machine, source, dest, width);
}

int16_t execute_test_immediate(v8086 *machine, uint8_t opcode) {
    uint8_t width = 8;
    if (opcode == 0xa9u) width = machine->internal_state.operand_32_bit ? 32 : 16;
    void *immediate = get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                                        machine->IP.w.ip +
                                                                                        machine->internal_state.IPOffset),
                                                  width);
    machine->internal_state.IPOffset += width / 8;
    void *reg = get_variable_length_register(machine, V8086_AX, width);
    if (immediate == NULL) return V8086_UNABLE_GET_MEMORY;
    if (reg == NULL) return V8086_UNDEFINED_REGISTER;
    return perform_test(machine, immediate, reg, width);
}

int16_t execute_group_2(v8086 *machine, uint8_t opcode) {
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip +
                                                                                                     machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t arg = opcode <= 0xd1 ? 1 : machine->regs.h.cl;
    uint8_t width = 8;
    if (opcode % 2) {
        if (machine->internal_state.operand_32_bit) width = 32;
        else width = 16;
    }
    void *dest = get_memory_from_mode(machine, mod_rm, width);
    if((opcode & 0xf0u) == 0xc0u)
    {
        arg = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip +
                                                                                              machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
    }
    if (dest == NULL) return V8086_UNABLE_GET_MEMORY;
    switch (get_reg(mod_rm)) {
        case 0:
            return perform_rol(machine, dest, arg, width);
        case 1:
            return perform_ror(machine, dest, arg, width);
        case 2:
            return perform_rcl(machine, dest, arg, width);
        case 3:
            return perform_rcr(machine, dest, arg, width);
        case 4:
            return perform_shl(machine, dest, arg, width);
        case 5:
            return perform_shr(machine, dest, arg, width);
        case 6:
            return V8086_UNDEFINED_OPCODE;
        case 7:
            return perform_sar(machine, dest, arg, width);
        default:
            return V8086_BAD_REG;
    }
}

int16_t execute_group_3(v8086 *machine, uint8_t opcode) {
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip +
                                                                                                     machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = 8;
    if (opcode == 0xf7) {
        if (machine->internal_state.operand_32_bit) width = 32;
        else width = 16;
    }

    void *dest = get_memory_from_mode(machine, mod_rm, width);

    if (dest == NULL) return V8086_UNABLE_GET_MEMORY;

    switch (get_reg(mod_rm)) {
        case 0: //TEST
        {
            uint32_t immediate;
            if (width == 8) {
                immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                                         machine->IP.w.ip +
                                                                                         machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 1;
            } else if (width == 16) {
                immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                                         machine->IP.w.ip +
                                                                                         machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 2;
            } else if (width == 32) {
                immediate = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                                          machine->IP.w.ip +
                                                                                          machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 4;
            }
            return perform_test(machine, &immediate, dest, width);
        }
        case 2: //NOT
            if (width == 8)
                *((uint8_t *) dest) = ~(*((uint8_t *) dest));
            else if (width == 16)
                *((uint16_t *) dest) = ~(*((uint16_t *) dest));
            else if (width == 32)
                *((uint32_t *) dest) = ~(*((uint32_t *) dest));
            else return V8086_BAD_WIDTH;
            return V8086_OK;
        case 3: //NEG
            return perform_neg(machine, dest, width);
        case 4: //MUL
            return perform_multiplication(machine, dest, 0, width);
        case 5: //IMUL
            return perform_multiplication(machine, dest, 1, width);
        case 6: //DIV
            return perform_division(machine, dest, 0, width);
        case 7: //IDIV
            return perform_division(machine, dest, 1, width);
        default:
            return V8086_BAD_REG;
    }
}

int16_t execute_double_shift(v8086* machine, uint8_t width, uint8_t right_shift, uint8_t use_cl)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip +
                                                                                                     machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    void* reg = get_variable_length_register(machine, get_reg(mod_rm), width);
    void* rm = get_memory_from_mode(machine, mod_rm, width);
    uint8_t imm;
    if(use_cl)
        imm = machine->regs.h.cl;
    else{
        imm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip +
                                                                                                 machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
    }

    if(right_shift)
        return perform_shrd(machine, rm, reg, imm, width);
    else
        return perform_shld(machine, rm, reg, imm, width);
}
