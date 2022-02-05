#include "v8086.h"
#include "../memory/heap/heap.h"
#include <string.h>
#include <filesystems/filesystem.h>
#include "../assembly/io.h"
#include "./memory_operations.h"
#include "./stack.h"
#include "./mod_rm_parsing.h"
#include "./operations/internal_funcs.h"
#include "./operations/arithmetic_operations.h"
#include "./operations/opcodes.h"
#include <stdio.h>
#include "../debug_helpers/library/kernel_stdio.h"

#ifdef DEBUG_V8086
    #include "../drivers/serial/serial.h"
    #include "../drivers/vga/vga.h"
    //#define DEBUG_V8086_TEXT
    //#define DEBUG_V8086_BIN
    //#define DEBUG_V8086_INTERACTIVE
#endif

bool skipDebugging = false;

int16_t parse_and_execute_instruction(v8086* machine);
uint8_t loop_flag = 0;

#ifdef DEBUG_V8086
void send_reg_32(uint32_t reg)
{
    for(int i = 0; i < 4; i++)
        serial_send(COM1_PORT, ((reg) >> (i*8)) & 0xff);
}

void send_reg_16(uint16_t reg)
{
    for(int i = 0; i < 2; i++)
        serial_send(COM1_PORT, ((reg) >> (i*8)) & 0xff);
}

void send_regs(v8086* machine)
{
    send_reg_32(machine->regs.d.edi);
    send_reg_32(machine->regs.d.esi);
    send_reg_32(machine->regs.d.ebp);
    send_reg_32(machine->regs.d.cflag);
    send_reg_32(machine->regs.d.ebx);
    send_reg_32(machine->regs.d.edx);
    send_reg_32(machine->regs.d.ecx);
    send_reg_32(machine->regs.d.eax);
    send_reg_16(machine->regs.d.eflags);
    send_reg_32(machine->regs.d.esp);
}

void send_sregs(v8086* machine)
{
    send_reg_16(machine->sregs.es);
    send_reg_16(machine->sregs.ds);
    send_reg_16(machine->sregs.fs);
    send_reg_16(machine->sregs.gs);
    send_reg_16(machine->sregs.cs);
    send_reg_16(machine->sregs.ss);
}

void read_regs(v8086* machine)
{
    machine->regs.d.edi = read_reg_32();
    machine->regs.d.esi = read_reg_32();
    
    machine->regs.d.ebp = read_reg_32();
    //char x[100];
    //kernel_sprintf(x, "%x\n", read_reg_32());
    //kernel_sprintf(x, "%x\n", machine->regs.d.ebp);
    //vga_printstring(x);
    machine->regs.d.cflag = read_reg_32();
    machine->regs.d.ebx = read_reg_32();
    machine->regs.d.edx = read_reg_32();
    machine->regs.d.ecx = read_reg_32();
    machine->regs.d.eax = read_reg_32();
    machine->regs.d.eflags = read_reg_16();
    machine->regs.d.esp = read_reg_32();
    //kernel_sprintf(x, "%x\n", machine->regs.d.esp);
    //vga_printstring(x);
}

void read_sregs(v8086* machine)
{
    machine->sregs.es = read_reg_16();
    machine->sregs.ds = read_reg_16();
    machine->sregs.fs = read_reg_16();
    machine->sregs.gs = read_reg_16();
    machine->sregs.cs = read_reg_16();
    machine->sregs.ss = read_reg_16();
}

uint16_t read_reg_16()
{
    uint16_t value;
    value = (uint16_t)serial_receive(COM1_PORT);
    value |= ((uint16_t)serial_receive(COM1_PORT) & 0xFF) << 8; 
    return value;
}

uint8_t read_reg_8()
{
    return serial_receive(COM1_PORT);
}

uint32_t read_reg_32()
{
    uint32_t value;
    value = ((uint32_t)serial_receive(COM1_PORT) & 0xFF);
    value |= ((uint32_t)serial_receive(COM1_PORT) & 0xFF) << 8;
    value |= ((uint32_t)serial_receive(COM1_PORT) & 0xFF) << 16;
    value |= ((uint32_t)serial_receive(COM1_PORT) & 0xFF) << 24; 
    return value;
}

void setSkipDebugging(bool value)
{
    skipDebugging = value;
}

#endif

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
    //RESERVED TO SEG=V8086_ES PREFIX
    ASSIGN_NULL(0x26u);
    ASSIGN_OPCODE(0x27u, daa);
    GROUP_OF_OPCODES(0x28u, 0x2du, sub);
    //RESERVED TO SEG=V8086_CS PREFIX
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
    ASSIGN_OPCODE(0xc2u, retn_imm);
    ASSIGN_OPCODE(0xc3u, retn);
    ASSIGN_OPCODE(0xc4u, les);
    ASSIGN_OPCODE(0xc5u, lds);
    GROUP_OF_OPCODES(0xc6u, 0xc7u, mov_rm_imm);
    //NOT DEFINED IN 8086 processor
    ASSIGN_NULL(0xc8u);
    ASSIGN_NULL(0xc9u);
    ASSIGN_OPCODE(0xcau, retf_imm);
    ASSIGN_OPCODE(0xcbu, retf);
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
    ASSIGN_OPCODE(0xecu, inb_dx);
    ASSIGN_OPCODE(0xedu, inw_dx);
    ASSIGN_OPCODE(0xeeu, outb_dx);
    ASSIGN_OPCODE(0xefu, outw_dx);
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

    ASSIGN_OPCODE(0x26u, prefix_es);
    ASSIGN_OPCODE(0x36u, prefix_ss);
    ASSIGN_OPCODE(0x64u, prefix_fs);
    ASSIGN_OPCODE(0x65u, prefix_gs);
    ASSIGN_OPCODE(0x2eu, prefix_cs);
    ASSIGN_OPCODE(0x3eu, prefix_ds);

    ASSIGN_OPCODE(0x66u, prefix_operand_32);
    ASSIGN_OPCODE(0x67u, prefix_address_32);

    ASSIGN_OPCODE(0xf0u, prefix_lock);
    ASSIGN_OPCODE(0xf2u, prefix_repne);
    ASSIGN_OPCODE(0xf3u, prefix_rep_repe);

    machine->is_compatibility = V8086_IS8086;
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
    ASSIGN_OPCODE(0xc0u, group_2);
    ASSIGN_OPCODE(0xc1u, group_2);
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
    GROUP_OF_0FH_OPCODES(0x80u, 0x8fu, jcc_l);
    GROUP_OF_0FH_OPCODES(0x90u, 0x9fu, setcc);
    ASSIGN_0FH_OPCODE(0xa0u, push_fs);
    ASSIGN_0FH_OPCODE(0xa1u, pop_fs);
    //NO CPUID in 386
    ASSIGN_NULL_0FH(0xa2u);
    ASSIGN_0FH_OPCODE(0xa3u, bt);
    ASSIGN_0FH_OPCODE(0xa4u, shld_imm);
    ASSIGN_0FH_OPCODE(0xa5u, shld_cl);
    //Not Defined in 386;
    ASSIGN_NULL_0FH(0xa6u);
    ASSIGN_NULL_0FH(0xa7u);
    ASSIGN_0FH_OPCODE(0xa8u, push_gs);
    ASSIGN_0FH_OPCODE(0xa9u, pop_gs);
    //NOT Defined in 386
    ASSIGN_NULL_0FH(0xaau);
    ASSIGN_0FH_OPCODE(0xabu, bts);
    ASSIGN_0FH_OPCODE(0xacu, shrd_imm);
    ASSIGN_0FH_OPCODE(0xadu, shrd_cl);
    //NOT Defined in 386
    ASSIGN_NULL_0FH(0xaeu);
    ASSIGN_0FH_OPCODE(0xafu, imul_2_byte);
    //NOT Defined in 386
    ASSIGN_NULL_0FH(0xb0u);
    ASSIGN_NULL_0FH(0xb1u);
    ASSIGN_0FH_OPCODE(0xb2u, lss);
    ASSIGN_0FH_OPCODE(0xb3u, btr);
    ASSIGN_0FH_OPCODE(0xb4u, lfs);
    ASSIGN_0FH_OPCODE(0xb5u, lgs);
    ASSIGN_0FH_OPCODE(0xb6u, movzx); // BYTE MOVZX
    ASSIGN_0FH_OPCODE(0xb7u, movzx); // WORD MOVZX
    //NOT defined in 386
    ASSIGN_NULL_0FH(0xb8u);
    ASSIGN_NULL_0FH(0xb9u);
    ASSIGN_0FH_OPCODE(0xbau, group_8);
    ASSIGN_0FH_OPCODE(0xbbu, btc);
    ASSIGN_0FH_OPCODE(0xbcu, bsf);
    ASSIGN_0FH_OPCODE(0xbdu, bsr);
    ASSIGN_0FH_OPCODE(0xbeu, movsx); // BYTE MOVSX
    ASSIGN_0FH_OPCODE(0xbfu, movsx); //BYTE MOVSX

    machine->is_compatibility = V8086_IS386;
}

v8086* v8086_create_machine()
{
    vga_printstring("HERE\n");
    v8086* machine = (v8086*) heap_kernel_alloc(sizeof(v8086), 0);
    if(machine == NULL) vga_printstring("NULL \n");
    vga_printstring("NOT HERE\n");
    if(machine == NULL) return NULL;
    vga_printstring("HERE\n");
    memset(machine, 0, sizeof(v8086));
    vga_printstring("NOT HERE\n");
    machine->regs.x.flags = 0x2;
    machine->sregs.cs = 0xf000;
    machine->IP.w.ip = 0xfff0;
    machine->sregs.ss = 0x0;
    machine->regs.d.ebp = 0x7bff;
    machine->regs.d.esp = 0x7bff;
    vga_printstring("HERE\n");
	memcpy(machine->Memory, (void*)0xc0000000, 0x100000);
    vga_printstring("NOT HERE\n");
    v8086_set_8086_instruction_set(machine);
    vga_printstring("HERE?\n");
    return machine;
}

void v8086_destroy_machine(v8086* machine)
{
    heap_kernel_dealloc(machine);
}

int16_t v8086_call_function(v8086* machine)
{
    while(!(machine->IP.w.ip == 0xFFFF && machine->sregs.cs == 0xFFFF))
    {
        int16_t status = parse_and_execute_instruction(machine);
        if(status != V8086_OK) return status;
    }
    return V8086_OK;
}

int16_t v8086_call_int(v8086* machine, int16_t num)
{
    if ((num < 0) || (num > 0xFF)) return V8086_BAD_INT_NUMBER;
    machine->IP.w.ip = read_word_from_pointer(machine->Memory, get_absolute_address(0, num * 4));
    machine->sregs.cs = read_word_from_pointer(machine->Memory, get_absolute_address(0, num * 4 + 2));
    push_word(machine, machine->regs.w.flags);
    push_word(machine, 0xFFFF);
    push_word(machine, 0xFFFF);
    int16_t x = v8086_call_function(machine);
    if(x != V8086_OK) return x;
    return num;
}

uint32_t v8086_get_address_of_int(v8086* machine, int16_t num)
{
    uint32_t ip = read_word_from_pointer(machine->Memory, get_absolute_address(0, num * 4));
    uint32_t cs = read_word_from_pointer(machine->Memory, get_absolute_address(0, num * 4 + 2));
    return cs * 0x10 + ip;
}

int16_t parse_and_execute_instruction(v8086* machine)
{
    //if(!machine->internal_state.previous_byte_was_prefix) {
        machine->IP.w.ip += machine->internal_state.IPOffset;
        machine->internal_state.IPOffset = 0;
        machine->internal_state.operand_32_bit = 0;
        machine->internal_state.address_32_bit = 0;
        machine->internal_state.segment_reg_select = V8086_DEFAULT;
        machine->internal_state.rep_prefix = V8086_NONE_REPEAT;
    //}

    int16_t status = V8086_OK;

    #ifdef DEBUG_V8086
        #ifdef DEBUG_V8086_TEXT
        serial_send_string(COM1_PORT, "REGS:\n");
        char str[100] = "";
        kernel_sprintf(str, "AL:%02X AH:%02X AX:%04X eAX:%08X\n", machine->regs.h.al, machine->regs.h.ah, machine->regs.x.ax, machine->regs.d.eax);
        serial_send_string(COM1_PORT, str);
        kernel_sprintf(str, "BL:%02X BH:%02X BX:%04X eBX:%08X\n", machine->regs.h.bl, machine->regs.h.bh, machine->regs.x.bx, machine->regs.d.ebx);
        serial_send_string(COM1_PORT, str);
        kernel_sprintf(str, "CL:%02X CH:%02X CX:%04X eCX:%08X\n", machine->regs.h.cl, machine->regs.h.ch, machine->regs.x.cx, machine->regs.d.ecx);
        serial_send_string(COM1_PORT, str);
        kernel_sprintf(str, "DL:%02X DH:%02X DX:%04X eDX:%08X\n", machine->regs.h.dl, machine->regs.h.dh, machine->regs.x.dx, machine->regs.d.edx);
        serial_send_string(COM1_PORT, str);
        kernel_sprintf(str, "DI:%04X eDI:%08X\n", machine->regs.h.di, machine->regs.d.edi);
        serial_send_string(COM1_PORT, str);
        kernel_sprintf(str, "SI:%04X eSI:%08X\n", machine->regs.h.si, machine->regs.d.esi);
        serial_send_string(COM1_PORT, str);
        kernel_sprintf(str, "FLAGS:%04X\n", machine->regs.w.flags);
        serial_send_string(COM1_PORT, str);
        serial_send_string(COM1_PORT, "SREGS:\n");
        kernel_sprintf(str, "DS:%04X ES:%04X\n", machine->sregs.ds, machine->sregs.es);
        serial_send_string(COM1_PORT, str);
        kernel_sprintf(str, "FS:%04X GS:%04X\n", machine->sregs.fs, machine->sregs.gs);
        serial_send_string(COM1_PORT, str);
        serial_send_string(COM1_PORT, "STACK:\n");
        kernel_sprintf(str, "BP:%04X eBP:%08X\n", machine->regs.h.bp, machine->regs.d.ebp);
        serial_send_string(COM1_PORT, str);
        kernel_sprintf(str, "SP:%04X eSP:%08X\n", machine->regs.w.sp, machine->regs.d.esp);
        serial_send_string(COM1_PORT, str);
        kernel_sprintf(str, "SS:%04X\n", machine->sregs.ss);
        serial_send_string(COM1_PORT, str);
        if((machine->regs.d.esp < 0x7bff) && (machine->sregs.ss == 0x0)){
            serial_send_string(COM1_PORT, "---\n");
            for(uint32_t i=0x7bff - 1; i >= machine->regs.d.esp; i-=1)
            {
                kernel_sprintf(str, "%02X ",read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, i)));
                serial_send_string(COM1_PORT, str);
                if(!((0x7bff - i) % 2)){
                    kernel_sprintf(str, ":%02X\n", i);
                    serial_send_string(COM1_PORT, str);
                }
                else if(i - 1 < machine->regs.d.esp) serial_send_string(COM1_PORT, "\n");
            }
            serial_send_string(COM1_PORT, "---\n");
        }
        else if((machine->regs.d.esp < 0x200) && (machine->sregs.ss == 0xeb62)){
            serial_send_string(COM1_PORT, "---\n");
            for(uint32_t i=0x200 - 1; i >= machine->regs.d.esp; i-=1)
            {
                kernel_sprintf(str, "%02X ",read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, i)));
                serial_send_string(COM1_PORT, str);
                if(!((0x200 - i) % 2)){
                    kernel_sprintf(str, ":%02X\n", i);
                    serial_send_string(COM1_PORT, str);
                }
                else if(i - 1 < machine->regs.d.esp) serial_send_string(COM1_PORT, "\n");
            }
            serial_send_string(COM1_PORT, "---\n");
        }
        else{
            serial_send_string(COM1_PORT, "WEIRD STACK ADDRESS!\n");
        }
        serial_send_string(COM1_PORT, "INSTRUCTION ADDRESS:\n");
        kernel_sprintf(str, "CS:%04X IP:%04X\n", machine->sregs.cs, machine->IP.w.ip);
        serial_send_string(COM1_PORT, str);
        #endif

        #ifdef DEBUG_V8086_BIN
        for(uint32_t i = 0; i < sizeof(v8086); i++)
        {
            serial_send(COM1_PORT, machine->bytes[i]);
        }
        #endif

        #ifdef DEBUG_V8086_INTERACTIVE
        if(!skipDebugging)
        while(true)
        {
            char d[100];
            vga_printstring("Waiting for commands!\n");
            char debug_operation = serial_receive(COM1_PORT);
            vga_printstring("Recived Byte: \n");
            vga_printchar(debug_operation);
            vga_printchar(' ');
            itoa(debug_operation, d, 10);
            vga_printstring(d);
            vga_newline();
            if(debug_operation == 0)
                break;
            else if(debug_operation == 1)
                send_regs(machine);
            else if(debug_operation == 2)
                send_sregs(machine);
            else if(debug_operation == 3)
            {
                uint16_t seg;
                uint16_t off;
                seg = (uint16_t)serial_receive(COM1_PORT) << 8;
                seg |= serial_receive(COM1_PORT);
                off = (uint16_t)serial_receive(COM1_PORT) << 8;
                off |= serial_receive(COM1_PORT);
                uint8_t mem = read_byte_from_pointer(machine->Memory, get_absolute_address(seg, off));
                serial_send(COM1_PORT, mem);
            }
            else if(debug_operation == 4)
            {
                uint16_t seg;
                uint16_t off;
                seg = (uint16_t)serial_receive(COM1_PORT) << 8;
                seg |= serial_receive(COM1_PORT);
                off = (uint16_t)serial_receive(COM1_PORT) << 8;
                off |= serial_receive(COM1_PORT);
                uint16_t mem = read_word_from_pointer(machine->Memory, get_absolute_address(seg, off));
                send_reg_16(mem);
            }
            else if(debug_operation == 5)
            {
                uint16_t seg;
                uint16_t off;
                seg = (uint16_t)serial_receive(COM1_PORT) << 8;
                seg |= serial_receive(COM1_PORT);
                off = (uint16_t)serial_receive(COM1_PORT) << 8;
                off |= serial_receive(COM1_PORT);
                uint32_t mem = read_dword_from_pointer(machine->Memory, get_absolute_address(seg, off));
                send_reg_32(mem);
            }
            else if(debug_operation == 6)
            {
                send_reg_16(machine->IP.w.ip);
            }
            else if(debug_operation == 7)
            {
                for(int i = 0; i < 0x100000; i++)
                    serial_send(COM1_PORT,machine->Memory[i]);
            } 
            else if(debug_operation == 8)
            {
                uint16_t seg;
                seg = (uint16_t)serial_receive(COM1_PORT) << 8;
                seg |= serial_receive(COM1_PORT);         
                for(int i = 0; i < 64 * 1024; i++)
                    serial_send(COM1_PORT,machine->Memory[seg * 0x10 + i]);              
            }
            else{
                vga_printstring("Unknown byte: ");
                vga_printchar(debug_operation);
                vga_newline();
            }
        }
        #endif
    #endif

    //Maybe opcode, an be also prefix
    uint8_t opcode;
    decode: opcode = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    
    
    /*char str[5] = "";
    itoa(machine->sregs.cs, str, 16);
    bool s;
    s = filesystem_append_to_file("A:/CSIP.BIN", str, 4);
    if(!s) return V8086_DEBUG_FILE_OVERFLOW;
    itoa(machine->IP.w.ip, str, 16);
    s = filesystem_append_to_file("A:/CSIP.BIN", str, 4);
    if(!s) return V8086_DEBUG_FILE_OVERFLOW;*/
    
    //PREFIXES
    //Segment Prefix V8086_CS DS V8086_ES SS
    /*if((opcode & 0x7u) == 0x6 && ((opcode >> 5u) & 0x7u) == 0x1u) //001XX110 pattern where XX is number of segment
    {
        machine->internal_state.segment_reg_select = (opcode >> 3u) & 0x3u;
        goto decode; //continue parsing opcode;
    }
    //Segment Prefix FS
    else if(opcode == 0x64)
    {
        machine->internal_state.segment_reg_select = V8086_FS;
        goto decode; //continue parsing opcode;
    }
    //Segment Prefix GS
    else if(opcode == 0x65)
    {
        machine->internal_state.segment_reg_select = V8086_GS;
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
        machine->internal_state.rep_prefix = V8086_REPNE;
        goto decode; //continue parsing opcode;
    }
    //REP/REPE Prefix
    else if(opcode == 0xF3)
    {
        machine->internal_state.rep_prefix = V8086_REP_REPE;
        goto decode; //continue parsing opcode;
    }
    //LOCK Prefix
    else if(opcode == 0xF0)
    {
        goto decode; //ommit prefix, contniue parsinf opcode; 
    }*/

    if(machine->operations[opcode] != NULL)
        status = machine->operations[opcode](machine, opcode);
    else
        return V8086_UNDEFINED_OPCODE;

    if(machine->internal_state.previous_byte_was_prefix)
    {
        machine->internal_state.previous_byte_was_prefix = 0;
        goto decode;
    }

    #ifdef DEBUG_V8086
        #ifdef DEBUG_V8086_TEXT
        serial_send_string(COM1_PORT, "------------------------------------------\n");
        #endif
    #endif
    return status;
}

int16_t v8086_call_com_program(v8086* machine, char* programPath)
{
    filesystem_file_info info;
	filesystem_get_file_info(programPath, &info);

	uint8_t *buffer = machine->Memory + 0x7C00 + 0x100; //SEG 0x7C0, OFF: 0x100
    filesystem_read_file(programPath, buffer, 0, info.size);

    machine->sregs.cs = 0x7c0;
    machine->sregs.ds = 0x7c0;
    machine->sregs.es = 0x7c0;
    machine->sregs.fs = 0x7c0;
    machine->sregs.gs = 0x7c0;
    machine->sregs.ss = 0x7c0;
    machine->IP.w.ip = 0x100;
    machine->regs.w.sp = 0xFFFE;

    push_word(machine, machine->regs.w.flags);
    push_word(machine, 0xFFFF);
    push_word(machine, 0xFFFF);
    int16_t x = v8086_call_function(machine);
    return x;
}