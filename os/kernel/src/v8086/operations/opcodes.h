#ifndef MICROS_OPCODES_H
#define MICROS_OPCODES_H

#include "../v8086.h"

#define OPCODE_PROTO_NAME(name) v8086_opcode_##name
#define OPCODE_PROTO(name) int16_t OPCODE_PROTO_NAME(name)(v8086* machine, uint8_t opcode)
#define ASSIGN_NULL(i) machine->operations[i] = NULL
#define ASSIGN_NULL_0FH(i) machine->operations_0fh[i] = NULL
#define ASSIGN_OPCODE(i, name) machine->operations[i] = OPCODE_PROTO_NAME(name)
#define ASSIGN_0FH_OPCODE(i, name) machine->operations_0fh[i] = OPCODE_PROTO_NAME(name)
#define GROUP_OF_OPCODES(from, to, name) for(uint8_t i = from; i <= to; i++) ASSIGN_OPCODE(i, name)
#define GROUP_OF_0FH_OPCODES(from, to, name) for(uint8_t i = from; i <= to; i++) ASSIGN_0FH_OPCODE(i, name)

OPCODE_PROTO(add);
OPCODE_PROTO(push_es);
OPCODE_PROTO(pop_es);
OPCODE_PROTO(or);
OPCODE_PROTO(push_cs);
OPCODE_PROTO(adc);
OPCODE_PROTO(push_ss);
OPCODE_PROTO(pop_ss);
OPCODE_PROTO(sbb);
OPCODE_PROTO(push_ds);
OPCODE_PROTO(pop_ds);
OPCODE_PROTO(and);
OPCODE_PROTO(daa);
OPCODE_PROTO(sub);
OPCODE_PROTO(das);
OPCODE_PROTO(xor);
OPCODE_PROTO(aaa);
OPCODE_PROTO(cmp);
OPCODE_PROTO(aas);
OPCODE_PROTO(inc);
OPCODE_PROTO(dec);
OPCODE_PROTO(push_gpr);
OPCODE_PROTO(pop_gpr);
OPCODE_PROTO(jcc);
OPCODE_PROTO(group1);
OPCODE_PROTO(test);
OPCODE_PROTO(xchg);
OPCODE_PROTO(mov_rm);
OPCODE_PROTO(mov_segment);
OPCODE_PROTO(lea);
OPCODE_PROTO(pop_rm);
OPCODE_PROTO(xchg_ax);
OPCODE_PROTO(cbw);
OPCODE_PROTO(cwd);
OPCODE_PROTO(far_call);
OPCODE_PROTO(pushf);
OPCODE_PROTO(popf);
OPCODE_PROTO(sahf);
OPCODE_PROTO(lahf);
OPCODE_PROTO(mov_moffset);
OPCODE_PROTO(movsb);
OPCODE_PROTO(movsw);
OPCODE_PROTO(cmpsb);
OPCODE_PROTO(cmpsw);
OPCODE_PROTO(test_imm);
OPCODE_PROTO(stosb);
OPCODE_PROTO(stosw);
OPCODE_PROTO(lodsb);
OPCODE_PROTO(lodsw);
OPCODE_PROTO(scasb);
OPCODE_PROTO(scasw);
OPCODE_PROTO(mov_gpr_imm);
OPCODE_PROTO(retn);
OPCODE_PROTO(retn_imm);
OPCODE_PROTO(les);
OPCODE_PROTO(lds);
OPCODE_PROTO(mov_rm_imm);
OPCODE_PROTO(retf_imm);
OPCODE_PROTO(retf);
OPCODE_PROTO(interrupt);
OPCODE_PROTO(iret);
OPCODE_PROTO(group_2);
OPCODE_PROTO(aam);
OPCODE_PROTO(aad);
OPCODE_PROTO(xlat);
OPCODE_PROTO(loop);
OPCODE_PROTO(jrcxz);
OPCODE_PROTO(inb_imm);
OPCODE_PROTO(inw_imm);
OPCODE_PROTO(outb_imm);
OPCODE_PROTO(outw_imm);
OPCODE_PROTO(call_near);
OPCODE_PROTO(jmp_near);
OPCODE_PROTO(jmp_far);
OPCODE_PROTO(jmp_short);
OPCODE_PROTO(inb_dx);
OPCODE_PROTO(inw_dx);
OPCODE_PROTO(outb_dx);
OPCODE_PROTO(outw_dx);
OPCODE_PROTO(set_flag);
OPCODE_PROTO(group_3);
OPCODE_PROTO(group_4);
OPCODE_PROTO(group_5);

//386 PROTOS
OPCODE_PROTO(push_all);
OPCODE_PROTO(pop_all);
OPCODE_PROTO(bound);
OPCODE_PROTO(push_imm16_32);
OPCODE_PROTO(push_imm8);
OPCODE_PROTO(imul_reg_reg_imm);
OPCODE_PROTO(imul_reg_reg_imm8);
OPCODE_PROTO(ins8);
OPCODE_PROTO(ins);
OPCODE_PROTO(outs8);
OPCODE_PROTO(outs);
OPCODE_PROTO(enter);
OPCODE_PROTO(leave);
//386 2_bytes_protos 0x0f Prefix
OPCODE_PROTO(two_byte_0fh);
OPCODE_PROTO(jcc_l);
OPCODE_PROTO(setcc);
OPCODE_PROTO(push_fs);
OPCODE_PROTO(pop_fs);
OPCODE_PROTO(push_gs);
OPCODE_PROTO(pop_gs);



#endif //MICROS_OPCODES_H
