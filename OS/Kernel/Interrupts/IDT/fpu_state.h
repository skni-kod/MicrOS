#ifndef FPU_STATE_H
#define FPU_STATE_H

typedef struct fpu_state
{
    uint32_t control_word;
    uint32_t status_word;
    uint32_t tag_word;
    uint32_t instruction_pointer_offset;
    uint16_t instruction_pointer_selector;
    uint16_t opcode;
    uint32_t operand_pointer_offset;
    uint32_t operand_pointer_selector;
    uint8_t registers[80];
} fpu_state;

#endif