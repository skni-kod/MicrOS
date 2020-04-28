#include "v8086.h"
#include "../memory/heap/heap.h"
#include <string.h>

enum WORD_REGISTERS {AL=0, CL, DL, BL, AH, CH, DH, BH};

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

static inline uint8_t read_byte_from_pointer(uint8_t* memory, uint16_t offset)
{
    return *(memory + offset);
}

static inline uint16_t read_word_from_pointer(uint8_t* memory, uint16_t offset)
{
    return *get_word_pointer(memory, offset);
}

static inline void write_word_to_pointer(uint8_t* memory, uint16_t offset, uint16_t value)
{
    *(get_word_pointer(memory, offset)) = value;
}

static inline uint16_t read_word_from_double_pointer(uint8_t* memory, uint16_t offset)
{
    *(get_word_pointer(memory, read_word_from_pointer(memory, offset)));
}

static inline void write_word_from_double_pointer(uint8_t memory, uint16_t offset, uint16_t value)
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
	memcpy(machine->Memory, 0xc0000000, 0x100000);

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

static inline uint8_t* get_byte_register(v8086* machine, uint8_t reg_field)
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
}

static inline uint8_t* get_byte_memory(v8086* machine, uint8_t mod_rm)
{
    
}

int16_t parse_and_execute_instruction(v8086* machine)
{
    //Maybe opcode, an be also prefix
    uint8_t opcode = read_byte_from_pointer(machine->Memory, machine->IP);
    
    //Aritmetic operations
    //ADD
    if(opcode > 0 && opcode <= 5)
    {
        //Maybe Mod/RM, Can be Immediate
        uint8_t mod_rm_or_immediate = read_byte_from_pointer(machine->Memory, machine->IP + 1);
        
        if(opcode % 2) //Odd Opcode means 16 or 32 bit operands
        {
            
        }
        else
        {
            uint8_t* reg = NULL;
            uint8_t* mem = NULL;
            uint16_t result = 0;
            switch(opcode)
            {
            case 0: //ADD r/m8, r8
                *reg = get_byte_register(machine, (mod_rm_or_immediate >> 3) & 7);
                result = *reg + *mem;
                *mem = result & 0xFF;
            case 2: //ADD r8, r/m8
                *reg = get_byte_register(machine, (mod_rm_or_immediate >> 3) & 7);
                result = *reg + *mem;
                *reg = result & 0xFF;
            case 4: //ADD AL, imm8
                result = machine->regs.h.al + mod_rm_or_immediate;
                machine->regs.h.al = result & 0xFF;
            }
        }
    }
}