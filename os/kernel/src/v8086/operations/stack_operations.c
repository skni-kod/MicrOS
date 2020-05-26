#include <stdint.h>
#include <v8086/v8086.h>
#include <v8086/mod_rm_parsing.h>
#include <stddef.h>
#include <v8086/stack.h>
#include "stack_operations.h"

uint16_t push_gpr(v8086* machine, uint8_t opcode)
{
    uint8_t width = 16;
    void* reg = NULL;
    if(machine->internal_state.operand_32_bit) width = 32;
    reg = get_variable_length_register(machine, opcode & 7u, width);
    if(reg == NULL) return UNDEFINED_REGISTER;
    if(width==16) push_word(machine, *((uint16_t*)reg));
    else push_dword(machine, *((uint32_t*)reg));
    return OK;
}

uint16_t pop_gpr(v8086* machine, uint8_t opcode)
{
    uint8_t width = 16;
    void* reg = NULL;
    if(machine->internal_state.operand_32_bit) width = 32;
    reg = get_variable_length_register(machine, opcode & 7u, width);
    if(reg == NULL) return UNDEFINED_REGISTER;
    if(width==16) *((uint16_t*)reg) = pop_word(machine);
    else *((uint32_t*)reg) = pop_dword(machine);
    return OK;
}

uint16_t pop_rm(v8086* machine)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = 16;
    if(machine->internal_state.operand_32_bit) width = 32;

    void* dest = get_memory_from_mode(machine, mod_rm, width);
    if(width == 16)
        *((uint16_t*)dest) = pop_word(machine);
    else
        *((uint32_t*)dest) = pop_dword(machine);
    return OK;
}