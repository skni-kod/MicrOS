#include <stdint.h>
#include <v8086/v8086.h>
#include <v8086/memory_operations.h>
#include <v8086/mod_rm_parsing.h>
#include "exchange_operations.h"
#include "internal_funcs.h"

int16_t perform_exchange(void* source, void* dest, uint8_t width)
{
    uint32_t temp;
    switch(width)
    {
        case 8:
            temp = *((uint8_t*) source);
            *((uint8_t*) source) = *((uint8_t*) dest);
            *((uint8_t*) dest) = temp;
            break;
        case 16:
            temp = *((uint16_t*) source);
            *((uint16_t*) source) = *((uint16_t*) dest);
            *((uint16_t*) dest) = temp;
            break;
        case 32:
            temp = *((uint32_t*) source);
            *((uint32_t*) source) = *((uint32_t*) dest);
            *((uint32_t*) dest) = temp;
            break;
        default:
            return V8086_BAD_WIDTH;
    }
    return V8086_OK;
}

int16_t perform_exchange_rm(v8086* machine, uint8_t opcode)
{
    int8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = 8;
    if(opcode % 2)
        width = machine->internal_state.operand_32_bit ? 32 : 16;

    void* source = get_variable_length_register(machine, get_reg(mod_rm), width);
    void* dest = get_memory_from_mode(machine, mod_rm, width);
    if(source == NULL) return V8086_UNDEFINED_REGISTER;
    if(dest == NULL) return V8086_UNABLE_GET_MEMORY;
    return perform_exchange(source, dest, width);
}

int16_t perform_exchange_ax_register(v8086* machine, uint8_t opcode)
{
    uint8_t width = 16;
    if(machine->internal_state.operand_32_bit) width = 32;
    void* regA = get_variable_length_register(machine, V8086_EAX, width);
    void* regB = get_word_register(machine, opcode & 7u);
    if((regA == NULL) || (regB == NULL)) return V8086_UNDEFINED_REGISTER;
    return perform_exchange(regA, regB, width);
}