#include <stdint.h>
#include <v8086/stack.h>
#include <stdbool.h>
#include <v8086/mod_rm_parsing.h>
#include "opcodes.h"
#include "arithmetic_operations.h"
#include "ascii_adjustments_operations.h"
#include "stack_operations.h"
#include "jump_operations.h"
#include "internal_funcs.h"
#include "exchange_operations.h"
#include "mov_operations.h"
#include "procedure_operations.h"
#include "string_operations.h"
#include "io_operations.h"
#include "misc_operations.h"
#include "bit_operations.h"

#define NO_CARRY 0
#define CARRY_FLAG_AS_NUMBER bit_get(machine->regs.d.eflags, 1u <<CARRY_FLAG_BIT) >> CARRY_FLAG_BIT

OPCODE_PROTO(add)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode, NO_CARRY, perform_adding);
}

OPCODE_PROTO(push_es)
{
    push_word(machine, machine->sregs.es);
    return V8086_OK;
}

OPCODE_PROTO(pop_es)
{
    machine->sregs.es = pop_word(machine);
    return V8086_OK;
}

OPCODE_PROTO(or)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x08, NO_CARRY, perform_or);
}

OPCODE_PROTO(push_cs)
{
    push_word(machine, machine->sregs.cs);
    return V8086_OK;
}

OPCODE_PROTO(adc)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x10, CARRY_FLAG_AS_NUMBER, perform_adding);
}

OPCODE_PROTO(push_ss)
{
    push_word(machine, machine->sregs.ss);
    return V8086_OK;
}

OPCODE_PROTO(pop_ss)
{
    machine->sregs.ss = pop_word(machine);
    return V8086_OK;
}

OPCODE_PROTO(sbb)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x18, CARRY_FLAG_AS_NUMBER, perform_subtracting);
}

OPCODE_PROTO(push_ds)
{
    push_word(machine, machine->sregs.ds);
    return V8086_OK;
}

OPCODE_PROTO(pop_ds)
{
    machine->sregs.ds = pop_word(machine);
    return V8086_OK;
}

OPCODE_PROTO(and)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x20, 0, perform_and);
}

OPCODE_PROTO(daa)
{
    return adjust_after_add_sub_packed(machine, false);
}

OPCODE_PROTO(sub)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x28, NO_CARRY, perform_subtracting);
}

OPCODE_PROTO(das)
{
    return adjust_after_add_sub_packed(machine, true);
}

OPCODE_PROTO(xor)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x30, NO_CARRY, perform_xor);
}

OPCODE_PROTO(aaa)
{
    return adjust_after_add_sub(machine, false);
}

OPCODE_PROTO(cmp)
{
    return perform_artihmetic_or_logical_instruction(machine, opcode - 0x38, NO_CARRY, perform_cmp);
}

OPCODE_PROTO(aas)
{
    return adjust_after_add_sub(machine, true);
}

OPCODE_PROTO(inc)
{
    return perform_inc_dec(machine, opcode, false);
}

OPCODE_PROTO(dec)
{
    return perform_inc_dec(machine, opcode, true);
}

OPCODE_PROTO(push_gpr)
{
    return push_gpr(machine, opcode);
}

OPCODE_PROTO(pop_gpr)
{
    return pop_gpr(machine, opcode);
}

OPCODE_PROTO(jcc)
{
    return jump_short_relative_on_condition(machine, opcode);
}

OPCODE_PROTO(group1)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t recalculated_opcode = opcode - 0x80;
    switch(get_reg(mod_rm))
    {
        case 0: //ADD
            return perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm, NO_CARRY,
                                                                     perform_adding);
        case 1: //OR
            return perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm, NO_CARRY, perform_or);
        case 2: //ADC
            return perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm,
                                                                     CARRY_FLAG_AS_NUMBER, perform_adding);
        case 3: //SBB
            return perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm,
                                                                     CARRY_FLAG_AS_NUMBER, perform_subtracting);
        case 4: //AND
            return perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm, NO_CARRY, perform_and);
        case 5: //SUB
            return perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm, NO_CARRY,
                                                                     perform_subtracting);
        case 6: //XOR
            return perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm, NO_CARRY, perform_xor);
        case 7: //CMP
            return perform_arithmetic_or_logical_instruction_group(machine, recalculated_opcode, mod_rm, NO_CARRY, perform_cmp);
        default:
            return V8086_BAD_REG;
    }
}

OPCODE_PROTO(test)
{
    return execute_test(machine, opcode);
}

OPCODE_PROTO(xchg)
{
    return perform_exchange_rm(machine, opcode);
}

OPCODE_PROTO(mov_rm)
{
    return perform_mov_rm(machine, opcode);
}

OPCODE_PROTO(mov_segment)
{
    return perform_mov_segment(machine, opcode);
}

OPCODE_PROTO(lea)
{
    return perform_lea(machine);
}

OPCODE_PROTO(pop_rm)
{
    return pop_rm(machine);
}

OPCODE_PROTO(xchg_ax)
{
    return perform_exchange_ax_register(machine, opcode);
}

OPCODE_PROTO(cbw)
{
    return convert_byte_to_word(machine);
}

OPCODE_PROTO(cwd)
{
    return convert_word_to_double(machine);
}

OPCODE_PROTO(far_call)
{
    return far_call(machine);
}

OPCODE_PROTO(pushf)
{
    push_word(machine, machine->regs.w.flags);
    return V8086_OK;
}

OPCODE_PROTO(popf)
{
    machine->regs.w.flags = pop_word(machine);
    return V8086_OK;
}

OPCODE_PROTO(sahf)
{
    return store_flags(machine);
}

OPCODE_PROTO(lahf)
{
    return load_flags(machine);
}

OPCODE_PROTO(mov_moffset)
{
    return perform_mov_moffset(machine, opcode);
}
OPCODE_PROTO(movsb)
{
    return perform_movs(machine, 8);
}
OPCODE_PROTO(movsw)
{
    return perform_movs(machine, machine->internal_state.operand_32_bit ? 32: 16);
}
OPCODE_PROTO(cmpsb)
{
    return perform_cmps(machine, 8);
}
OPCODE_PROTO(cmpsw)
{
    return perform_cmps(machine, machine->internal_state.operand_32_bit ? 32: 16);
}
OPCODE_PROTO(test_imm)
{
    return execute_test_immediate(machine, opcode);
}
OPCODE_PROTO(stosb)
{
    return perform_stos(machine, 8);
}
OPCODE_PROTO(stosw)
{
    return perform_stos(machine, machine->internal_state.operand_32_bit ? 32: 16);
}
OPCODE_PROTO(lodsb)
{
    return perform_lods(machine, 8);
}
OPCODE_PROTO(lodsw)
{
    return perform_lods(machine, machine->internal_state.operand_32_bit ? 32: 16);
}
OPCODE_PROTO(scasb)
{
    return perform_scas(machine, 8);
}
OPCODE_PROTO(scasw)
{
    return perform_scas(machine, machine->internal_state.operand_32_bit ? 32: 16);
}

OPCODE_PROTO(mov_gpr_imm)
{
    return perform_mov_gpr_imm(machine, opcode);
}

OPCODE_PROTO(retn)
{
    return near_ret(machine);
}

OPCODE_PROTO(retn_imm)
{
    return near_ret_imm(machine);
}

OPCODE_PROTO(les)
{
    return perform_load_far_pointer(machine, V8086_ES);
}

OPCODE_PROTO(lds)
{
    return perform_load_far_pointer(machine, V8086_DS);
}

OPCODE_PROTO(mov_rm_imm)
{
    return perform_mov_rm_imm(machine, opcode);
}

OPCODE_PROTO(retf_imm)
{
    return far_ret_imm(machine);
}

OPCODE_PROTO(retf)
{
    return far_ret(machine);
}

OPCODE_PROTO(interrupt)
{
    uint8_t interrupt_number = 3;
    if(opcode == 0xcd) //INT, imm
    {
        interrupt_number = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
        machine->internal_state.IPOffset += 1;
    }
    else if(opcode == 0xce) //INTO
    {
        if(!bit_get(machine->regs.w.flags, 1u << OVERFLOW_FLAG_BIT)) return V8086_OK;
        interrupt_number = 4;
    }

    return perform_interrupt(machine, interrupt_number);
}

OPCODE_PROTO(iret)
{
    return perform_iret(machine);
}

OPCODE_PROTO(group_2)
{
    return execute_group_2(machine, opcode);
}

OPCODE_PROTO(aam)
{
    return adjust_after_mul_div(machine, 0);
}

OPCODE_PROTO(aad)
{
    return adjust_after_mul_div(machine, 1);
}

OPCODE_PROTO(xlat)
{
    return perform_xlat(machine);
}

OPCODE_PROTO(loop)
{
    return perform_loop_loopne(machine, opcode);
}

OPCODE_PROTO(jrcxz)
{
    return jump_short_relative_on_condition(machine, opcode);
}

OPCODE_PROTO(inb_imm)
{
    return perform_in_imm(machine, 8);
}

OPCODE_PROTO(inw_imm)
{
    return perform_in_imm(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(outb_imm)
{
    return perform_out_imm(machine, 8);
}

OPCODE_PROTO(outw_imm)
{
    return perform_in_imm(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(call_near)
{
    return near_relative_call(machine);
}

OPCODE_PROTO(jmp_near)
{
    return jump_near_relative(machine);
}

OPCODE_PROTO(jmp_far)
{
    return jump_far(machine);
}

OPCODE_PROTO(jmp_short)
{
    return jump_short_relative(machine);
}

OPCODE_PROTO(inb_dx)
{
    return perform_in_dx(machine, 8);
}

OPCODE_PROTO(inw_dx)
{
    return perform_in_dx(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(outb_dx)
{
    return perform_out_dx(machine, 8);
}

OPCODE_PROTO(outw_dx)
{
    return perform_out_dx(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(set_flag)
{
    return setting_and_clearing_flags(machine, opcode);
}

OPCODE_PROTO(group_3)
{
    return execute_group_3(machine, opcode);
}

OPCODE_PROTO(group_4)
{
    return perform_group_4(machine);
}
OPCODE_PROTO(group_5)
{
    return perform_group_5(machine);
}

//386 PROTOS

OPCODE_PROTO(push_all)
{
    return push_all(machine);
}

OPCODE_PROTO(pop_all)
{
    return pop_all(machine);
}

OPCODE_PROTO(bound)
{
    return check_bounds(machine);
}

OPCODE_PROTO(push_imm16_32)
{
    return push_immediate(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(push_imm8)
{
    return push_immediate(machine, 8);
}

OPCODE_PROTO(imul_reg_reg_imm)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = machine->internal_state.operand_32_bit ? 32 : 16;
    void* mem_or_reg = get_memory_from_mode(machine, mod_rm, width);
    void* reg = get_variable_length_register(machine, get_reg(mod_rm), width);
    void* imm = get_variable_length_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset), width);
    machine->internal_state.IPOffset += width/8;

    return perform_multiplication_3_byte(machine, reg, mem_or_reg, imm, 1, width, width);
}

OPCODE_PROTO(imul_reg_reg_imm8)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = machine->internal_state.operand_32_bit ? 32 : 16;
    void* mem_or_reg = get_memory_from_mode(machine, mod_rm, width);
    void* reg = get_variable_length_register(machine, get_reg(mod_rm), width);
    uint8_t imm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    return perform_multiplication_3_byte(machine, reg, mem_or_reg, &imm, 1, width, 8);
}

OPCODE_PROTO(ins8){
    return perform_ins_dx(machine, 8);
}

OPCODE_PROTO(ins)
{
    return perform_ins_dx(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(outs8){
    return perform_outs_dx(machine, 8);
}

OPCODE_PROTO(outs) {
    return perform_outs_dx(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(enter)
{
    return enter(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(leave)
{
    return leave(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

//386 2_bytes_protos 0x0f Prefix

OPCODE_PROTO(two_byte_0fh){
    uint8_t secondary_opcode;
    secondary_opcode = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;

    if(machine->operations_0fh[secondary_opcode] == NULL) return V8086_UNDEFINED_OPCODE;

    return machine->operations_0fh[secondary_opcode](machine, secondary_opcode);
}

OPCODE_PROTO(jcc_l)
{
    return jump_on_condition(machine, opcode, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(setcc)
{
    return set_byte(machine, opcode);
}

OPCODE_PROTO(push_fs)
{
    push_word(machine, machine->sregs.fs);
    return V8086_OK;
}

OPCODE_PROTO(pop_fs)
{
    machine->sregs.fs = pop_word(machine);
    return V8086_OK;
}

OPCODE_PROTO(push_gs)
{
    push_word(machine, machine->sregs.gs);
    return V8086_OK;
}

OPCODE_PROTO(pop_gs)
{
    machine->sregs.gs = pop_word(machine);
    return V8086_OK;
}

OPCODE_PROTO(bt)
{
    return bit_test(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(bts)
{
    return bit_test_set(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(btr)
{
    return bit_test_reset(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(btc)
{
    return bit_test_complement(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(group_8)
{
    return perform_group_8(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(shld_imm)
{
    return execute_double_shift(machine, machine->internal_state.operand_32_bit ? 32 : 16, 0, 0);
}

OPCODE_PROTO(shld_cl)
{
    return execute_double_shift(machine, machine->internal_state.operand_32_bit ? 32 : 16, 0, 1);
}

OPCODE_PROTO(shrd_imm)
{
    return execute_double_shift(machine, machine->internal_state.operand_32_bit ? 32 : 16, 1, 0);
}

OPCODE_PROTO(shrd_cl)
{
    return execute_double_shift(machine, machine->internal_state.operand_32_bit ? 32 : 16, 1, 1);
}

OPCODE_PROTO(imul_2_byte)
{
    uint8_t mod_rm = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t width = machine->internal_state.operand_32_bit ? 32 : 16;
    void* mem_or_reg = get_memory_from_mode(machine, mod_rm, width);
    void* reg = get_variable_length_register(machine, get_reg(mod_rm), width);

    return perform_multiplication_3_byte(machine, reg, reg, mem_or_reg, 1, width, width);
}

OPCODE_PROTO(lss)
{
    return perform_load_far_pointer(machine, V8086_SS);
}

OPCODE_PROTO(lfs)
{
    return perform_load_far_pointer(machine, V8086_FS);
}

OPCODE_PROTO(lgs)
{
    return perform_load_far_pointer(machine, V8086_GS);
}

OPCODE_PROTO(movzx)
{
    return perform_movzx(machine, opcode);
}

OPCODE_PROTO(movsx)
{
    return perform_movsx(machine, opcode);
}

OPCODE_PROTO(bsf)
{
    return bit_scan_forward(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(bsr)
{
    return bit_scan_backward(machine, machine->internal_state.operand_32_bit ? 32 : 16);
}

OPCODE_PROTO(prefix_cs)
{
    machine->internal_state.segment_reg_select = V8086_CS;
    machine->internal_state.previous_byte_was_prefix = 1;
    return V8086_OK;
}

OPCODE_PROTO(prefix_ds)
{
    machine->internal_state.segment_reg_select = V8086_DS;
    machine->internal_state.previous_byte_was_prefix = 1;
    return V8086_OK;
}

OPCODE_PROTO(prefix_es)
{
    machine->internal_state.segment_reg_select = V8086_ES;
    machine->internal_state.previous_byte_was_prefix = 1;
    return V8086_OK;
}

OPCODE_PROTO(prefix_fs)
{
    machine->internal_state.segment_reg_select = V8086_FS;
    machine->internal_state.previous_byte_was_prefix = 1;
    return V8086_OK;
}

OPCODE_PROTO(prefix_gs)
{
    machine->internal_state.segment_reg_select = V8086_GS;
    machine->internal_state.previous_byte_was_prefix = 1;
    return V8086_OK;
}

OPCODE_PROTO(prefix_ss)
{
    machine->internal_state.segment_reg_select = V8086_SS;
    machine->internal_state.previous_byte_was_prefix = 1;
    return V8086_OK;
}

OPCODE_PROTO(prefix_address_32)
{
    machine->internal_state.address_32_bit = 1;
    machine->internal_state.previous_byte_was_prefix = 1;
    return V8086_OK;
}

OPCODE_PROTO(prefix_operand_32)
{
    machine->internal_state.operand_32_bit = 1;
    machine->internal_state.previous_byte_was_prefix = 1;
    return V8086_OK;
}

OPCODE_PROTO(prefix_repne)
{
    machine->internal_state.rep_prefix = V8086_REPNE;
    machine->internal_state.previous_byte_was_prefix = 1;
    return V8086_OK;
}

OPCODE_PROTO(prefix_rep_repe)
{
    machine->internal_state.rep_prefix = V8086_REP_REPE;
    machine->internal_state.previous_byte_was_prefix = 1;
    return V8086_OK;
}

OPCODE_PROTO(prefix_lock)
{
    machine->internal_state.previous_byte_was_prefix = 1;
    return V8086_OK;
}