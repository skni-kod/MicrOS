#include "v8086.h"
#include "../memory/heap/heap.h"
#include <string.h>

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(p,m,v) (v ? bit_set(p,m) : bit_clear(p,m))

#define CARRY_FLAG_BIT 0
#define PARITY_FLAG_BIT 2
#define AUX_CARRY_FLAG_BIT 4
#define ZERO_FLAG_BIT 6
#define SIGN_FLAG_BIT 7
#define DIRECTION_FLAG_BIT 10
#define OVERFLOW_FLAG_BIT 11

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

static inline void write_byte_to_pointer(uint8_t* memory, uint16_t offset, uint8_t value)
{
    *(get_byte_pointer(memory, offset)) = value;
}

static inline void write_word_to_pointer(uint8_t* memory, uint16_t offset, uint16_t value)
{
    *(get_word_pointer(memory, offset)) = value;
}

static inline void write_dword_to_pointer(uint8_t* memory, uint16_t offset, uint32_t value)
{
    *(get_dword_pointer(memory, offset)) = value;
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

static inline void push_dword(v8086* machine, uint32_t value)
{
    write_dword_to_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp -= 4), value);
}

static inline uint16_t pop_word(v8086* machine)
{
    uint16_t v = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp));
    machine->regs.w.sp += 2;
    return v;
}

static inline uint32_t pop_word(v8086* machine)
{
    uint16_t v = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp));
    machine->regs.w.sp += 4;
    return v;
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

void* get_variable_length_register(v8086* machine, uint8_t reg_field, uint8_t width)
{
    switch (width)
    {
        case 8:
            return get_byte_register(machine, reg_field);
        case 16:
            return get_word_register(machine, reg_field);
        case 32:
            return get_dword_register(machine, reg_field);
        default:
            return NULL;
    }
}

uint16_t* select_segment_register(v8086* machine, segment_register_select select)
{
    switch(select)
    {
        case CS:
            return &(machine->sregs.cs);
        case DS:
            return &(machine->sregs.ds);
        case SS:
            return &(machine->sregs.ss);
        case ES:
            return &(machine->sregs.es);
        case FS:
            return &(machine->sregs.fs);
        case GS:
            return &(machine->sregs.gs);
        default:
            return NULL;
    }
}

int16_t calculate_segment_offset_from_mode(v8086* machine, uint8_t mod_rm, uint16_t* segment, uint16_t* offset)
{
    uint16_t* segment_register = NULL;
    if(machine->internal_state.segment_reg_select != DEFAULT)
        segment_register = select_segment_register(machine, machine->internal_state.segment_reg_select);
    else
    {
        switch (mod_rm & 7)
        {
        case 0:
        case 1:
        case 4:
        case 5:
        case 7:
            segment_register = select_segment_register(machine, DS);
            break;
        case 6:
            if((mod_rm >> 6) == 0) segment_register = select_segment_register(machine, DS);
            else segment_register = select_segment_register(machine, SS);
        default:
            segment_register = select_segment_register(machine, SS);
            break;
        }
    }

    *segment = *segment_register;

    switch(mod_rm >> 6) //Parsing mod than parsing rm
    {
        case 0:
            switch(mod_rm & 7){
                case 0:
                    *offset = machine->regs.x.bx + machine->regs.x.si;
                    return 0;  
                case 1:
                    *offset = machine->regs.x.bx + machine->regs.x.di;
                    return 0;
                case 2:
                    *offset = machine->regs.x.bp + machine->regs.x.si;
                    return 0;
                case 3:
                    *offset = machine->regs.x.bp + machine->regs.x.di;
                    return 0;
                case 4:
                    *offset = machine->regs.x.si;
                    return 0;
                case 5:
                    *offset = machine->regs.x.di;
                    return 0;
                case 6:{
                    *offset = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + (machine->internal_state.IPOffset)));
                    machine->internal_state.IPOffset += 1;
                    return 0;
                }
                case 7:
                    *offset = machine->regs.x.bx;
                default:
                    return -1;
            }
        case 1:{
            int8_t disp = (int8_t) read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 1;
            switch(mod_rm & 7){
                case 0:
                    *offset = machine->regs.x.bx + machine->regs.x.si + disp;
                    return 0;
                case 1:
                    *offset = machine->regs.x.bx + machine->regs.x.di + disp;
                    return 0;
                case 2:
                    *offset = machine->regs.x.bp + machine->regs.x.si + disp;
                    return 0;
                case 3:
                    *offset = machine->regs.x.bp + machine->regs.x.di + disp;
                    return 0;
                case 4:
                    *offset = machine->regs.x.si + disp;
                    return 0;
                case 5:
                    *offset = machine->regs.x.di + disp;
                    return 0;
                case 6:
                    *offset = machine->regs.x.bp + disp;
                    return 0;
                case 7:
                    *offset = machine->regs.x.bx + disp;
                    return 0;
                default:
                    return -1;
            }
        }
        case 2:{
            uint16_t disp = (uint16_t) read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 2;
            switch(mod_rm & 7){
                case 0:
                    *offset = machine->regs.x.bx + machine->regs.x.si + disp;
                    return 0;
                case 1:
                    *offset = machine->regs.x.bx + machine->regs.x.di + disp;
                    return 0;
                case 2:
                    *offset = machine->regs.x.bp + machine->regs.x.si + disp;
                    return 0;
                case 3:
                    *offset = machine->regs.x.bp + machine->regs.x.di + disp;
                    return 0;
                case 4:
                    *offset = machine->regs.x.si + disp;
                    return 0;
                case 5:
                    *offset = machine->regs.x.di + disp;
                    return 0;
                case 6:
                    *offset = machine->regs.x.bp + disp;
                    return 0;
                case 7:
                    *offset = machine->regs.x.bx + disp;
                    return 0;
                default:
                    return -1;
            }
        }
        return -1;
    }

}

void* get_memory_from_mode(v8086* machine, uint8_t mod_rm, uint8_t width)
{
    uint16_t segment;
    uint16_t offset;

    switch(mod_rm >> 6) //Parsing mod than parsing rm
    {
        case 0:
        case 1:
        case 2:
            calculate_segment_offset_from_mode(machine, mod_rm, &segment, &offset);
            return get_variable_length_pointer(machine->Memory, get_absolute_address(segment, offset), width);
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
    } else return -1;
    result = dest_before + source_before + carry;
    bit_write(machine->regs.d.eflags, 1<<CARRY_FLAG_BIT, (result >> width) ? 1 : 0); // CARRY FLAG
    uint8_t parrity = result & 1;
    for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
    bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine->regs.d.eflags, 1<<AUX_CARRY_FLAG_BIT, (((dest_before & 0xf) + (source_before & 0xf)) >> 4) ? 1: 0); //AUX CARRY FLAG
    bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> (width - 1)); //SIGN FLAG
    bit_write(machine->regs.d.eflags, 1<<OVERFLOW_FLAG_BIT, ((result >> (width - 1)) != (dest_before >> (width - 1)))); //OVERFLOW FLAG
    if(width == 8) *((uint8_t*)dest) = result & 0xFF;
    else if(width == 16) *((uint16_t*)dest) = result & 0xFFFF;
    else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
    else return -1;
    return 0;
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
    } else return -1;
    result = dest_before - (source_before + carry);
    bit_write(machine->regs.d.eflags, 1<<CARRY_FLAG_BIT, (result >> width) ? 1 : 0); // CARRY FLAG
    bit_write(machine->regs.d.eflags, 1<<AUX_CARRY_FLAG_BIT, (((dest_before & 0xf) - (source_before & 0xf)) >> 4) ? 1: 0); //AUX CARRY FLAG
    uint8_t parrity = result & 1;
    for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
    bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> (width - 1)); //SIGN FLAG
    bit_write(machine->regs.d.eflags, 1<<OVERFLOW_FLAG_BIT, ((result >> (width - 1)) != (dest_before >> (width - 1)))); //OVERFLOW FLAG
    if(width == 8) *((uint8_t*)dest) = result & 0xFF;
    else if(width == 16) *((uint16_t*)dest) = result & 0xFFFF;
    else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
    else return -1;
    return 0;
}

int16_t perform_or(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry)
{
    uint32_t result = 0;
    if(width == 8)
    {
        result = *((uint8_t*)dest) | *((uint8_t*)source);
    } else if(width == 16)
    {
        result = *((uint16_t*)dest) | *((uint16_t*)source);
    } else if(width == 32)
    {
        result = *((uint32_t*)dest) | *((uint32_t*)source);
    }
    else return -1;
    bit_write(machine->regs.d.eflags, 1<<CARRY_FLAG_BIT, 0); // CARRY FLAG
    bit_write(machine->regs.d.eflags, 1<<OVERFLOW_FLAG_BIT, 0); // OVERFLOW FLAG
    uint8_t parrity = result & 1;
    for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
    bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> (width - 1)); //SIGN FLAG
    //AUX MARKED AS UNDEFINED IN INTEL DOCUMENTATION
    if(width == 8) *((uint8_t*)dest) = result & 0xFF;
    else if(width == 16) *((uint16_t*)dest) = result & 0xFFFF;
    else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
}

int16_t perform_and(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry)
{
    uint32_t result = 0;
    if(width == 8)
    {
        result = *((uint8_t*)dest) & *((uint8_t*)source);
    } else if(width == 16)
    {
        result = *((uint16_t*)dest) & *((uint16_t*)source);
    } else if(width == 32)
    {
        result = *((uint32_t*)dest) & *((uint32_t*)source);
    }
    else return -1;
    bit_write(machine->regs.d.eflags, 1<<CARRY_FLAG_BIT, 0); // CARRY FLAG
    bit_write(machine->regs.d.eflags, 1<<OVERFLOW_FLAG_BIT, 0); // OVERFLOW FLAG
    uint8_t parrity = result & 1;
    for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
    bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> (width - 1)); //SIGN FLAG
    //AUX MARKED AS UNDEFINED IN INTEL DOCUMENTATION
    if(width == 8) *((uint8_t*)dest) = result & 0xFF;
    else if(width == 16) *((uint16_t*)dest) = result & 0xFFFF;
    else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
}

int16_t perform_xor(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry)
{
    uint32_t result = 0;
    if(width == 8)
    {
        result = *((uint8_t*)dest) ^ *((uint8_t*)source);
    } else if(width == 16)
    {
        result = *((uint16_t*)dest) ^ *((uint16_t*)source);
    } else if(width == 32)
    {
        result = *((uint32_t*)dest) ^ *((uint32_t*)source);
    }
    else return -1;
    bit_write(machine->regs.d.eflags, 1<<CARRY_FLAG_BIT, 0); // CARRY FLAG
    bit_write(machine->regs.d.eflags, 1<<OVERFLOW_FLAG_BIT, 0); // OVERFLOW FLAG
    uint8_t parrity = result & 1;
    for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
    bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> (width - 1)); //SIGN FLAG
    //AUX MARKED AS UNDEFINED IN INTEL DOCUMENTATION
    if(width == 8) *((uint8_t*)dest) = result & 0xFF;
    else if(width == 16) *((uint16_t*)dest) = result & 0xFFFF;
    else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
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
    } else return -1;
    result = dest_before - source_before;
    bit_write(machine->regs.d.eflags, 1<<CARRY_FLAG_BIT, (result >> width) ? 1 : 0); // CARRY FLAG
    bit_write(machine->regs.d.eflags, 1<<AUX_CARRY_FLAG_BIT, (((dest_before & 0xf) - (source_before & 0xf)) >> 4) ? 1: 0); //AUX CARRY FLAG
    uint8_t parrity = result & 1;
    for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
    bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
    bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
    bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> (width - 1)); //SIGN FLAG
    bit_write(machine->regs.d.eflags, 1<<OVERFLOW_FLAG_BIT, ((result >> (width - 1)) != (dest_before >> (width - 1)))); //OVERFLOW FLAG
    return 0;
}

int16_t perform_artihmetic_or_logical_instruction(v8086* machine, uint8_t recalculated_opcode, uint32_t carry, int16_t (*operation)(v8086*, void*, void*, uint8_t, uint32_t))
{
    //Maybe Mod/RM, Can be Immediate
    uint8_t mod_rm_or_immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
        
    uint8_t width = 16;
    void* source = NULL;
    void* dest = NULL;
    if(!(recalculated_opcode % 2)) width = 8; //Odd Opcode means 16 or 32 bit operands
    else if(machine->internal_state.operand_32_bit) width = 32;
    switch(recalculated_opcode)
    {
        case 0: //OPERATION r/m8, r8
            source = get_byte_register(machine, (mod_rm_or_immediate >> 3) & 7);
            dest = get_memory_from_mode(machine, mod_rm_or_immediate, 8);
            break;
        case 1: //OPERATION r/m32, r32 or OPERATION r/m16, r16
            source = get_variable_length_register(machine, (mod_rm_or_immediate >> 3) & 7, width);
            dest = get_memory_from_mode(machine, mod_rm_or_immediate, width);
            break;
        case 2: //OPERATION r8, r/m8
            dest = get_byte_register(machine, (mod_rm_or_immediate >> 3) & 7);
            source = get_memory_from_mode(machine, mod_rm_or_immediate, 8);
            break;
        case 3: //OPERATION r32, r/m32 or OPERATION r16, r/m16
            dest = get_variable_length_register(machine, (mod_rm_or_immediate >> 3) & 7, width);
            source = get_memory_from_mode(machine, mod_rm_or_immediate, width);
            break;
        case 4: //OPERATION AL, imm8
            dest = get_byte_register(machine, AL);
            source = &(mod_rm_or_immediate);
            break;
        case 5: //OPERATION EAX, imm32 or OPERATION AX, imm16
            dest = get_variable_length_register(machine, AX, width);
            if(width == 32)
                source = get_dword_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP - 1 + machine->internal_state.IPOffset));
            else 
                source = get_word_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP - 1 + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += ((width / 8) - 1);
            break;
    }
    perform_adding(machine, dest, source, width, carry);
    return 0;
}

int16_t parse_and_execute_instruction(v8086* machine)
{
    machine->internal_state.IPOffset = 0;
    machine->internal_state.operand_32_bit = 0;
    machine->internal_state.segment_reg_select = DEFAULT;
    machine->internal_state.rep_prefix = NONE;

    //Maybe opcode, an be also prefix
    uint8_t opcode;
    decode: opcode = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    
    //PREFIXES
    //Segment Prefix CS DS ES SS
    if((opcode & 0x7) == 0x6 && ((opcode >> 5) & 0x7) == 0x1) //001XX110 pattern where XX is number of segment
    {
        machine->internal_state.segment_reg_select = (opcode >> 3) & 0x3;
        goto decode; //continue parsing opcode;
    }
    //Segment Prefix FS
    else if(opcode == 0x64)
    {
        machine->internal_state.segment_reg_select = FS;
        goto decode; //continue parsing opcode;
    }
    //Segment Prefix GS
    else if(opcode == 0x65)
    {
        machine->internal_state.segment_reg_select = GS;
        goto decode; //continue parsing opcode;
    }
    //Operand Size Prefix
    else if(opcode == 0x66)
    {
        machine->internal_state.operand_32_bit = 1;
        goto decode; //continue parsing opcode;
    }
    //REPNE Prefix
    else if(opcode == 0xF2)
    {
        machine->internal_state.rep_prefix = REPNE;
        goto decode; //continue parsing opcode;
    }
    //REP/REPE Prefix
    else if(opcode == 0xF3)
    {
        machine->internal_state.rep_prefix = REP_REPE;
        goto decode; //continue parsing opcode;
    }
    //LOCK Prefix
    else if(opcode == 0xF0)
    {
        goto decode; //ommit prefix, contniue parsinf opcode; 
    }
    //Aritmetic operations
    //ADD
    else if(opcode <= 5)
    {
        perform_artihmetic_or_logical_instruction(machine, opcode, 0, perform_adding);
    }
    //ADC
    else if(opcode >= 0x10 && opcode <= 0x15)
    {
        perform_artihmetic_or_logical_instruction(machine, opcode - 0x10, bit_get(machine->regs.d.eflags, 1<<CARRY_FLAG_BIT) >> CARRY_FLAG_BIT, perform_adding);
    }
    //SBB
    else if(opcode >= 0x18 && opcode <= 0x1d)
    {
        perform_artihmetic_or_logical_instruction(machine, opcode - 0x18, bit_get(machine->regs.d.eflags, 1<<CARRY_FLAG_BIT) >> CARRY_FLAG_BIT, perform_subtracting);
    }
    //SUB
    else if(opcode >= 0x28 && opcode <= 0x2d)
    {
        perform_artihmetic_or_logical_instruction(machine, opcode - 0x28, 0, perform_subtracting);
    }
    //INC general registers 16 or 32-bit
    else if(opcode >= 0x40 && opcode <= 0x47)
    {
        uint8_t width = 16;
        void* dest = NULL;
        uint64_t result = 0;
        uint32_t dest_before;
        if(machine->internal_state.operand_32_bit) width=32;
        dest = get_variable_length_register(machine, opcode & 7, width);
        if(width == 16) dest_before = *((uint16_t*)dest);
        else if(width == 32) dest_before = *((uint32_t*)dest);
        else return -1;

        result = dest_before + 1;

        uint8_t parrity = result & 1;
        for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
        bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
        bit_write(machine->regs.d.eflags, 1<<AUX_CARRY_FLAG_BIT, (((dest_before & 0xf) + 1) >> 4) ? 1: 0); //AUX CARRY FLAG
        bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
        bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> (width - 1)); //SIGN FLAG
        bit_write(machine->regs.d.eflags, 1<<OVERFLOW_FLAG_BIT, ((result >> (width - 1)) != (dest_before >> (width - 1)))); //OVERFLOW FLAG
        if(width == 16) *((uint16_t*)dest) = result & 0xFFFF;
        else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
    }
    //DEC general registers 16 or 32-bit
    else if(opcode >= 0x48 && opcode <= 0x4f)
    {
        uint8_t width = 16;
        void* dest = NULL;
        uint64_t result = 0;
        uint32_t dest_before;
        if(machine->internal_state.operand_32_bit) width=32;
        dest = get_variable_length_register(machine, opcode & 7, width);
        if(width == 16) dest_before = *((uint16_t*)dest);
        else if(width == 32) dest_before = *((uint32_t*)dest);
        else return -1;

        result = dest_before - 1;

        uint8_t parrity = result & 1;
        for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
        bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
        bit_write(machine->regs.d.eflags, 1<<AUX_CARRY_FLAG_BIT, (((dest_before & 0xf) - 1) >> 4) ? 1: 0); //AUX CARRY FLAG
        bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
        bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> (width - 1)); //SIGN FLAG
        bit_write(machine->regs.d.eflags, 1<<OVERFLOW_FLAG_BIT, ((result >> (width - 1)) != (dest_before >> (width - 1)))); //OVERFLOW FLAG
        if(width == 16) *((uint16_t*)dest) = result & 0xFFFF;
        else if(width == 32) *((uint32_t*)dest) = result & 0xFFFFFFFF;
    }
    //LOGICAL operations
    //OR
    else if(opcode >= 0x08 && opcode <= 0x0d)
    {
        perform_artihmetic_or_logical_instruction(machine, opcode - 0x08, 0, perform_or);
    }
    //AND
    else if(opcode >= 0x20 && opcode <= 0x25)
    {
        perform_artihmetic_or_logical_instruction(machine, opcode - 0x20, 0, perform_and);
    }
    //XOR
    else if(opcode >- 0x30 && opcode <= 0x35)
    {
        perform_artihmetic_or_logical_instruction(machine, opcode - 0x30, 0, perform_xor);
    }
    //CMP
    else if(opcode >= 0x38 && opcode <= 0x3d)
    {
        perform_artihmetic_or_logical_instruction(machine, opcode - 0x30, 0, perform_cmp);
    }
    //PUSH Operations
    //PUSH General purpose registers
    else if(opcode >= 0x50 && opcode <= 0x57)
    {
        uint8_t width = 16;
        void* reg = NULL;
        if(machine->internal_state.operand_32_bit) width = 16;
        reg = get_variable_length_register(machine, opcode & 7, width);
        if(width==16) push_word(machine, *((uint16_t*)reg));
        else if(width==32) push_dword(machine, *((uint32_t*)reg));
        else return -1;
    }
    //PUSH CS
    else if(opcode == 0x0e)
        push_word(machine, machine->sregs.cs);
    //PUSH DS
    else if(opcode == 0x1e)
        push_word(machine, machine->sregs.ds);
    //PUSH ES
    else if(opcode == 0x06)
        push_word(machine, machine->sregs.es);
    //PUSH SS
    else if(opcode == 0x16)
        push_word(machine, machine->sregs.ss);
    //POP Operations
    //POP General purpose registers
    else if(opcode >= 0x50 && opcode <= 0x57)
    {
        uint8_t width = 16;
        void* reg = NULL;
        if(machine->internal_state.operand_32_bit) width = 16;
        reg = get_variable_length_register(machine, opcode & 7, width);
        if(width==16) *((uint16_t*)reg) = pop_word(machine);
        else if(width==32) *((uint32_t*)reg) = pop_dword(machine);
        else return -1;
    }
    //POP DS
    else if(opcode == 0x1f)
        machine->sregs.ds = pop_word(machine);
    //POP ES
    else if(opcode == 0x07)
        machine->sregs.es = pop_word(machine);
    //POP SS
    else if(opcode == 0x17)
        machine->sregs.ss = pop_word(machine);
    //XCHG group
    //XCHG GRP with AX or EAX
    else if(opcode >= 90 && opcode <= 97)
    {
        uint8_t width = 16;
        if(machine->internal_state.operand_32_bit) width = 32;
        if(width == 16)
        {
            uint16_t temp;
            uint16_t* regA = get_word_register(machine, AX); 
            uint16_t* regB = get_word_register(machine, opcode & 7);
            temp = *regA;
            *regA = *regB;
            *regB = temp;
        }
        else if(width == 32)
        {
            uint16_t temp;
            uint16_t* regA = get_dword_register(machine, EAX); 
            uint16_t* regB = get_dword_register(machine, opcode & 7);
            temp = *regA;
            *regA = *regB;
            *regB = temp;
        }
        else return -1;
    }
    //XCHG r8, rm8 or XCHG r16, rm16 or XCHG r32, rm32
    else if(opcode >= 0x86 && opcode <= 0x87)
    {
        int8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        if(opcode == 0x86)
        {
            uint8_t* source = get_byte_register(machine, (mod_rm >> 3) & 7);
            uint8_t* dest = get_memory_from_mode(machine, mod_rm, 8);
            uint8_t temp;
            temp = *source;
            *source = *dest;
            *dest = temp;
        }
        else if(opcode  == 0x87)
        {
            if(machine->internal_state.operand_32_bit)
            {
                uint32_t* source = get_dword_register(machine, (mod_rm >> 3) & 7);
                uint32_t* dest = get_memory_from_mode(machine, mod_rm, 32);
                uint32_t temp;
                temp = *source;
                *source = *dest;
                *dest = temp;
            }
            else
            {
                uint16_t* source = get_word_register(machine, (mod_rm >> 3) & 7);
                uint16_t* dest = get_memory_from_mode(machine, mod_rm, 16);
                uint16_t temp;
                temp = *source;
                *source = *dest;
                *dest = temp;
            }
            
        }
    }
    //SHORT JUMPS
    else if(opcode >= 0x70 && opcode <= 0x7f)
    {
        int8_t offset = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint32_t tempIP = machine->IP;
        uint8_t jump = 0;
        switch(opcode & 0x0f)
        {
            case 0: //JO
                if(bit_get(machine->regs.x.flags, 1<<OVERFLOW_FLAG_BIT)) jump = 1;
                break;
            case 1: //JNO
                if(!bit_get(machine->regs.x.flags, 1<<OVERFLOW_FLAG_BIT)) jump = 1;
                break;
            case 2: //JB or JNAE or JC
                if(bit_get(machine->regs.x.flags, 1<<CARRY_FLAG_BIT)) jump = 1;
                break;
            case 3: //JNB or JAE or JNC
                if(!bit_get(machine->regs.x.flags, 1<<CARRY_FLAG_BIT)) jump = 1;
                break;
            case 4: //JZ or JE
                if(!bit_get(machine->regs.x.flags, 1<<ZERO_FLAG_BIT)) jump = 1;
                break;
            case 5: //JNZ or JNE
                if(!bit_get(machine->regs.x.flags, 1<<ZERO_FLAG_BIT)) jump = 1;
                break;
            case 6: //JBE or JNA
                if(bit_get(machine->regs.x.flags, 1<<CARRY_FLAG_BIT) || bit_get(machine->regs.x.flags, 1<<ZERO_FLAG_BIT)) jump = 1;
                break;
            case 7: //JNBE or JA
                if(!(bit_get(machine->regs.x.flags, 1<<CARRY_FLAG_BIT) || bit_get(machine->regs.x.flags, 1<<ZERO_FLAG_BIT))) jump = 1;
                break;
            case 8: //JS
                if(bit_get(machine->regs.x.flags, 1<<SIGN_FLAG_BIT)) jump = 1;
                break;
            case 9: //JNS
                if(!bit_get(machine->regs.x.flags, 1<<SIGN_FLAG_BIT)) jump = 1;
                break;
            case 0xa: //JP or JPE
                if(bit_get(machine->regs.x.flags, 1<<PARITY_FLAG_BIT)) jump = 1;
                break;
            case 0xb: //JNP or JPO
                if(!bit_get(machine->regs.x.flags, 1<<PARITY_FLAG_BIT)) jump = 1;
                break;
            case 0xc: //JL or JNGE
                if(!bit_get(machine->regs.x.flags, 1<<SIGN_FLAG_BIT) != !bit_get(machine->regs.x.flags, 1<<OVERFLOW_FLAG_BIT)) jump = 1;
                break; 
            case 0xd: //JNL or JGE
                if(!bit_get(machine->regs.x.flags, 1<<SIGN_FLAG_BIT) == !bit_get(machine->regs.x.flags, 1<<OVERFLOW_FLAG_BIT)) jump = 1;
                break; 
            case 0xe: //JLE or JNG
                if(bit_get(machine->regs.x.flags, 1<<ZERO_FLAG_BIT) || !bit_get(machine->regs.x.flags, 1<<SIGN_FLAG_BIT) != !bit_get(machine->regs.x.flags, 1<<OVERFLOW_FLAG_BIT)) jump = 1;
                break;
            case 0xf: //JNLE or JG
                if(bit_get(machine->regs.x.flags, 1<<ZERO_FLAG_BIT) || !bit_get(machine->regs.x.flags, 1<<SIGN_FLAG_BIT) == !bit_get(machine->regs.x.flags, 1<<OVERFLOW_FLAG_BIT)) jump = 1;
                break;
        }
        if(jump) tempIP += offset;
        if(tempIP > 0xFFFF) return -1;
        machine->IP = tempIP;
    }
    //MOV Group
    //MOV r8, imm8
    else if(opcode >= 0xb0 && opcode <= 0xb7)
    {
        uint8_t* reg = get_byte_register(machine, opcode & 0x7);
        uint8_t imm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        *reg = imm;
    }
    //MOV r16, imm16 or r32, imm32
    else if(opcode >= 0xb8 && opcode <= 0xbf)
    {
        if(machine->internal_state.operand_32_bit)
        {
            uint32_t* reg = get_dword_register(machine, (opcode - 0xb8) & 0x7);
            uint32_t imm = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 4;
            *reg = imm;
        }
        else
        {
            uint16_t* reg = get_word_register(machine, (opcode - 0xb8) & 0x7);
            uint16_t imm = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 2;
            *reg = imm; 
        }
    }
    //MOV AL/AX/EAX, moffs8/moffs16/moffs32 or MOV moffs8/moffs16/moffs32, AL/AX/EAX
    else if(opcode >= 0xa0 && opcode <= 0xa3)
    {
        uint16_t offset = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 2;
        uint16_t segment = *select_segment_register(machine, DS) ? machine->internal_state.segment_reg_select == DEFAULT : *select_segment_register(machine, machine->internal_state.segment_reg_select);

        switch (opcode)
        {
        case 0xa0:
            machine->regs.h.al = read_byte_from_pointer(machine->Memory, get_absolute_address(segment, offset));
            break;
        case 0xa1:
            if(machine->internal_state.operand_32_bit) machine->regs.d.eax = read_dword_from_pointer(machine->Memory, get_absolute_address(segment, offset));
            else machine->regs.x.ax = read_word_from_pointer(machine->Memory, get_absolute_address(segment, offset));
            break;
        case 0xa2:
            write_byte_to_pointer(machine->Memory, get_absolute_address(segment, offset), machine->regs.h.al);
            break;
        case 0xa3:
            if(machine->internal_state.operand_32_bit) write_dword_to_pointer(machine->Memory, get_absolute_address(segment, offset), machine->regs.d.eax);
            else write_word_to_pointer(machine->Memory, get_absolute_address(segment, offset), machine->regs.x.ax);
            break;
        }
    }
    //MOV r8/r16/r32, rm8/rm16/rm32 or MOV rm8/rm16/rm32, r8/r16/r32
    else if(opcode >= 0x88 && opcode <= 0x8b)
    {
        //Mod/RM
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        void* source = NULL;
        void* dest = NULL;
        uint8_t width = machine->internal_state.operand_32_bit ? 32 : 16;
        switch (opcode)
        {
        case 0x88:
            source = get_byte_register(machine, (mod_rm >> 3) & 7);
            dest = get_memory_from_mode(machine, mod_rm, 8);
            *((uint8_t*)dest) = *((uint8_t*) source);
            break;
        case 0x89:
            source = get_variable_length_register(machine, (mod_rm >> 3) & 7, width);
            dest = get_memory_from_mode(machine, mod_rm, width);
            if(width == 16) *((uint16_t*)dest) = *((uint16_t*) source);
            else *((uint32_t*)dest) = *((uint32_t*) source);
            break;
        case 0x8a:
            dest = get_byte_register(machine, (mod_rm >> 3) & 7);
            source = get_memory_from_mode(machine, mod_rm, 8);
            *((uint8_t*)dest) = *((uint8_t*) source);
            break;
        case 0x8b:
            dest = get_variable_length_register(machine, (mod_rm >> 3) & 7, width);
            source = get_memory_from_mode(machine, mod_rm, width);
            if(width == 16) *((uint16_t*)dest) = *((uint16_t*) source);
            else *((uint32_t*)dest) = *((uint32_t*) source);
            break;
        }
    }
    //MOV Segment to/from r/m
    else if(opcode == 0x8c || opcode == 0x8e)
    {
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint16_t* source = NULL;
        uint16_t* dest = NULL;
        if(opcode == 0x8c)
        {
            source = select_segment_register(machine, (mod_rm >> 3) & 7);
            dest = get_memory_from_mode(machine, mod_rm, 16);
        }
        else
        {
            dest = select_segment_register(machine, (mod_rm >> 3) & 7);
            source = get_memory_from_mode(machine, mod_rm, 16);
        }
        *dest = *source;
    }
    //MOV rm, imm
    else if(opcode >= 0xc6 && opcode <= 0xc7)
    {
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        if(opcode == 0xc6)
        {
            uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 1;
            uint8_t* mem = get_memory_from_mode(machine, mod_rm, 8);
            *mem = immediate;
        }
        else
        {
            if(machine->internal_state.operand_32_bit)
            {
                uint32_t immediate = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 4;
                uint32_t* mem = get_memory_from_mode(machine, mod_rm, 32);
                *mem = immediate;
            }   
            else
            {
                uint16_t immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 2;
                uint16_t* mem = get_memory_from_mode(machine, mod_rm, 16);
                *mem = immediate;
            }
             
        }
        
    }
    //TEST GROUP
    else if(opcode == 0x84)
    {
        //Mod/RM
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t* reg = get_byte_register(machine, (mod_rm >> 3) & 7);
        uint8_t* memory = get_memory_from_mode(machine, mod_rm, 8);
        uint8_t result = *reg & *memory;
        bit_clear(machine->regs.d.eflags, 1<<CARRY_FLAG_BIT);
        bit_clear(machine->regs.d.eflags, 1<<OVERFLOW_FLAG_BIT);
        uint8_t parrity = result & 1;
        for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
        bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
        bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
        bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> 7); //SIGN FLAG
    }
    else if(opcode == 0x85)
    {
        //Mod/RM
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t width = 16;
        if(machine->internal_state.operand_32_bit) width = 32;
        uint32_t result;
        void* source = get_variable_length_register(machine, (mod_rm >> 3) & 7, width);
        void* dest = get_memory_from_mode(machine, mod_rm, width);
        
        if(width == 16)
            result = *((uint16_t*) source) & *((uint16_t*) dest);
        else if(width == 32)
            result = *((uint32_t*) source) & *((uint32_t*) dest);

        uint8_t parrity = result & 1;
        for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
        bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
        bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
        bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> (width - 1)); //SIGN FLAG
    }
    else if(opcode == 0xa8)
    {
        //Mod/RM
        uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t* reg = get_byte_register(machine, AL);
        uint8_t result = *reg & immediate;
        bit_clear(machine->regs.d.eflags, 1<<CARRY_FLAG_BIT);
        bit_clear(machine->regs.d.eflags, 1<<OVERFLOW_FLAG_BIT);
        uint8_t parrity = result & 1;
        for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
        bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
        bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
        bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> 7); //SIGN FLAG
    }
    else if(opcode == 0xa9)
    {
        //Mod/RM
        uint32_t immediate; 
        uint8_t width = 16;
        if(machine->internal_state.operand_32_bit) width = 32;
        uint32_t result;
        if(width == 16)
        {
            immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 2;
        }
        else if(width == 32)
        {
            immediate = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 4;
        }

        void* reg = get_variable_length_register(machine, AX, width);
        
        if(width == 16)
            result = *((uint16_t*) reg) & immediate;
        else if(width == 32)
            result = *((uint32_t*) reg) & immediate;

        uint8_t parrity = result & 1;
        for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
        bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
        bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
        bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> (width - 1)); //SIGN FLAG
    }
    //STRING GROUP
    //MOVSB
    else if(opcode==0xA4)
    {
        uint16_t* source_segment;
        uint16_t* dest_segment = select_segment_register(machine, ES);
        uint8_t* source;
        uint8_t* dest;
        if(machine->internal_state.segment_reg_select == DEFAULT) source_segment = select_segment_register(machine, DS);
        else source_segment = select_segment_register(machine, machine->internal_state.segment_reg_select);

        //if repeat and number of repats == 0 -> dont copy anything
        if(machine->internal_state.rep_prefix == REP_REPE && machine->regs.w.cx == 0) goto recalculate_ip; 

        do{
            source = get_byte_pointer(machine->Memory, get_absolute_address(*source_segment, machine->regs.w.si));
            dest = get_byte_pointer(machine->Memory, get_absolute_address(*dest_segment, machine->regs.w.di));
            *dest = *source;
            machine->regs.w.si += bit_get(machine->regs.w.flags, 1 << DIRECTION_FLAG_BIT) ? -1 : 1;
            machine->regs.w.di += bit_get(machine->regs.w.flags, 1 << DIRECTION_FLAG_BIT) ? -1 : 1;
        } while(machine->internal_state.rep_prefix == REP_REPE && --(machine->regs.w.cx));
    }
    //MOVSW or MOVSD
    else if(opcode == 0xA5)
    {
        uint16_t* source_segment;
        uint16_t* dest_segment = select_segment_register(machine, ES);
        if(machine->internal_state.segment_reg_select == DEFAULT) source_segment = select_segment_register(machine, DS);
        else source_segment = select_segment_register(machine, machine->internal_state.segment_reg_select);

        //if repeat and number of repats == 0 -> dont copy anything
        if(machine->internal_state.rep_prefix == REP_REPE && machine->regs.w.cx == 0) goto recalculate_ip; 

        if(machine->internal_state.operand_32_bit)
            do{
                uint32_t* source = get_dword_pointer(machine->Memory, get_absolute_address(*source_segment, machine->regs.w.si));
                uint32_t* dest = get_dword_pointer(machine->Memory, get_absolute_address(*dest_segment, machine->regs.w.di));
                *dest = *source;
                machine->regs.w.si += bit_get(machine->regs.w.flags, 1 << DIRECTION_FLAG_BIT) ? -4 : 4;
                machine->regs.w.di += bit_get(machine->regs.w.flags, 1 << DIRECTION_FLAG_BIT) ? -4 : 4;
            } while(machine->internal_state.rep_prefix == REP_REPE && --(machine->regs.w.cx));
        else
            do{
                uint16_t* source = get_word_pointer(machine->Memory, get_absolute_address(*source_segment, machine->regs.w.si));
                uint16_t* dest = get_word_pointer(machine->Memory, get_absolute_address(*dest_segment, machine->regs.w.di));
                *dest = *source;
                machine->regs.w.si += bit_get(machine->regs.w.flags, 1 << DIRECTION_FLAG_BIT) ? -2 : 2;
                machine->regs.w.di += bit_get(machine->regs.w.flags, 1 << DIRECTION_FLAG_BIT) ? -2 : 2;
            } while(machine->internal_state.rep_prefix == REP_REPE && --(machine->regs.w.cx));
        
    }
    //CMPSB or CMPSW or CMPSD
    else if(opcode >= 0xA6 && opcode <= 0xA7)
    {
        uint16_t* source_segment;
        uint16_t* dest_segment = select_segment_register(machine, ES);
        void* source;
        void* dest;
        uint32_t dest_before; //for overflow flag checking
        uint32_t source_before;
        uint64_t result;
        uint8_t width = 8;
        if(opcode == 0xA7)
            if(machine->internal_state.operand_32_bit) width=32;
            else width = 16;
        
        if(machine->internal_state.segment_reg_select == DEFAULT) source_segment = select_segment_register(machine, DS);
        else source_segment = select_segment_register(machine, machine->internal_state.segment_reg_select);

        //if repeat and number of repats == 0 -> dont copy anything
        if(machine->internal_state.rep_prefix != NONE && machine->regs.w.cx == 0) goto recalculate_ip; 

        do{
            source = get_variable_length_pointer(machine->Memory, get_absolute_address(*source_segment, machine->regs.w.si), width);
            dest = get_varaible_length_pointer(machine->Memory, get_absolute_address(*dest_segment, machine->regs.w.di), width);
            if(width == 8){
                dest_before = *((uint8_t*)dest);
                source_before = *((uint8_t*)source);
            } else if(width == 16){
            dest_before = *((uint16_t*)dest);
                    source_before = *((uint16_t*)source);
            } else if(width == 32){
                dest_before = *((uint32_t*)dest);
                source_before = *((uint32_t*)source);
            } else return -1;
            
            result = dest_before - source_before;
            bit_write(machine->regs.d.eflags, 1<<CARRY_FLAG_BIT, (result >> width) ? 1 : 0); // CARRY FLAG
            bit_write(machine->regs.d.eflags, 1<<AUX_CARRY_FLAG_BIT, (((dest_before & 0xf) - (source_before & 0xf)) >> 4) ? 1: 0); //AUX CARRY FLAG
            uint8_t parrity = result & 1;
            for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
            bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
            bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
            bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> (width - 1)); //SIGN FLAG
            bit_write(machine->regs.d.eflags, 1<<OVERFLOW_FLAG_BIT, ((result >> (width - 1)) != (dest_before >> (width - 1)))); //OVERFLOW FLAG

            machine->regs.w.si += bit_get(machine->regs.w.flags, 1 << DIRECTION_FLAG_BIT) ? -(width/8) : (width/8);
            machine->regs.w.di += bit_get(machine->regs.w.flags, 1 << DIRECTION_FLAG_BIT) ? -(width/8) : (width/8);

            if(machine->internal_state.rep_prefix == REP_REPE && !bit_get(machine->regs.w.flags, ZERO_FLAG_BIT)) break;
            else if(machine->internal_state.rep_prefix == REPNE && bit_get(machine->regs.w.flags, ZERO_FLAG_BIT)) break;
        } while(machine->internal_state.rep_prefix != DEFAULT && --(machine->regs.w.cx));
    }
    //STOSB STOSW STOSD
    else if(opcode >= 0xAA && opcode <= 0xAB)
    {
        uint16_t* segment = select_segment_register(machine, ES);
        void* source;
        void* dest;
        uint8_t width = 8;
        if(opcode == 0xA7)
            if(machine->internal_state.operand_32_bit) width=32;
            else width = 16;
        
        //if repeat and number of repats == 0 -> dont copy anything
        if(machine->internal_state.rep_prefix == REP_REPE && machine->regs.w.cx == 0) goto recalculate_ip; 

        do{
            dest = get_variable_length_pointer(machine->Memory, get_absolute_address(*segment, machine->regs.w.di), width);
            source = get_variable_length_register(machine, AL, width);

            if(width == 8) *((uint8_t*) dest) = *((uint8_t*) source);
            else if(width == 16) *((uint16_t*) dest) = *((uint16_t*) source);
            else if(width == 32) *((uint32_t*) dest) = *((uint32_t*) source);
            else return -1;

            machine->regs.w.di += bit_get(machine->regs.w.flags, 1 << DIRECTION_FLAG_BIT) ? -(width/8) : (width/8);
        } while(machine->internal_state.rep_prefix == REP_REPE && --(machine->regs.w.cx));
    }
    //LOADB LOADW LOADD
    else if(opcode >= 0xAC && opcode <= 0xAD)
    {
        uint16_t* segment;
        void* source;
        void* dest;
        uint8_t width = 8;
        if(opcode == 0xA7)
            if(machine->internal_state.operand_32_bit) width=32;
            else width = 16;

        if(machine->internal_state.segment_reg_select == DEFAULT) segment = select_segment_register(machine, DS);
        else segment = select_segment_register(machine, machine->internal_state.segment_reg_select);
        
        //if repeat and number of repats == 0 -> dont copy anything
        if(machine->internal_state.rep_prefix == REP_REPE && machine->regs.w.cx == 0) goto recalculate_ip; 

        do{
            source = get_variable_length_pointer(machine->Memory, get_absolute_address(*segment, machine->regs.w.si), width);
            dest = get_variable_length_register(machine, AL, width);

            if(width == 8) *((uint8_t*) dest) = *((uint8_t*) source);
            else if(width == 16) *((uint16_t*) dest) = *((uint16_t*) source);
            else if(width == 32) *((uint32_t*) dest) = *((uint32_t*) source);
            else return -1;

            machine->regs.w.si += bit_get(machine->regs.w.flags, 1 << DIRECTION_FLAG_BIT) ? -(width/8) : (width/8);
        }while(machine->internal_state.rep_prefix == REP_REPE && --(machine->regs.w.cx));
    }
    //SCASB or SCASW or SCADD
    else if(opcode >= 0xAE && opcode <= 0xAF)
    {
        uint16_t* source_segment = select_segment_register(machine, ES);
        void* source;
        void* dest;
        uint32_t dest_before; //for overflow flag checking
        uint32_t source_before;
        uint64_t result;
        uint8_t width = 8;
        if(opcode == 0xA7)
            if(machine->internal_state.operand_32_bit) width=32;
            else width = 16;
        
        //if repeat and number of repats == 0 -> dont copy anything
        if(machine->internal_state.rep_prefix != NONE && machine->regs.w.cx == 0) goto recalculate_ip; 

        do{
            dest = get_variable_length_register(machine, AX, width);
            source = get_varaible_length_pointer(machine->Memory, get_absolute_address(*source_segment, machine->regs.w.di), width);
            if(width == 8){
                dest_before = *((uint8_t*)dest);
                source_before = *((uint8_t*)source);
            } else if(width == 16){
            dest_before = *((uint16_t*)dest);
                    source_before = *((uint16_t*)source);
            } else if(width == 32){
                dest_before = *((uint32_t*)dest);
                source_before = *((uint32_t*)source);
            } else return -1;
            
            result = dest_before - source_before;
            bit_write(machine->regs.d.eflags, 1<<CARRY_FLAG_BIT, (result >> width) ? 1 : 0); // CARRY FLAG
            bit_write(machine->regs.d.eflags, 1<<AUX_CARRY_FLAG_BIT, (((dest_before & 0xf) - (source_before & 0xf)) >> 4) ? 1: 0); //AUX CARRY FLAG
            uint8_t parrity = result & 1;
            for(int i = 1; i < 8; i++) parrity ^= (result >> i) & 1;
            bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
            bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
            bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, result >> (width - 1)); //SIGN FLAG
            bit_write(machine->regs.d.eflags, 1<<OVERFLOW_FLAG_BIT, ((result >> (width - 1)) != (dest_before >> (width - 1)))); //OVERFLOW FLAG

            machine->regs.w.di += bit_get(machine->regs.w.flags, 1 << DIRECTION_FLAG_BIT) ? -(width/8) : (width/8);

            if(machine->internal_state.rep_prefix == REP_REPE && !bit_get(machine->regs.w.flags, ZERO_FLAG_BIT)) break;
            else if(machine->internal_state.rep_prefix == REPNE && bit_get(machine->regs.w.flags, ZERO_FLAG_BIT)) break;
        } while(machine->internal_state.rep_prefix != DEFAULT && --(machine->regs.w.cx));
    }
    //ASCII ADJUSTMENT
    //AAA
    else if(opcode == 0x37)
    {
        if(((machine->regs.h.al & 0x0f) > 9) || bit_get(machine->regs.w.flags, 1<<AUX_CARRY_FLAG_BIT))
        {
            machine->regs.x.ax += 0x106;
            bit_set(machine->regs.w.flags, 1<<AUX_CARRY_FLAG_BIT);
            bit_set(machine->regs.w.flags, 1<<CARRY_FLAG_BIT);
        }
        else
        {
            bit_clear(machine->regs.w.flags, 1<<AUX_CARRY_FLAG_BIT);
            bit_clear(machine->regs.w.flags, 1<<CARRY_FLAG_BIT);
        }
        machine->regs.h.al &= 0x0f;
    }
    //AAS
    else if(opcode == 0x3F)
    {
        if(((machine->regs.h.al & 0x0f) > 9) || bit_get(machine->regs.w.flags, 1<<AUX_CARRY_FLAG_BIT))
        {
            machine->regs.h.al -= 0x6;
            machine->regs.h.ah -= 1;
            bit_set(machine->regs.w.flags, 1<<AUX_CARRY_FLAG_BIT);
            bit_set(machine->regs.w.flags, 1<<CARRY_FLAG_BIT);
        }
        else
        {
            bit_clear(machine->regs.w.flags, 1<<AUX_CARRY_FLAG_BIT);
            bit_clear(machine->regs.w.flags, 1<<CARRY_FLAG_BIT);
        }
        machine->regs.h.al &= 0x0f;
    }
    //DAA
    else if(opcode == 0x27)
    {
        uint8_t old_AL = machine->regs.h.al;
        uint16_t old_cf = bit_get(machine->regs.w.flags, 1<<CARRY_FLAG_BIT);
        bit_clear(machine->regs.w.flags, 1<<CARRY_FLAG_BIT);
        if(((machine->regs.h.al & 0x0f) > 9) || bit_get(machine->regs.w.flags, 1<<AUX_CARRY_FLAG_BIT))
        {
            uint16_t temp_ax = machine->regs.h.al + 6;
            machine->regs.h.al = temp_ax & 0xFF;
            bit_write(machine->regs.w.flags, 1<<CARRY_FLAG_BIT, (old_cf || (temp_ax > 0xFF)) ? 1 : 0);
            bit_set(machine->regs.w.flags, 1<<AUX_CARRY_FLAG_BIT);
        }
        else bit_clear(machine->regs.w.flags, 1<<AUX_CARRY_FLAG_BIT);

        if((old_AL > 0x99) || old_cf)
        {
            machine->regs.h.al += 0x60;
            bit_set(machine->regs.w.flags, 1<<CARRY_FLAG_BIT);
        }
        else bit_clear(machine->regs.w.flags, 1<<CARRY_FLAG_BIT);
        uint8_t parrity = machine->regs.h.al & 1;
        for(int i = 1; i < 8; i++) parrity ^= (machine->regs.h.al >> i) & 1;
        bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
        bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, machine->regs.h.al == 0); //ZERO FLAG
        bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, machine->regs.h.al >> (7)); //SIGN FLAG
    }
    //DAS
    else if(opcode == 0x2f)
    {
        uint8_t old_AL = machine->regs.h.al;
        uint16_t old_cf = bit_get(machine->regs.w.flags, 1<<CARRY_FLAG_BIT);
        bit_clear(machine->regs.w.flags, 1<<CARRY_FLAG_BIT);
        if(((machine->regs.h.al & 0x0f) > 9) || bit_get(machine->regs.w.flags, 1<<AUX_CARRY_FLAG_BIT))
        {
            uint16_t temp_ax = machine->regs.h.al - 6;
            machine->regs.h.al = temp_ax & 0xFF;
            bit_write(machine->regs.w.flags, 1<<CARRY_FLAG_BIT, (old_cf || (temp_ax > 0xFF)) ? 1 : 0);
            bit_set(machine->regs.w.flags, 1<<AUX_CARRY_FLAG_BIT);
        }
        else bit_clear(machine->regs.w.flags, 1<<AUX_CARRY_FLAG_BIT);

        if((old_AL > 0x99) || old_cf)
        {
            machine->regs.h.al -= 0x60;
            bit_set(machine->regs.w.flags, 1<<CARRY_FLAG_BIT);
        }
        uint8_t parrity = machine->regs.h.al & 1;
        for(int i = 1; i < 8; i++) parrity ^= (machine->regs.h.al >> i) & 1;
        bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
        bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, machine->regs.h.al == 0); //ZERO FLAG
        bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, machine->regs.h.al >> (7)); //SIGN FLAG
    }
    //AAM
    else if(opcode == 0xd4)
    {
        uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t tempAL = machine->regs.h.al;
        machine->regs.h.ah = tempAL / immediate;
        machine->regs.h.al = tempAL % immediate;

        uint8_t parrity = machine->regs.h.al & 1;
        for(int i = 1; i < 8; i++) parrity ^= (machine->regs.h.al >> i) & 1;
        bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
        bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, machine->regs.h.al == 0); //ZERO FLAG
        bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, machine->regs.h.al >> (7)); //SIGN FLAG
    }
    //AAD
    else if(opcode == 0xd5)
    {
        uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t tempAL = machine->regs.h.al;
        uint8_t tempAH = machine->regs.h.ah;

        machine->regs.h.al = (tempAL + (tempAH * immediate)) & 0xFF;
        machine->regs.h.ah = 0;

        uint8_t parrity = machine->regs.h.al & 1;
        for(int i = 1; i < 8; i++) parrity ^= (machine->regs.h.al >> i) & 1;
        bit_write(machine->regs.d.eflags, 1<<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
        bit_write(machine-> regs.d.eflags, 1<<ZERO_FLAG_BIT, machine->regs.h.al == 0); //ZERO FLAG
        bit_write(machine->regs.d.eflags, 1<<SIGN_FLAG_BIT, machine->regs.h.al >> (7)); //SIGN FLAG
    }
    //LOAD Operations
    //LEA
    else if(opcode == 0x8d)
    {
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;

        uint16_t segment;
        uint16_t offset;

        if((mod_rm >> 6) > 3) return -1;

        int16_t r = calculate_segment_offset_from_mode(machine, mod_rm, &segment, &offset);
        if(r) return r;
        
        if(machine->internal_state.operand_32_bit)
        {
            uint32_t* reg = get_dword_register(machine, (mod_rm>>3)&7);
            *reg = offset;
        }
        else
        {
            uint16_t* reg = get_word_register(machine, (mod_rm>>3)&7);
            *reg = offset;
        }
    }
    //LDS or LES
    else if(opcode >= 0xc4 && opcode <= 0xc5)
    {
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;

        uint16_t* segment_register;
        if(opcode == 0xc4) segment_register = select_segment_register(machine, ES);
        else segment_register = select_segment_register(machine, DS);

        uint16_t* source = get_memory_from_mode(machine, mod_rm, 16);

        if(machine->internal_state.operand_32_bit)
        {
            uint16_t* dest = get_dword_register(machine, (mod_rm >> 3) & 7);
            *dest = *((uint32_t*) source);
            *segment_register = *(source+2);
        }
        else
        {
            uint16_t* dest = get_word_register(machine, (mod_rm >> 3) & 7);
            *dest = *source;
            *segment_register = *(source+1);
        }
        
    }
    recalculate_ip: machine->IP += machine->internal_state.IPOffset;

    return 0;
}