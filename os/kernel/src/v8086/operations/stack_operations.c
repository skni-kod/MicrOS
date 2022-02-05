#include <stdint.h>
#include <v8086/v8086.h>
#include <v8086/mod_rm_parsing.h>
#include <stddef.h>
#include <v8086/stack.h>
#include "stack_operations.h"

int16_t push_gpr(v8086* machine, uint8_t opcode)
{
    uint8_t width = 16;
    void* reg = NULL;
    if(machine->internal_state.operand_32_bit) width = 32;
    reg = get_variable_length_register(machine, opcode & 7u, width);
    if(reg == NULL) return V8086_UNDEFINED_REGISTER;
    if(width==16) push_word(machine, *((uint16_t*)reg));
    else push_dword(machine, *((uint32_t*)reg));
    return V8086_OK;
}

int16_t pop_gpr(v8086* machine, uint8_t opcode)
{
    uint8_t width = 16;
    void* reg = NULL;
    if(machine->internal_state.operand_32_bit) width = 32;
    reg = get_variable_length_register(machine, opcode & 7u, width);
    if(reg == NULL) return V8086_UNDEFINED_REGISTER;
    if(width==16) *((uint16_t*)reg) = pop_word(machine);
    else *((uint32_t*)reg) = pop_dword(machine);
    return V8086_OK;
}

int16_t pop_rm(v8086* machine)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = 16;
    if(machine->internal_state.operand_32_bit) width = 32;
    uint32_t temp = width == 16 ? pop_word(machine) : pop_dword(machine);
    void* dest = get_memory_from_mode(machine, mod_rm, width);
    if(width == 16)
        *((uint16_t*)dest) = (uint16_t)temp;
    else
        *((uint32_t*)dest) = temp;
    return V8086_OK;
}

int16_t push_all(v8086* machine)
{
    uint8_t width = machine->internal_state.operand_32_bit ? 32 : 16;
    uint32_t temp_sp = width == 16 ? machine->regs.w.sp : machine->regs.d.esp;
    if(width == 16)
    {
        push_word(machine, machine->regs.x.ax);
        push_word(machine, machine->regs.x.cx);
        push_word(machine, machine->regs.x.dx);
        push_word(machine, machine->regs.x.bx);
        push_word(machine, temp_sp);
        push_word(machine, machine->regs.x.bp);
        push_word(machine, machine->regs.x.si);
        push_word(machine, machine->regs.x.di);
    } else
    {
        push_dword(machine, machine->regs.d.eax);
        push_dword(machine, machine->regs.d.ecx);
        push_dword(machine, machine->regs.d.edx);
        push_dword(machine, machine->regs.d.ebx);
        push_dword(machine, temp_sp);
        push_dword(machine, machine->regs.d.ebp);
        push_dword(machine, machine->regs.d.esi);
        push_dword(machine, machine->regs.d.edi);
    }
    return V8086_OK;
}

int16_t pop_all(v8086* machine)
{
    uint8_t width = machine->internal_state.operand_32_bit ? 32 : 16;
    if(width == 16)
    {
        machine->regs.x.di = pop_word(machine);
        machine->regs.x.si = pop_word(machine);
        machine->regs.x.bp = pop_word(machine);
        //Throw away sp;
        pop_word(machine);
        machine->regs.x.bx = pop_word(machine);
        machine->regs.x.dx = pop_word(machine);
        machine->regs.x.cx = pop_word(machine);
        machine->regs.x.ax = pop_word(machine);
    } else
    {
        machine->regs.d.edi = pop_dword(machine);
        machine->regs.d.esi = pop_dword(machine);
        machine->regs.d.ebp = pop_dword(machine);
        //Throw away esp;
        pop_dword(machine);
        machine->regs.d.ebx = pop_dword(machine);
        machine->regs.d.edx = pop_dword(machine);
        machine->regs.d.ecx = pop_dword(machine);
        machine->regs.d.eax = pop_dword(machine);
    }
    return V8086_OK;
}

int16_t push_immediate(v8086* machine, uint8_t width)
{
    if(width == 8)
    {
        int8_t imm = (int8_t)read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        if (machine -> internal_state.operand_32_bit )
            push_dword(machine, imm);
        else
            push_word(machine, imm);
        return V8086_OK;
    }
    if(width == 16)
    {
        uint16_t imm = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 2;
        push_word(machine, imm);
        return V8086_OK;
    }
    if(width == 32)
    {
        uint32_t imm = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 4;
        push_dword(machine, imm);
        return V8086_OK;
    }
    return V8086_BAD_WIDTH;
}

int16_t enter(v8086* machine, uint8_t width)
{
    uint16_t alloc_size = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 2;

    uint8_t nesting_level = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    nesting_level = nesting_level & 0x1fu;

    uint32_t temp_ebp;

    if(width == 32)
    {
        push_dword(machine, machine->regs.d.ebp);
        temp_ebp = machine->regs.d.esp;
    }
    else if(width == 16)
    {
        push_word(machine, machine->regs.w.bp);
        temp_ebp = machine->regs.w.sp;
    }
    else return V8086_BAD_WIDTH;

    if(nesting_level > 0)
    {
        for(uint8_t i = 1; i < nesting_level; i++)
        {
            if(width == 32)
            {
                machine->regs.d.ebp -= 4;
                push_dword(machine, read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.d.ebp)));
            }
            else{
                machine->regs.w.bp -= 2;
                push_word(machine, read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.bp)));
            }
        }
        if(width == 32)
            push_dword(machine, temp_ebp);
        else
            push_word(machine, temp_ebp);
    }

    if(width == 32) {
        machine->regs.d.ebp = temp_ebp;
        machine->regs.d.esp -= alloc_size;
    }
    else {
        machine->regs.w.bp = temp_ebp;
        machine->regs.w.sp -= alloc_size;
    }

    return V8086_OK;
}

int16_t leave(v8086* machine, uint8_t width)
{
    if(width == 32) {
        machine->regs.d.esp = machine->regs.d.ebp;
        machine->regs.d.ebp = pop_dword(machine);
        return V8086_OK;
    }
    if(width == 16){
        machine -> regs.w.sp = machine->regs.w.sp;
        machine->regs.w.bp = pop_word(machine);
        return V8086_OK;
    }
    return V8086_BAD_WIDTH;
}