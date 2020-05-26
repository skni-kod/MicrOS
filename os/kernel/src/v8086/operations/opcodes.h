#ifndef MICROS_OPCODES_H
#define MICROS_OPCODES_H

#include "../v8086.h"

#define OPCODE_PROTO_NAME(name) v8086_opcode_##name
#define OPCODE_PROTO(name) int16_t OPCODE_PROTO_NAME(name)(v8086* machine, uint8_t opcode)
#define ASSIGN_NULL(i) machine->operations[i] = NULL
#define ASSIGN_OPCODE(i, name) machine->operations[i] = OPCODE_PROTO_NAME(name)
#define GROUP_OF_OPCODES(from, to, name) for(uint8_t i = from; i <= to; i++) ASSIGN_OPCODE(i, name)

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


#endif //MICROS_OPCODES_H
