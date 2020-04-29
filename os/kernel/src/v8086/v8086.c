#include "v8086.h"
#include "../memory/heap/heap.h"
#include <string.h>

enum BYTE_REGISTERS {AL=0, CL, DL, BL, AH, CH, DH, BH};
enum WORD_REGISTERS {AX=0, CX, DX, BX, SP, BP, SI, DI};
enum DWORD_REGISTERS {EAX=0, ECX, EDX, EBX, ESP, EBP, ESI, EDI};

int16_t parse_instruction(v8086* machine);

static inline uint16_t get_absolute_address(uint16_t segment, uint16_t offset)
{
    return segment * 16 + offset;
}

static inline uint8_t* get_byte_pointer(uint8_t* memory, uint16_t offset)
{
    return memory + offset;
}

static inline uint16_t* get_word_pointer(uint8_t* memory, uint16_t offset)
{
    return (uint16_t*)(memory + offset);
}

static inline uint32_t* get_dword_pointer(uint8_t* memory, uint16_t offset)
{
    return (uint32_t*)(memory + offset);
}

static inline void* get_variable_length_pointer(uint8_t* memory, uint16_t offset, uint8_t width)
{
    switch(width)
    {
        case 8:
            return get_byte_pointer(memory, offset);
        case 16:
            return get_word_pointer(memory, offset);
        case 32:
            return get_dword_pointer(memory, offset);
        default:
            return NULL;
    }
}

static inline uint8_t read_byte_from_pointer(uint8_t* memory, uint16_t offset)
{
    return *(memory + offset);
}

static inline uint16_t read_word_from_pointer(uint8_t* memory, uint16_t offset)
{
    return *get_word_pointer(memory, offset);
}

static inline uint32_t read_dword_from_pointer(uint8_t* memory, uint16_t offset)
{
    return *get_dword_pointer(memory, offset);
}

static inline void write_word_to_pointer(uint8_t* memory, uint16_t offset, uint16_t value)
{
    *(get_word_pointer(memory, offset)) = value;
}

static inline uint16_t read_word_from_double_pointer(uint8_t* memory, uint16_t offset)
{
    return *(get_word_pointer(memory, read_word_from_pointer(memory, offset)));
}

static inline void write_word_from_double_pointer(uint8_t* memory, uint16_t offset, uint16_t value)
{
    *(get_word_pointer(memory, read_word_from_pointer(memory, offset))) = value;
}

static inline void push_word(v8086* machine, uint16_t value)
{
    write_word_to_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp -= 2), value);
}

static inline uint16_t pop_word(v8086* machine, uint16_t value)
{
    return read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp += 2));
}

v8086* v8086_create_machine()
{
    v8086* machine = (v8086*) heap_kernel_alloc(sizeof(v8086), 0);
    if(machine == NULL) return NULL;
    memset(machine, 0, sizeof(v8086));
    machine->regs.x.flags = 0x2;
    machine->sregs.cs = 0xf000;
    machine->IP = 0xfff0;
	memcpy(machine->Memory, (void*)0xc0000000, 0x100000);

    return machine;
}

int16_t v8086_call_function(v8086* machine)
{
    while(!(machine->IP == 0xFFFF && machine->sregs.cs == 0xFFFF))
    {
        int x = -1;
        if(x) return x;
    }
    return 0;
}

int16_t v8086_call_int(v8086* machine, int16_t num)
{
    if ((num < 0) || (num > 0xFF)) return -1;
    machine -> IP = read_word_from_double_pointer(machine->Memory, get_absolute_address(0, num * 4));
    machine -> sregs.cs = read_word_from_double_pointer(machine->Memory, get_absolute_address(0, num * 4 + 2));
    push_word(machine, machine->regs.w.flags);
    push_word(machine, 0xFFFF);
    push_word(machine, 0xFFFF);
    int x = v8086_call_function(machine);
    if(x) return -2;
    return num;
}

uint8_t* get_byte_register(v8086* machine, uint8_t reg_field)
{
    switch(reg_field)
    {
        case AL:
            return &(machine->regs.h.al);
        case AH:
            return &(machine->regs.h.ah);
        case BL:
            return &(machine->regs.h.bl);
        case BH:
            return &(machine->regs.h.bh);
        case CL:
            return &(machine->regs.h.cl);
        case CH:
            return &(machine->regs.h.ch);
        case DL:
            return &(machine->regs.h.dl);
        case DH:
            return &(machine->regs.h.dh);
    }
    return NULL;
}

uint16_t* get_word_register(v8086* machine, uint8_t reg_field)
{
    switch(reg_field)
    {
        case AX:
            return &(machine->regs.x.ax);
        case CX:
            return &(machine->regs.x.cx);
        case DX:
            return &(machine->regs.x.dx);  
        case BX:
            return &(machine->regs.x.bx);
        case SP:
            return &(machine->regs.x.sp);
        case BP:
            return &(machine->regs.x.bp);
        case SI:
            return &(machine->regs.x.si);
        case DI:
            return &(machine->regs.x.di);
    }
    return NULL;
}

uint32_t* get_dword_register(v8086* machine, uint8_t reg_field)
{
    switch(reg_field)
    {
        case EAX:
            return &(machine->regs.d.eax);
        case ECX:
            return &(machine->regs.d.ecx);
        case EDX:
            return &(machine->regs.d.edx);  
        case EBX:
            return &(machine->regs.d.ebx);
        case ESP:
            return &(machine->regs.d.esp);
        case EBP:
            return &(machine->regs.d.ebp);
        case ESI:
            return &(machine->regs.d.esi);
        case EDI:
            return &(machine->regs.d.edi);
    }
    return NULL;
}

void* get_memory_from_mode(v8086* machine, uint8_t mod_rm, uint8_t width)
{
    switch(mod_rm >> 6) //Parsing mod than parsing rm
    {
        case 0:
            switch(mod_rm & 7){
                case 0:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.bx + machine->regs.x.si), width);  
                case 1:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.bx + machine->regs.x.di), width);
                case 2:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.x.bp + machine->regs.x.si), width);
                case 3:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.x.bp + machine->regs.x.di), width);
                case 4:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.si), width);
                case 5:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.di), width);
                case 6:{
                    void* ptr = get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + (machine->internal_state.IPOffset)))), width);
                    machine->internal_state.IPOffset += 1;
                    return ptr;
                }
                case 7:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.bx), width);
                default:
                    return NULL;
            }
        case 1:{
            int8_t disp = (int8_t) read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 1;
            switch(mod_rm & 7){
                case 0:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.bx + machine->regs.x.si + disp), width);
                case 1:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.bx + machine->regs.x.di + disp), width);
                case 2:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.x.bp + machine->regs.x.si + disp), width);
                case 3:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.x.bp + machine->regs.x.di + disp), width);
                case 4:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.si + disp), width);
                case 5:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.di + disp), width);
                case 6:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.x.bp + disp), width);
                case 7:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.bx + disp), width);
                default:
                    return NULL;
            }
        }
        case 2:{
            uint16_t disp = (uint16_t) read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 2;
            switch(mod_rm & 7){
                case 0:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.bx + machine->regs.x.si + disp), width);
                case 1:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.bx + machine->regs.x.di + disp), width);
                case 2:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.x.bp + machine->regs.x.si + disp), width);
                case 3:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.x.bp + machine->regs.x.di + disp), width);
                case 4:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.si + disp), width);
                case 5:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.di + disp), width);
                case 6:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.x.bp + disp), width);
                case 7:
                    return get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.ds, machine->regs.x.bx + disp), width);
                default:
                    return NULL;
            }
        }
        case 3:
            switch(width){
                case 8:
                    return get_byte_register(machine, mod_rm & 7);
                case 16:
                    return get_word_register(machine, mod_rm & 7);
                case 32:
                    return get_dword_register(machine, mod_rm & 7);
                default:
                    return NULL;
            }
    }
    return NULL;
}

int16_t parse_and_execute_instruction(v8086* machine)
{
    machine->internal_state.IPOffset = 0;
    machine->internal_state.operand_32_bit = 0;

    //Maybe opcode, an be also prefix
    uint8_t opcode = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    
    //Aritmetic operations
    //ADD
    if(opcode > 0 && opcode <= 5)
    {
        //Maybe Mod/RM, Can be Immediate
        uint8_t mod_rm_or_immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        
        if(opcode % 2) //Odd Opcode means 16 or 32 bit operands
        {
            if(machine->internal_state.operand_32_bit)
            {
                uint32_t* reg = NULL;
                uint32_t* mem = NULL;
                uint64_t result = 0;
                switch(opcode)
                {
                case 1: //ADD r/m8, r8
                    reg = get_dword_register(machine, (mod_rm_or_immediate >> 3) & 7);
                    mem = get_memory_from_mode(machine, mod_rm_or_immediate, 32);
                    result = *reg + *mem;
                    *mem = result & 0xFFFFFFFF;
                    break;
                case 3: //ADD r32, r/m32
                    reg = get_dword_register(machine, (mod_rm_or_immediate >> 3) & 7);
                    mem = get_memory_from_mode(machine, mod_rm_or_immediate, 32);
                    result = *reg + *mem;
                    *reg = result & 0xFFFFFFFF;
                    break;
                case 5: //ADD EAX, imm32
                    result = machine->regs.d.eax + read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP - 1 + machine->internal_state.IPOffset));
                    machine->internal_state.IPOffset += 3;
                    machine->regs.d.eax = result & 0xFFFFFFFF;
                    break;
                }
            }
            else
            {
                uint16_t* reg = NULL;
                uint16_t* mem = NULL;
                uint32_t result = 0;
                switch(opcode)
                {
                case 1: //ADD r/m16, r16
                    reg = get_word_register(machine, (mod_rm_or_immediate >> 3) & 7);
                    mem = get_memory_from_mode(machine, mod_rm_or_immediate, 16);
                    result = *reg + *mem;
                    *mem = result & 0xFFFF;
                    break;
                case 3: //ADD r16, r/m16
                    reg = get_word_register(machine, (mod_rm_or_immediate >> 3) & 7);
                    mem = get_memory_from_mode(machine, mod_rm_or_immediate, 16);
                    result = *reg + *mem;
                    *reg = result & 0xFFFF;
                    break;
                case 5: //ADD AX, imm16
                    result = machine->regs.x.ax + read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP - 1 + machine->internal_state.IPOffset));
                    machine->internal_state.IPOffset += 1;
                    machine->regs.x.ax = result & 0xFFFF;
                    break;
                }
            }
        }
        else
        {
            uint8_t* reg = NULL;
            uint8_t* mem = NULL;
            uint16_t result = 0;
            switch(opcode)
            {
            case 0: //ADD r/m8, r8
                reg = get_byte_register(machine, (mod_rm_or_immediate >> 3) & 7);
                mem = get_memory_from_mode(machine, mod_rm_or_immediate, 8);
                result = *reg + *mem;
                *mem = result & 0xFF;
                break;
            case 2: //ADD r8, r/m8
                reg = get_byte_register(machine, (mod_rm_or_immediate >> 3) & 7);
                mem = get_memory_from_mode(machine, mod_rm_or_immediate, 8);
                result = *reg + *mem;
                *reg = result & 0xFF;
                break;
            case 4: //ADD AL, imm8
                result = machine->regs.h.al + mod_rm_or_immediate;
                machine->regs.h.al = result & 0xFF;
                break;
            }
        }
    }
    return 0;
}