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
    GROUP_OF_OPCODES(0x50u, 0x57u, push_gpr);
    GROUP_OF_OPCODES(0x58u, 0x5fu, pop_gpr);
    //NOT DEFINED IN 8086 processor
    for(uint8_t i = 0x60u; i <= 0x6fu; i++) ASSIGN_NULL(i);
    GROUP_OF_OPCODES(0x70u, 0x7fu, jcc);
    GROUP_OF_OPCODES(0x80u, 0x83u, group1);
    GROUP_OF_OPCODES(0x84u, 0x85u, test);
    GROUP_OF_OPCODES(0x86u, 0x87u, xchg);
    GROUP_OF_OPCODES(0x88u, 0x8bu, mov_rm);
    ASSIGN_OPCODE(0x8cu, mov_segment);
    ASSIGN_OPCODE(0x8du, lea);
    ASSIGN_OPCODE(0x8eu, mov_segment);
    ASSIGN_OPCODE(0x8fu, pop_rm);
    GROUP_OF_OPCODES(0x90u, 0x97u, xchg_ax);
    ASSIGN_OPCODE(0x98u, cbw);
    ASSIGN_OPCODE(0x99u, cwd);
    ASSIGN_OPCODE(0x9au, far_call);
    //NO COPROCESSOR
    ASSIGN_NULL(0x9bu);
    ASSIGN_OPCODE(0x9cu, pushf);
    ASSIGN_OPCODE(0x9du, popf);
    ASSIGN_OPCODE(0x9eu, sahf);
    ASSIGN_OPCODE(0x9fu, lahf);
    GROUP_OF_OPCODES(0xa0u, 0xa3u, mov_moffset);
    ASSIGN_OPCODE(0xa4u, movsb);
    ASSIGN_OPCODE(0xa5u, movsw);
    ASSIGN_OPCODE(0xa6u, cmpsb);
    ASSIGN_OPCODE(0xa7u, cmpsw);
    GROUP_OF_OPCODES(0xa8u, 0xa9u, test_imm);
    ASSIGN_OPCODE(0xaau, stosb);
    ASSIGN_OPCODE(0xabu, stosw);
    ASSIGN_OPCODE(0xacu, lodsb);
    ASSIGN_OPCODE(0xadu, lodsw);
    ASSIGN_OPCODE(0xaeu, scasb);
    ASSIGN_OPCODE(0xafu, scasw);
    GROUP_OF_OPCODES(0xb0u, 0xbfu, mov_gpr_imm);
    //NOT DEFINED IN 8086 processor
    ASSIGN_NULL(0xc0u);
    ASSIGN_NULL(0xc1u);
    ASSIGN_OPCODE(0xc2u, retn);
    ASSIGN_OPCODE(0xc3u, retn_imm);
    ASSIGN_OPCODE(0xc4u, les);
    ASSIGN_OPCODE(0xc5u, lds);
    GROUP_OF_OPCODES(0xc6u, 0xc7u, mov_rm_imm);
    //NOT DEFINED IN 8086 processor
    ASSIGN_NULL(0xc8u);
    ASSIGN_NULL(0xc9u);
    GROUP_OF_OPCODES(0xccu, 0xceu, interrupt);
    ASSIGN_OPCODE(0xcfu, iret);
    GROUP_OF_OPCODES(0xd0u, 0xd3u, group_2);
    ASSIGN_OPCODE(0xd4u, aam);
    ASSIGN_OPCODE(0xd5u, aad);
    //NOT DEFINED IN 8086 processor
    ASSIGN_NULL(0xd6u);
    ASSIGN_OPCODE(0xd7u, xlat);
    //NO COPROCESSOR
    for(uint8_t i = 0xd8u; i <= 0xdfu; ++i) ASSIGN_NULL(i);
    GROUP_OF_OPCODES(0xe0u, 0xe2u, loop);
    ASSIGN_OPCODE(0xe3u, jrcxz);
    ASSIGN_OPCODE(0xe4u, inb_imm);
    ASSIGN_OPCODE(0xe5u, inw_imm);
    ASSIGN_OPCODE(0xe6u, outb_imm);
    ASSIGN_OPCODE(0xe7u, outw_imm);
    ASSIGN_OPCODE(0xe8u, call_near);
    ASSIGN_OPCODE(0xe9u, jmp_near);
    ASSIGN_OPCODE(0xeau, jmp_far);
    ASSIGN_OPCODE(0xebu, jmp_short);
    ASSIGN_OPCODE(0xe4u, inb_dx);
    ASSIGN_OPCODE(0xe5u, inw_dx);
    ASSIGN_OPCODE(0xe6u, outb_dx);
    ASSIGN_OPCODE(0xe7u, outw_dx);
    //RESERVED FOR LOCK PREFIX
    ASSIGN_NULL(0xf0u);
    //NOT DEFINED IN 8086
    ASSIGN_NULL(0xf1u);
    //RESERVED FOR REPNE PREFIX
    ASSIGN_NULL(0xf2u);
    //RESERVED FOR REP PREFIX
    ASSIGN_NULL(0xf3u);
    //NO EXTERNAL INTERRUPTS, HLT NOT WORKING
    ASSIGN_NULL(0xf4u);
    ASSIGN_OPCODE(0xf5u, set_flag);
    GROUP_OF_OPCODES(0xf6u, 0xf7u, group_3);
    GROUP_OF_OPCODES(0xf8u, 0xfdu, set_flag);
    ASSIGN_OPCODE(0xfeu, group_4);
    ASSIGN_OPCODE(0xffu, group_5);

    for(uint16_t i = 0u; i < 256; i++)
    {
        machine->operations_0fh[i] = NULL;
    }
}

void v8086_set_386_instruction_set(v8086* machine)
{
    v8086_set_8086_instruction_set(machine);
    ASSIGN_OPCODE(0x60u, push_all);
    ASSIGN_OPCODE(0x61u, pop_all);
    ASSIGN_OPCODE(0x62u, bound);
    //ARPL Not working in REAL MODE
    ASSIGN_NULL(0x63u);
    //RESERVED FOR SEG=FS
    ASSIGN_NULL(0x64u);
    //RESERVED FOR SEG=GS
    ASSIGN_NULL(0x65u);
    ASSIGN_OPCODE(0x68u, push_imm16_32);
    ASSIGN_OPCODE(0x69u, imul_reg_reg_imm);
    ASSIGN_OPCODE(0x6au, push_imm8);
    ASSIGN_OPCODE(0x6bu, imul_reg_reg_imm8);
    ASSIGN_OPCODE(0x6cu, ins8);
    ASSIGN_OPCODE(0x6du, ins);
    ASSIGN_OPCODE(0x6eu, outs8);
    ASSIGN_OPCODE(0x6fu, outs);
    ASSIGN_OPCODE(0xc8u, enter);
    ASSIGN_OPCODE(0xc9u, leave);

    //2 byte opcodes 0fh_prefix
    ASSIGN_OPCODE(0x0fu, two_byte_0fh);
    //NO Protected mode and CR0 emulated
    ASSIGN_NULL_0FH(0x00u);
    ASSIGN_NULL_0FH(0x01u);
    ASSIGN_NULL_0FH(0x02u);
    ASSIGN_NULL_0FH(0x03u);
    //NO DEFINED IN 386
    ASSIGN_NULL_0FH(0x04u);
    ASSIGN_NULL_0FH(0x05u);
    //NO Protected mode and CR0 emulated
    ASSIGN_NULL_0FH(0x06u);
    //NOT DEFINED IN 386
    for(uint8_t i = 0x07; i <= 0x1fu; i++)
    {
        machine->operations_0fh[i] = NULL;
    }
    //NO CR, DEBUG, TEST registers emulated
    for(uint8_t i = 0x20u; i <= 0x26u; i++)
    {
        machine->operations_0fh[i] = NULL;
    }
    //NOT DEFINED IN 386
    for(uint8_t i = 0x27u; i <= 0x7fu; i++)
    {
        machine->operations_0fh[i] = NULL;
    }

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
    machine->internal_state.address_32_bit = 0;
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
    //Address Szie Prefix
    else if(opcode == 0x67)
    {
        machine->internal_state.address_32_bit = 1;
        goto decode;
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

    if(machine->operations[opcode] != NULL)
        status = machine->operations[opcode](machine, opcode);
    else
        return UNDEFINED_OPCODE;

    machine->IP.w.ip += machine->internal_state.IPOffset;
    return status;
}