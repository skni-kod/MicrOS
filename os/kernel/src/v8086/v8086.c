#include "v8086.h"
#include "../memory/heap/heap.h"
#include <string.h>
#include "../assembly/io.h"
#include "./memory_operations.h"
#include "./stack.h"
#include "./mod_rm_parsing.h"
#include "./operations/internal_funcs.h"
#include "./operations/arithmetic_operations.h"
#include "./operations/opcodes.h"

int16_t parse_and_execute_instruction(v8086* machine);

void v8086_set_8086_instruction_set(v8086* machine)
{
    GROUP_OF_OPCODES(0x00u, 0x05u, add);
    ASSIGN_OPCODE(0x06, push_es);
    ASSIGN_OPCODE(0x07, pop_es);
    GROUP_OF_OPCODES(0x08u, 0x0du, or);
    ASSIGN_OPCODE(0x0eu, push_cs);
    //THERE IS NO 2-BYTES OPCODES
    ASSIGN_NULL(0x0fu);
    GROUP_OF_OPCODES(0x10u, 0x15u, adc);
    ASSIGN_OPCODE(0x16u, push_ss);
    ASSIGN_OPCODE(0x17u, pop_ss);
    GROUP_OF_OPCODES(0x18u, 0x1du, sbb);
    ASSIGN_OPCODE(0x1eu, push_ds);
    ASSIGN_OPCODE(0x1fu, pop_ds);
    GROUP_OF_OPCODES(0x20u, 0x25u, and);
    //RESERVED TO SEG=ES PREFIX
    ASSIGN_NULL(0x26u);
    ASSIGN_OPCODE(0x27u, daa);
    GROUP_OF_OPCODES(0x28u, 0x2du, sub);
    //RESERVED TO SEG=CS PREFIX
    ASSIGN_NULL(0x2eu);
    ASSIGN_OPCODE(0x2fu, das);
    GROUP_OF_OPCODES(0x30u, 0x35u, xor);
    //RESERVED TO SEG=SS PREFIX
    ASSIGN_NULL(0x36u);
    ASSIGN_OPCODE(0x37u, aaa);
    GROUP_OF_OPCODES(0x38u, 0x3du, cmp);
    //RESERVED TO SEG=DS PREFIX
    ASSIGN_NULL(0x3eu);
    ASSIGN_OPCODE(0x3fu, aas);
    GROUP_OF_OPCODES(0x40u, 0x47u, inc);
    GROUP_OF_OPCODES(0x48u, 0x4f, dec);

}

v8086* v8086_create_machine()
{
    v8086* machine = (v8086*) heap_kernel_alloc(sizeof(v8086), 0);
    if(machine == NULL) return NULL;
    memset(machine, 0, sizeof(v8086));
    machine->regs.x.flags = 0x2;
    machine->sregs.cs = 0xf000;
    machine->IP.w.ip = 0xfff0;
	memcpy(machine->Memory, (void*)0xc0000000, 0x100000);
    v8086_set_8086_instruction_set(machine);
    return machine;
}

int16_t v8086_call_function(v8086* machine)
{
    while(!(machine->IP.w.ip == 0xFFFF && machine->sregs.cs == 0xFFFF))
    {
        int16_t status = parse_and_execute_instruction(machine);
        if(status != OK) return status;
    }
    return OK;
}

int16_t v8086_call_int(v8086* machine, int16_t num)
{
    if ((num < 0) || (num > 0xFF)) return BAD_INT_NUMBER;
    machine -> IP.w.ip = read_word_from_pointer(machine->Memory, get_absolute_address(0, num * 4));
    machine -> sregs.cs = read_word_from_pointer(machine->Memory, get_absolute_address(0, num * 4 + 2));
    push_word(machine, machine->regs.w.flags);
    push_word(machine, 0xFFFF);
    push_word(machine, 0xFFFF);
    int16_t x = v8086_call_function(machine);
    if(x != OK) return x;
    return num;
}

int16_t parse_and_execute_instruction(v8086* machine)
{
    machine->internal_state.IPOffset = 0;
    machine->internal_state.operand_32_bit = 0;
    machine->internal_state.segment_reg_select = DEFAULT;
    machine->internal_state.rep_prefix = NONE;

    int16_t status = OK;

    //Maybe opcode, an be also prefix
    uint8_t opcode;
    decode: opcode = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    uint32_t temp = get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset);
    uint8_t* ptr_to_opcode = get_byte_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    
    //PREFIXES
    //Segment Prefix CS DS ES SS
    if((opcode & 0x7u) == 0x6 && ((opcode >> 5u) & 0x7u) == 0x1u) //001XX110 pattern where XX is number of segment
    {
        machine->internal_state.segment_reg_select = (opcode >> 3u) & 0x3u;
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
    //GROUP 1
    else if(opcode >= 0x80 && opcode <= 0x83)
    {
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t recalculated_opcode = opcode - 0x80;
        switch(get_reg(mod_rm))
        {
            case 0: //ADD
                status = perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm, 0,
                                                                perform_adding);
                break;
            case 1: //OR
                status = perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm, 0, perform_or);
                break;
            case 2: //ADC
                status = perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm,
                                                                bit_get(machine->regs.w.flags, 1u << CARRY_FLAG_BIT) !=
                                                                0, perform_adding);
                break;
            case 3: //SBB
                status = perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm,
                                                                bit_get(machine->regs.w.flags, 1u << CARRY_FLAG_BIT) !=
                                                                0, perform_subtracting);
                break;
            case 4: //AND
                status = perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm, 0, perform_and);
                break;
            case 5: //SUB
                status = perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm, 0,
                                                                perform_subtracting);
                break;
            case 6: //XOR
                status = perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm, 0, perform_xor);
                break;
            case 7: //CMP
                status = perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm, 0, perform_cmp);
                break;
        }
    }
    //GROUP 3
    else if(opcode >= 0xf6 && opcode <= 0xf7)
    {
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t width = 8;
        if(opcode == 0xf7)
        {
            if(machine->internal_state.operand_32_bit) width = 32;
            else width = 16;
        }

        void* dest = get_memory_from_mode(machine, mod_rm, width);

        if(dest == NULL) return UNABLE_GET_MEMORY;

        switch(get_reg(mod_rm))
        {
            case 0: //TEST
            {
                uint32_t immediate;
                if(width == 8)
                {
                    immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                    machine -> internal_state.IPOffset += 1;
                }
                else if(width == 16)
                {
                    immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                    machine -> internal_state.IPOffset += 2;
                }
                else if(width == 32)
                {
                    immediate = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                    machine -> internal_state.IPOffset += 4;
                }
                status = perform_test(machine, &immediate, dest, width);
                break;
            }
            case 2: //NOT
                if(width == 8)
                    *((uint8_t*)dest) = ~(*((uint8_t*)dest));
                else if(width == 16)
                    *((uint16_t*)dest) = ~(*((uint16_t*)dest));
                else if(width == 32)
                    *((uint32_t*)dest) = ~(*((uint32_t*)dest));
                break;
            case 3: //NEG
                status = perform_neg(machine, dest, width);
                break;
            case 4: //MUL
                status = perform_multiplication(machine, dest, 0, width);
                break;
            case 5: //IMUL
                status = perform_multiplication(machine, dest, 1, width);
                break;
            case 6: //DIV
                status = perform_division(machine, dest, 0, width);
                break;
            case 7:
                status = perform_division(machine, dest, 1, width);
                break;
            default:
                return BAD_REG;
        }
    }
    //PUSH Operations
    //PUSH General purpose registers
    else if(opcode >= 0x50 && opcode <= 0x57)
    {
        uint8_t width = 16;
        void* reg = NULL;
        if(machine->internal_state.operand_32_bit) width = 16;
        reg = get_variable_length_register(machine, opcode & 7u, width);
        if(reg == NULL) return UNDEFINED_REGISTER;
        if(width==16) push_word(machine, *((uint16_t*)reg));
        else if(width==32) push_dword(machine, *((uint32_t*)reg));
        else return BAD_WIDTH;
    }
    //PUSH FLAGS
    else if(opcode == 0x9c)
        push_word(machine, machine->regs.w.flags);
    //POP Operations
    //POP General purpose registers
    else if(opcode >= 0x50 && opcode <= 0x57)
    {
        uint8_t width = 16;
        void* reg = NULL;
        if(machine->internal_state.operand_32_bit) width = 16;
        reg = get_variable_length_register(machine, opcode & 7u, width);
        if(reg == NULL) return UNDEFINED_REGISTER;
        if(width==16) *((uint16_t*)reg) = pop_word(machine);
        else if(width==32) *((uint32_t*)reg) = pop_dword(machine);
        else return BAD_WIDTH;
    }
    //POP FLAGS
    else if(opcode == 0x9d)
        machine->regs.w.flags = pop_word(machine);
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
            uint16_t* regB = get_word_register(machine, opcode & 7u);
            if((regA == NULL) || (regB == NULL)) return UNDEFINED_REGISTER;
            temp = *regA;
            *regA = *regB;
            *regB = temp;
        }
        else if(width == 32)
        {
            uint32_t temp;
            uint32_t* regA = get_dword_register(machine, EAX); 
            uint32_t* regB = get_dword_register(machine, opcode & 7u);
            if((regA == NULL) || (regB == NULL)) return UNDEFINED_REGISTER;
            temp = *regA;
            *regA = *regB;
            *regB = temp;
        }
        else return -1;
    }
    //XCHG r8, rm8 or XCHG r16, rm16 or XCHG r32, rm32
    else if(opcode >= 0x86 && opcode <= 0x87)
    {
        int8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        if(opcode == 0x86)
        {
            uint8_t* source = get_byte_register(machine, get_reg(mod_rm));
            uint8_t* dest = get_memory_from_mode(machine, mod_rm, 8);
            if(source == NULL) return UNDEFINED_REGISTER;
            if(dest == NULL) return UNABLE_GET_MEMORY;
            uint8_t temp;
            temp = *source;
            *source = *dest;
            *dest = temp;
        }
        else if(opcode  == 0x87)
        {
            if(machine->internal_state.operand_32_bit)
            {
                uint32_t* source = get_dword_register(machine, get_reg(mod_rm));
                uint32_t* dest = get_memory_from_mode(machine, mod_rm, 32);
                if(source == NULL) return UNDEFINED_REGISTER;
                if(dest == NULL) return UNABLE_GET_MEMORY;
                uint32_t temp;
                temp = *source;
                *source = *dest;
                *dest = temp;
            }
            else
            {
                uint16_t* source = get_word_register(machine, get_reg(mod_rm));
                uint16_t* dest = get_memory_from_mode(machine, mod_rm, 16);
                if(source == NULL) return UNDEFINED_REGISTER;
                if(dest == NULL) return UNABLE_GET_MEMORY;
                uint16_t temp;
                temp = *source;
                *source = *dest;
                *dest = temp;
            }
        }
    }
    //ROLS and RORS Group (Group 2)
    else if(opcode >= 0xd0 && opcode <= 0xd3)
    {
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t arg = opcode <=0xd1 ? 1 : machine->regs.h.cl;
        uint8_t width = 8;
        if(opcode % 2){
            if(machine->internal_state.operand_32_bit) width = 32;
            else width = 16;
        }
        void* dest = get_memory_from_mode(machine, mod_rm, width);
        if(dest == NULL) return UNABLE_GET_MEMORY;
        switch(get_reg(mod_rm))
        {
            case 0:
                status = perform_rol(machine, dest, arg, width);
                break;
            case 1:
                status = perform_ror(machine, dest, arg, width);
                break;
            case 2:
                status = perform_rcl(machine, dest, arg, width);
                break;
            case 3:
                status = perform_rcr(machine, dest, arg, width);
                break;
            case 4:
                status = perform_shl(machine, dest, arg, width);
                break;
            case 5:
                status = perform_shr(machine, dest, arg, width);
                break;
            case 6:
                return UNDEFINED_OPCODE;
            case 7:
                status = perform_sar(machine, dest, arg, width);
                break;
        }
    }
    //Jumps Group
    //SHORT JUMPS on conditions
    else if((opcode >= 0x70 && opcode <= 0x7f) || (opcode == 0xe3))
    {
        int8_t offset = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint32_t tempIP = machine->IP.w.ip;
        uint8_t jump = 0;
        if(opcode != 0xe3)
            switch(opcode & 0x0fu)
            {
                case 0: //JO
                    if(bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) jump = 1;
                    break;
                case 1: //JNO
                    if(!bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) jump = 1;
                    break;
                case 2: //JB or JNAE or JC
                    if(bit_get(machine->regs.x.flags, 1u <<CARRY_FLAG_BIT)) jump = 1;
                    break;
                case 3: //JNB or JAE or JNC
                    if(!bit_get(machine->regs.x.flags, 1u <<CARRY_FLAG_BIT)) jump = 1;
                    break;
                case 4: //JZ or JE
                    if(bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT)) jump = 1;
                    break;
                case 5: //JNZ or JNE
                    if(!bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT)) jump = 1;
                    break;
                case 6: //JBE or JNA
                    if(bit_get(machine->regs.x.flags, 1u <<CARRY_FLAG_BIT) || bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT)) jump = 1;
                    break;
                case 7: //JNBE or JA
                    if(!(bit_get(machine->regs.x.flags, 1u <<CARRY_FLAG_BIT) || bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT))) jump = 1;
                    break;
                case 8: //JS
                    if(bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT)) jump = 1;
                    break;
                case 9: //JNS
                    if(!bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT)) jump = 1;
                    break;
                case 0xa: //JP or JPE
                    if(bit_get(machine->regs.x.flags, 1u <<PARITY_FLAG_BIT)) jump = 1;
                    break;
                case 0xb: //JNP or JPO
                    if(!bit_get(machine->regs.x.flags, 1u <<PARITY_FLAG_BIT)) jump = 1;
                    break;
                case 0xc: //JL or JNGE
                    if(!bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT) != !bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) jump = 1;
                    break; 
                case 0xd: //JNL or JGE
                    if(!bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT) == !bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) jump = 1;
                    break; 
                case 0xe: //JLE or JNG
                    if(bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT) || !bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT) != !bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) jump = 1;
                    break;
                case 0xf: //JNLE or JG
                    if(bit_get(machine->regs.x.flags, 1u <<ZERO_FLAG_BIT) || !bit_get(machine->regs.x.flags, 1u <<SIGN_FLAG_BIT) == !bit_get(machine->regs.x.flags, 1u <<OVERFLOW_FLAG_BIT)) jump = 1;
                    break;
            }
        else
        {
            if(machine->internal_state.operand_32_bit)
            {
                if(!machine->regs.d.ecx) jump = 1;
            }
            else
                if(!machine->regs.w.cx) jump = 1;
        }
        if(jump) tempIP += offset;
        if(tempIP > 0xFFFF) return -1;
        machine->IP.w.ip = tempIP;
    }
    //Short relative JMP
    else if(opcode == 0xeb)
    {
        int8_t offset = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        machine->IP.w.ip += offset;
    }
    //Near realtive JMP
    else if(opcode == 0xe9)
    {
        int16_t offset = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        machine->IP.w.ip += offset;
    }
    //Far JMP
    else if(opcode == 0xea)
    {
        int16_t newIP = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        int16_t newCS = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->sregs.cs = newCS;
        machine->IP.w.ip = newIP;
        machine->internal_state.IPOffset = 0;
    }
    //LOOP Group
    //LOOP LOOPE LOOPNE
    else if(opcode >= 0xe0 && opcode <= 0xe2)
    {
        uint8_t jump = 0;
        int8_t offset = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;

        if(machine->internal_state.operand_32_bit) machine->regs.d.ecx--;
        else machine->regs.w.cx--;
            
        switch (opcode)
        {
        case 0xe0:
            if(machine->internal_state.operand_32_bit){
                if(machine->regs.d.ecx && !bit_get(machine->regs.w.flags, 1u << ZERO_FLAG_BIT)) jump = 1;
            }
            else
                if(machine->regs.w.cx && !bit_get(machine->regs.w.flags, 1u << ZERO_FLAG_BIT)) jump = 1;
            break;
        case 0xe1:
            if(machine->internal_state.operand_32_bit){
                if(machine->regs.d.ecx && bit_get(machine->regs.w.flags, 1u << ZERO_FLAG_BIT)) jump = 1;
            }
            else
                if(machine->regs.w.cx && bit_get(machine->regs.w.flags, 1u << ZERO_FLAG_BIT)) jump = 1;
            break;
        case 0xe2:
            if(machine->internal_state.operand_32_bit){
                if(machine->regs.d.ecx) jump = 1;
            }
            else
                if(machine->regs.w.cx) jump = 1;
            break;
        default:
             return UNKNOWN_ERROR;
        }

        if(jump)
            machine->IP.w.ip += offset;
    }
    //MOV Group
    //MOV r8, imm8
    else if(opcode >= 0xb0 && opcode <= 0xb7)
    {
        uint8_t* reg = get_byte_register(machine, opcode & 0x7u);
        uint8_t imm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        if(reg == NULL) return UNDEFINED_REGISTER;
        machine->internal_state.IPOffset += 1;
        *reg = imm;
    }
    //MOV r16, imm16 or r32, imm32
    else if(opcode >= 0xb8 && opcode <= 0xbf)
    {
        if(machine->internal_state.operand_32_bit)
        {
            uint32_t* reg = get_dword_register(machine, (opcode - 0xb8u) & 0x7u);
            if(reg == NULL) return UNDEFINED_REGISTER;
            uint32_t imm = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 4;
            *reg = imm;
        }
        else
        {
            uint16_t* reg = get_word_register(machine, (opcode - 0xb8u) & 0x7u);
            if(reg == NULL) return UNDEFINED_REGISTER;
            uint16_t imm = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 2;
            *reg = imm; 
        }
    }
    //MOV AL/AX/EAX, moffs8/moffs16/moffs32 or MOV moffs8/moffs16/moffs32, AL/AX/EAX
    else if(opcode >= 0xa0 && opcode <= 0xa3)
    {
        uint16_t offset = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 2;
        uint16_t* segment = machine->internal_state.segment_reg_select == DEFAULT ? select_segment_register(machine, DS) : select_segment_register(machine, machine->internal_state.segment_reg_select);
        if(segment == NULL) return UNDEFINED_SEGMENT_REGISTER;
        switch (opcode)
        {
        case 0xa0:
            machine->regs.h.al = read_byte_from_pointer(machine->Memory, get_absolute_address(*segment, offset));
            break;
        case 0xa1:
            if(machine->internal_state.operand_32_bit) machine->regs.d.eax = read_dword_from_pointer(machine->Memory, get_absolute_address(*segment, offset));
            else machine->regs.x.ax = read_word_from_pointer(machine->Memory, get_absolute_address(*segment, offset));
            break;
        case 0xa2:
            write_byte_to_pointer(machine->Memory, get_absolute_address(*segment, offset), machine->regs.h.al);
            break;
        case 0xa3:
            if(machine->internal_state.operand_32_bit) write_dword_to_pointer(machine->Memory, get_absolute_address(*segment, offset), machine->regs.d.eax);
            else write_word_to_pointer(machine->Memory, get_absolute_address(*segment, offset), machine->regs.x.ax);
            break;
        default:
            return UNKNOWN_ERROR;
        }
    }
    //MOV r8/r16/r32, rm8/rm16/rm32 or MOV rm8/rm16/rm32, r8/r16/r32
    else if(opcode >= 0x88 && opcode <= 0x8b)
    {
        //Mod/RM
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        void* source = NULL;
        void* dest = NULL;
        uint8_t width = machine->internal_state.operand_32_bit ? 32 : 16;
        switch (opcode)
        {
        case 0x88:
            source = get_byte_register(machine, get_reg(mod_rm));
            dest = get_memory_from_mode(machine, mod_rm, 8);
            if(source == NULL) return UNDEFINED_REGISTER;
            if(dest == NULL) return UNABLE_GET_MEMORY;
            *((uint8_t*)dest) = *((uint8_t*) source);
            break;
        case 0x89:
            source = get_variable_length_register(machine, get_reg(mod_rm), width);
            dest = get_memory_from_mode(machine, mod_rm, width);
            if(source == NULL) return UNDEFINED_REGISTER;
            if(dest == NULL) return UNABLE_GET_MEMORY;
            if(width == 16) *((uint16_t*)dest) = *((uint16_t*) source);
            else *((uint32_t*)dest) = *((uint32_t*) source);
            break;
        case 0x8a:
            dest = get_byte_register(machine, get_reg(mod_rm));
            source = get_memory_from_mode(machine, mod_rm, 8);
            if(dest == NULL) return UNDEFINED_REGISTER;
            if(source == NULL) return UNABLE_GET_MEMORY;
            *((uint8_t*)dest) = *((uint8_t*) source);
            break;
        case 0x8b:
            dest = get_variable_length_register(machine, get_reg(mod_rm), width);
            source = get_memory_from_mode(machine, mod_rm, width);
            if(dest == NULL) return UNDEFINED_REGISTER;
            if(source == NULL) return UNABLE_GET_MEMORY;
            if(width == 16) *((uint16_t*)dest) = *((uint16_t*) source);
            else *((uint32_t*)dest) = *((uint32_t*) source);
            break;
        default:
            return UNKNOWN_ERROR;
        }
    }
    //MOV Segment to/from r/m
    else if(opcode == 0x8c || opcode == 0x8e)
    {
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint16_t* source = NULL;
        uint16_t* dest = NULL;
        if(opcode == 0x8c)
        {
            source = select_segment_register(machine, get_reg(mod_rm));
            dest = get_memory_from_mode(machine, mod_rm, 16);
            if(source == NULL) return UNDEFINED_SEGMENT_REGISTER;
            if(dest == NULL) return UNABLE_GET_MEMORY;
        }
        else
        {
            dest = select_segment_register(machine, get_reg(mod_rm));
            source = get_memory_from_mode(machine, mod_rm, 16);
            if(dest == NULL) return UNDEFINED_SEGMENT_REGISTER;
            if(source == NULL) return UNABLE_GET_MEMORY;
        }
        *dest = *source;
    }
    //MOV rm, imm
    else if(opcode >= 0xc6 && opcode <= 0xc7)
    {
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        if(opcode == 0xc6)
        {
            uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 1;
            uint8_t* mem = get_memory_from_mode(machine, mod_rm, 8);
            if(mem == NULL) return UNABLE_GET_MEMORY;
            *mem = immediate;
        }
        else
        {
            if(machine->internal_state.operand_32_bit)
            {
                uint32_t immediate = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 4;
                uint32_t* mem = get_memory_from_mode(machine, mod_rm, 32);
                if(mem == NULL) return UNABLE_GET_MEMORY;
                *mem = immediate;
            }   
            else
            {
                uint16_t immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 2;
                uint16_t* mem = get_memory_from_mode(machine, mod_rm, 16);
                if(mem == NULL) return UNABLE_GET_MEMORY;
                *mem = immediate;
            }
        }
    }
    //TEST GROUP
    else if(opcode == 0x84)
    {
        //Mod/RM
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t* reg = get_byte_register(machine, get_reg(mod_rm));
        uint8_t* memory = get_memory_from_mode(machine, mod_rm, 8);
        if(reg == NULL) return UNDEFINED_REGISTER;
        if(memory == NULL) return UNABLE_GET_MEMORY;
        status = perform_test(machine, reg, memory, 8);
    }
    else if(opcode == 0x85)
    {
        //Mod/RM
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t width = 16;
        if(machine->internal_state.operand_32_bit) width = 32;
        void* source = get_variable_length_register(machine, get_reg(mod_rm), width);
        void* dest = get_memory_from_mode(machine, mod_rm, width);
        if(source == NULL) return UNDEFINED_REGISTER;
        if(dest == NULL) return UNABLE_GET_MEMORY;
        status = perform_test(machine, source, dest, width);
    }
    else if(opcode == 0xa8)
    {
        //Mod/RM
        uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t* reg = get_byte_register(machine, AL);
        if(reg == NULL) return UNDEFINED_REGISTER;
        uint8_t result = *reg & immediate;
        bit_clear(machine->regs.d.eflags, 1u <<CARRY_FLAG_BIT);
        bit_clear(machine->regs.d.eflags, 1u <<OVERFLOW_FLAG_BIT);
        uint8_t parrity = result & 1u;
        for(uint8_t i = 1; i < 8; i++) parrity ^= (uint8_t)(result >> i) & 1u;
        bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
        bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
        bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, result >> 7u); //SIGN FLAG
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
            immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 2;
        }
        else if(width == 32)
        {
            immediate = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 4;
        }
        else return BAD_WIDTH;

        void* reg = get_variable_length_register(machine, AX, width);
        if(reg == NULL) return UNDEFINED_REGISTER;
        
        if(width == 16)
            result = *((uint16_t*) reg) & immediate;
        else if(width == 32)
            result = *((uint32_t*) reg) & immediate;

        uint8_t parrity = result & 1u;
        for(uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
        bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
        bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
        bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, result >> (width - 1u)); //SIGN FLAG
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
        if(source_segment == NULL) return UNDEFINED_SEGMENT_REGISTER;
        if(dest_segment == NULL) return UNDEFINED_SEGMENT_REGISTER;
        //if repeat and number of repats == 0 -> dont copy anything
        if(machine->internal_state.rep_prefix == REP_REPE && machine->regs.w.cx == 0) goto recalculate_ip; 

        do{
            source = get_byte_pointer(machine->Memory, get_absolute_address(*source_segment, machine->regs.w.si));
            dest = get_byte_pointer(machine->Memory, get_absolute_address(*dest_segment, machine->regs.w.di));
            *dest = *source;
            machine->regs.w.si += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -1 : 1;
            machine->regs.w.di += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -1 : 1;
        } while(machine->internal_state.rep_prefix == REP_REPE && --(machine->regs.w.cx));
    }
    //MOVSW or MOVSD
    else if(opcode == 0xA5)
    {
        uint16_t* source_segment;
        uint16_t* dest_segment = select_segment_register(machine, ES);
        if(machine->internal_state.segment_reg_select == DEFAULT) source_segment = select_segment_register(machine, DS);
        else source_segment = select_segment_register(machine, machine->internal_state.segment_reg_select);
        if(source_segment == NULL) return UNDEFINED_SEGMENT_REGISTER;
        if(dest_segment == NULL) return UNDEFINED_SEGMENT_REGISTER;
        //if repeat and number of repats == 0 -> dont copy anything
        if(machine->internal_state.rep_prefix == REP_REPE && machine->regs.w.cx == 0) goto recalculate_ip; 

        if(machine->internal_state.operand_32_bit)
            do{
                uint32_t* source = get_dword_pointer(machine->Memory, get_absolute_address(*source_segment, machine->regs.w.si));
                uint32_t* dest = get_dword_pointer(machine->Memory, get_absolute_address(*dest_segment, machine->regs.w.di));
                *dest = *source;
                machine->regs.w.si += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -4 : 4;
                machine->regs.w.di += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -4 : 4;
            } while(machine->internal_state.rep_prefix == REP_REPE && --(machine->regs.w.cx));
        else
            do{
                uint16_t* source = get_word_pointer(machine->Memory, get_absolute_address(*source_segment, machine->regs.w.si));
                uint16_t* dest = get_word_pointer(machine->Memory, get_absolute_address(*dest_segment, machine->regs.w.di));
                *dest = *source;
                machine->regs.w.si += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -2 : 2;
                machine->regs.w.di += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -2 : 2;
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
        if(opcode == 0xA7){
            if(machine->internal_state.operand_32_bit) width=32;
            else width = 16;
        }
        
        if(machine->internal_state.segment_reg_select == DEFAULT) source_segment = select_segment_register(machine, DS);
        else source_segment = select_segment_register(machine, machine->internal_state.segment_reg_select);
        if(source_segment == NULL) return UNDEFINED_SEGMENT_REGISTER;
        if(dest_segment == NULL) return UNDEFINED_SEGMENT_REGISTER;
        //if repeat and number of repats == 0 -> dont copy anything
        if(machine->internal_state.rep_prefix != NONE && machine->regs.w.cx == 0) goto recalculate_ip; 

        do{
            source = get_variable_length_pointer(machine->Memory, get_absolute_address(*source_segment, machine->regs.w.si), width);
            dest = get_variable_length_pointer(machine->Memory, get_absolute_address(*dest_segment, machine->regs.w.di), width);
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
            bit_write(machine->regs.d.eflags, 1u <<CARRY_FLAG_BIT, (result >> width) ? 1 : 0); // CARRY FLAG
            bit_write(machine->regs.d.eflags, 1u <<AUX_CARRY_FLAG_BIT, (((dest_before & 0xfu) - (source_before & 0xfu)) >> 4u) ? 1: 0); //AUX CARRY FLAG
            uint8_t parrity = result & 1u;
            for(uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
            bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
            bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
            bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, result >> (width - 1u)); //SIGN FLAG
            bit_write(machine->regs.d.eflags, 1u <<OVERFLOW_FLAG_BIT, ((result >> (width - 1u)) != (dest_before >> (width - 1u)))); //OVERFLOW FLAG

            machine->regs.w.si += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -(width/8) : (width/8);
            machine->regs.w.di += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -(width/8) : (width/8);

            if(machine->internal_state.rep_prefix == REP_REPE && !bit_get(machine->regs.w.flags, ZERO_FLAG_BIT)) break;
            else if(machine->internal_state.rep_prefix == REPNE && bit_get(machine->regs.w.flags, ZERO_FLAG_BIT)) break;
        } while(machine->internal_state.rep_prefix != NONE && --(machine->regs.w.cx));
    }
    //STOSB STOSW STOSD
    else if(opcode >= 0xAA && opcode <= 0xAB)
    {
        uint16_t* segment = select_segment_register(machine, ES);
        void* source;
        void* dest;
        uint8_t width = 8;
        if(opcode == 0xA7)
        {
            if(machine->internal_state.operand_32_bit) width=32;
            else width = 16;
        }

        if(segment == NULL) return UNDEFINED_SEGMENT_REGISTER;
        //if repeat and number of repats == 0 -> dont copy anything
        if(machine->internal_state.rep_prefix == REP_REPE && machine->regs.w.cx == 0) goto recalculate_ip; 

        do{
            dest = get_variable_length_pointer(machine->Memory, get_absolute_address(*segment, machine->regs.w.di), width);
            source = get_variable_length_register(machine, AL, width);
            if(source == NULL) return UNDEFINED_REGISTER;
            if(width == 8) *((uint8_t*) dest) = *((uint8_t*) source);
            else if(width == 16) *((uint16_t*) dest) = *((uint16_t*) source);
            else if(width == 32) *((uint32_t*) dest) = *((uint32_t*) source);
            else return -1;

            machine->regs.w.di += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -(width/8) : (width/8);
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
        {
            if(machine->internal_state.operand_32_bit) width=32;
            else width = 16;
        }

        if(machine->internal_state.segment_reg_select == DEFAULT) segment = select_segment_register(machine, DS);
        else segment = select_segment_register(machine, machine->internal_state.segment_reg_select);
        if(segment == NULL) return UNDEFINED_SEGMENT_REGISTER;
        //if repeat and number of repats == 0 -> dont copy anything
        if(machine->internal_state.rep_prefix == REP_REPE && machine->regs.w.cx == 0) goto recalculate_ip; 

        do{
            source = get_variable_length_pointer(machine->Memory, get_absolute_address(*segment, machine->regs.w.si), width);
            dest = get_variable_length_register(machine, AL, width);
            if(dest == NULL) return UNDEFINED_REGISTER;
            if(width == 8) *((uint8_t*) dest) = *((uint8_t*) source);
            else if(width == 16) *((uint16_t*) dest) = *((uint16_t*) source);
            else if(width == 32) *((uint32_t*) dest) = *((uint32_t*) source);
            else return -1;

            machine->regs.w.si += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -(width/8) : (width/8);
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
        {
            if(machine->internal_state.operand_32_bit) width=32;
            else width = 16;
        }
        if(source_segment == NULL) return UNDEFINED_SEGMENT_REGISTER;
        //if repeat and number of repats == 0 -> dont copy anything
        if(machine->internal_state.rep_prefix != NONE && machine->regs.w.cx == 0) goto recalculate_ip; 

        do{
            dest = get_variable_length_register(machine, AX, width);
            source = get_variable_length_pointer(machine->Memory, get_absolute_address(*source_segment, machine->regs.w.di), width);
            if(dest == NULL) return UNDEFINED_REGISTER;
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
            bit_write(machine->regs.d.eflags, 1u <<CARRY_FLAG_BIT, (result >> width) ? 1 : 0); // CARRY FLAG
            bit_write(machine->regs.d.eflags, 1u <<AUX_CARRY_FLAG_BIT, (((dest_before & 0xfu) - (source_before & 0xfu)) >> 4u) ? 1: 0); //AUX CARRY FLAG
            uint8_t parrity = result & 1u;
            for(uint8_t i = 1; i < 8; i++) parrity ^= (result >> i) & 1u;
            bit_write(machine->regs.d.eflags, 1u <<PARITY_FLAG_BIT, (parrity) ? 1: 0); //PARRITY FLAG
            bit_write(machine-> regs.d.eflags, 1u <<ZERO_FLAG_BIT, result == 0); //ZERO FLAG
            bit_write(machine->regs.d.eflags, 1u <<SIGN_FLAG_BIT, result >> (width - 1u)); //SIGN FLAG
            bit_write(machine->regs.d.eflags, 1u <<OVERFLOW_FLAG_BIT, ((result >> (width - 1u)) != (dest_before >> (width - 1u)))); //OVERFLOW FLAG

            machine->regs.w.di += bit_get(machine->regs.w.flags, 1u << DIRECTION_FLAG_BIT) ? -(width/8) : (width/8);

            if(machine->internal_state.rep_prefix == REP_REPE && !bit_get(machine->regs.w.flags, ZERO_FLAG_BIT)) break;
            else if(machine->internal_state.rep_prefix == REPNE && bit_get(machine->regs.w.flags, ZERO_FLAG_BIT)) break;
        } while(machine->internal_state.rep_prefix != NONE && --(machine->regs.w.cx));
    }
    //LOAD Operations group
    //LEA
    else if(opcode == 0x8d)
    {
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;

        uint16_t segment;
        uint32_t offset;

        if((mod_rm >> 6u) > 3) return -1;

        int16_t r = calculate_segment_offset_from_mode(machine, mod_rm, &segment, &offset);
        if(r) return r;
        
        if(machine->internal_state.operand_32_bit)
        {
            uint32_t* reg = get_dword_register(machine, get_reg(mod_rm));
            *reg = offset;
        }
        else
        {
            uint16_t* reg = get_word_register(machine, get_reg(mod_rm));
            *reg = offset;
        }
    }
    //LDS or LES
    else if(opcode >= 0xc4 && opcode <= 0xc5)
    {
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;

        uint16_t* segment_register;
        if(opcode == 0xc4) segment_register = select_segment_register(machine, ES);
        else segment_register = select_segment_register(machine, DS);
        if(segment_register == NULL) return UNDEFINED_SEGMENT_REGISTER;
        uint16_t* source = get_memory_from_mode(machine, mod_rm, 16);
        if(source == NULL) return UNABLE_GET_MEMORY;

        if(machine->internal_state.operand_32_bit)
        {
            uint32_t* dest = get_dword_register(machine, get_reg(mod_rm));
            *dest = *((uint32_t*) source);
            *segment_register = *(source+2);
        }
        else
        {
            uint16_t* dest = get_word_register(machine, get_reg(mod_rm));
            *dest = *source;
            *segment_register = *(source+1);
        }
        
    }
    //CONVERSIONS group
    //CBW or CWDE
    else if(opcode == 0x98)
    {
        if(machine->internal_state.operand_32_bit)
            machine->regs.d.eax = ((int32_t)(machine->regs.w.ax));
        else
            machine->regs.w.ax = ((int16_t)(machine->regs.h.al));
    }
    //CWD or CDQ
    else if(opcode == 0x99)
    {
        if(machine->internal_state.operand_32_bit){
            int64_t t = machine->regs.d.eax;
            machine->regs.d.edx = ((uint64_t)t >> 32u);
        }
        else{
            int32_t t = machine->regs.w.ax;
            machine->regs.w.dx = ((uint32_t)t >> 16u);
        }
    }
    //Store and load flags group
    //SAHF
    else if(opcode == 0x9e){
        for(uint8_t i = 0; i < 8; i++)
            if(i != 1 && i != 3 && i != 5)
                bit_write(machine->regs.w.flags, 1u <<i, bit_get(machine->regs.h.ah, 1u <<i));
    }
    //LAHF
    else if(opcode == 0x9f)
        machine->regs.h.ah = machine->regs.w.flags & 0xFFu;
    //FLAG Setting and clearing Group
    //CMC
    else if(opcode == 0xf5)
        bit_flip(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT);
    //CLC
    else if(opcode == 0xf8)
        bit_clear(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT);
    //STC
    else if(opcode == 0xf9)
        bit_set(machine->regs.w.flags, 1u <<CARRY_FLAG_BIT);
    //CLI
    else if(opcode == 0xfa)
        bit_clear(machine->regs.w.flags, 1u <<INTERRUPT_FLAG_BIT);
    //STI
    else if(opcode == 0xfb)
        bit_set(machine->regs.w.flags, 1u <<INTERRUPT_FLAG_BIT);
    //CLD
    else if(opcode == 0xfc)
        bit_clear(machine->regs.w.flags, 1u <<DIRECTION_FLAG_BIT);
    //STD
    else if(opcode == 0xfd)
        bit_set(machine->regs.w.flags, 1u <<DIRECTION_FLAG_BIT);
    //INPUT OUTPUT GROUP
    //IN AL, i8
    else if(opcode == 0xe4)
    {
        uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        machine->regs.h.al = io_in_byte(immediate);        
    }
    //IN AX/EAX, i8
    else if(opcode == 0xe5)
    {
        uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        if(machine->internal_state.operand_32_bit)
            machine->regs.d.eax = io_in_long(immediate);
        else
            machine->regs.w.ax = io_in_word(immediate);
    }
    //IN AL, DX
    else if(opcode == 0xec)
        machine->regs.h.al = io_in_byte(machine->regs.x.dx);
    //IN AX/EAX, DX
    else if(opcode == 0xed)
    {
        if(machine->internal_state.operand_32_bit)
            machine->regs.d.eax = io_in_long(machine->regs.x.dx);
        else
            machine->regs.w.ax = io_in_word(machine->regs.x.dx);
    }
    //OUT i8, AL
    else if(opcode == 0xe6)
    {
        uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        io_out_byte(immediate, machine->regs.h.al);
    }
    //OUT i8, AX/EAX
    else if(opcode == 0xe7)
    {
        uint8_t immediate = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        if(machine->internal_state.operand_32_bit)
            io_out_long(immediate, machine->regs.d.eax);
        else
            io_out_long(immediate, machine->regs.w.ax);
    }
    //OUT DX, AL
    else if(opcode == 0xee)
        io_out_byte(machine->regs.w.dx, machine->regs.h.al);
    //OUT DX, AX/EAX
    else if(opcode == 0xef)
    {
        if(machine->internal_state.operand_32_bit)
            io_out_long(machine->regs.w.dx, machine->regs.d.eax);
        else
            io_out_long(machine->regs.w.dx, machine->regs.w.ax);
    }
    //CALLs and RETs group
    //NEAR relative CALL
    else if(opcode == 0xe8)
    {
        int16_t immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 2;

        machine->IP.w.ip += machine->internal_state.IPOffset;
        push_word(machine, machine->IP.w.ip);
        machine->IP.w.ip += immediate;
        machine->internal_state.IPOffset = 0;
    }
    //FAR CALL
    else if(opcode == 0x9a)
    {
        int16_t newIP = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 2;
        int16_t newCS = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 2;
        push_word(machine, machine->sregs.cs);
        push_word(machine, machine->IP.w.ip + machine->internal_state.IPOffset);
        machine->IP.w.ip = newIP;
        machine->sregs.cs = newCS;
        machine->internal_state.IPOffset = 0;
    }
    //Near RET
    else if(opcode == 0xc3)
    {
        machine->IP.w.ip = pop_word(machine);
        machine->internal_state.IPOffset = 0;
    }
    //Near RET, imm16
    else if(opcode == 0xc2)
    {
        uint16_t immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 2;
        machine->IP.w.ip = pop_word(machine);
        machine->regs.w.sp += immediate;
        machine->internal_state.IPOffset = 0;
    }
    //Far RET
    else if(opcode == 0xcb)
    {
        machine->IP.w.ip = pop_word(machine);
        machine->sregs.cs = pop_word(machine);
        machine->internal_state.IPOffset = 0;
    }
    //Far RET, imm16
    else if(opcode == 0xca)
    {
        uint16_t immediate = read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 2;
        machine->IP.w.ip = pop_word(machine);
        machine->sregs.cs = pop_word(machine);
        machine->regs.w.sp += immediate;
        machine->internal_state.IPOffset = 0;
    }
    //Interrupts group
    //INT, imm and INT 3 and INTO
    else if(opcode >= 0xcc && opcode <= 0xce)
    {
        uint8_t interrupt_number = 3;
        if(opcode == 0xcd) //INT, imm
        {
            interrupt_number = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 1;
        }
        else if(opcode == 0xce) //INTO
        {
            if(!bit_get(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT)) goto recalculate_ip;
            interrupt_number = 4;
        }

        int16_t newIP = read_word_from_pointer(machine->Memory, get_absolute_address(0, interrupt_number * 4));
        int16_t newCS = read_word_from_pointer(machine->Memory, get_absolute_address(0, interrupt_number * 4 + 2));

        push_word(machine, machine->regs.w.flags);
        push_word(machine, machine->sregs.cs);
        push_word(machine, machine->IP.w.ip);

        machine->IP.w.ip = newIP;
        machine->sregs.cs = newCS;

        machine->internal_state.IPOffset = 0;
    }
    //IRET
    else if(opcode == 0xcf)
    {
        machine->IP.w.ip = pop_word(machine);
        machine->sregs.cs = pop_word(machine);
        machine->regs.w.flags = pop_word(machine);

        machine->internal_state.IPOffset = 0;
    }
    //MISC group
    //XLAT/XLATB
    else if(opcode == 0xd7)
    {
        uint8_t tempAL = machine->regs.h.al;
        uint16_t* segment;
        if(machine->internal_state.segment_reg_select != DEFAULT)
            segment = select_segment_register(machine, machine->internal_state.segment_reg_select);
        else
            segment = select_segment_register(machine, DS);
        if(segment == NULL) return UNDEFINED_SEGMENT_REGISTER;
        machine->regs.h.al = read_byte_from_pointer(machine->Memory, get_absolute_address(*segment, machine->regs.w.bx + tempAL));
    }
    //GROUP 4
    else if(opcode == 0xfe)
    {
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t width = 8;

        void* dest = get_memory_from_mode(machine, mod_rm, width);
        if(dest == NULL) return UNABLE_GET_MEMORY;
        switch(get_reg(mod_rm))
        {
            case 0: //INC rm8
                status = perform_inc(machine, dest, width);
                break;
            case 1: //DEC rm8
                status = perform_dec(machine, dest, width);
                break;
            default:
                return UNDEFINED_OPCODE;
        }
    }
    //GROUP 5
    else if(opcode == 0xff)
    {
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t width = 16;
        if(machine->internal_state.operand_32_bit) width = 32;

        void* dest = get_memory_from_mode(machine, mod_rm, width);
        if(dest == NULL) return UNABLE_GET_MEMORY;
        switch(get_reg(mod_rm))
        {
            case 0: //INC rm8
                status = perform_inc(machine, dest, width);
                break;
            case 1: //DEC rm8
                status = perform_dec(machine, dest, width);
                break;
            case 2: //Near absolute indirect call
                machine->IP.w.ip += machine->internal_state.IPOffset;
                push_word(machine, machine->IP.w.ip);
                if(width == 16)
                    machine->IP.w.ip += *((uint16_t*) dest);
                else return BAD_WIDTH;
                machine->internal_state.IPOffset = 0;
                break;
            case 3: // Far absolute indirect call
                machine->IP.w.ip += machine->internal_state.IPOffset;
                push_word(machine, machine->sregs.cs);
                push_word(machine, machine->IP.w.ip);
                machine->IP.w.ip = *((uint16_t*) dest);
                machine->sregs.cs = *(((uint16_t*)dest)+1);
                machine->internal_state.IPOffset = 0;
                break;
            case 4: //Near absolute indirect jmp
                if(width == 16)
                    machine->IP.w.ip += *((uint16_t*) dest);
                else return BAD_WIDTH;
                machine->internal_state.IPOffset = 0;
                break;
            case 5: //Far absolute indirect jmp
                machine->IP.w.ip = *((uint16_t*) dest);
                machine->sregs.cs = *(((uint16_t*)dest)+1);
                machine->internal_state.IPOffset = 0;
                break;
            case 6:
                if(width == 16) push_word(machine, *((uint16_t*)dest));
                else if(width == 32) push_dword(machine, *((uint32_t*)dest));
                else return BAD_WIDTH;
                break;
            default:
                return UNDEFINED_OPCODE;
        }
    }
    //GROUP 1A
    else if(opcode == 0x8f)
    {
        //ITS only POP rm16/32
        uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
        uint8_t width = 16;
        if(machine->internal_state.operand_32_bit) width = 32;

        void* dest = get_memory_from_mode(machine, mod_rm, width);
        if(width == 16)
            *((uint16_t*)dest) = pop_word(machine);
        else if(width == 32)
            *((uint32_t*)dest) = pop_dword(machine);
    }
    else return UNDEFINED_OPCODE;
    recalculate_ip: machine->IP.w.ip += machine->internal_state.IPOffset;

    return status;
}