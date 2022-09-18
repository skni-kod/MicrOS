#ifndef V8086_H
#define V8086_H

#include <stdint.h>
#include <stdbool.h>
#include "../cpu/timer/timer.h"

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(p,m,v) ((v) ? bit_set(p,m) : bit_clear(p,m))

#define CARRY_FLAG_BIT 0u
#define PARITY_FLAG_BIT 2u
#define AUX_CARRY_FLAG_BIT 4u
#define ZERO_FLAG_BIT 6u
#define SIGN_FLAG_BIT 7u
#define INTERRUPT_FLAG_BIT 9u
#define DIRECTION_FLAG_BIT 10u
#define OVERFLOW_FLAG_BIT 11u

//#define DEBUG_V8086
//#define TEST_V8086

typedef enum _segment_register_select {
  V8086_ES=0, V8086_CS, V8086_SS, V8086_DS, V8086_FS, V8086_GS, V8086_DEFAULT
} segment_register_select;

typedef enum _repeat_prefix {
  V8086_NONE_REPEAT, V8086_REPNE, V8086_REP_REPE
} repeat_prefix;

enum instruction_set_compatibility{
    V8086_IS8086, V8086_IS386
};

enum BYTE_REGISTERS {V8086_AL=0, V8086_CL, V8086_DL, V8086_BL, V8086_AH, V8086_CH, V8086_DH, V8086_BH};
enum WORD_REGISTERS {V8086_AX=0, V8086_CX, V8086_DX, V8086_BX, V8086_SP, V8086_BP, V8086_SI, V8086_DI};
enum DWORD_REGISTERS {V8086_EAX=0, V8086_ECX, V8086_EDX, V8086_EBX, V8086_ESP, V8086_EBP, V8086_ESI, V8086_EDI};

typedef enum _machine_status {
    V8086_OK = 0,
    V8086_UNDEFINED_OPCODE = -1,
    V8086_UNDEFINED_REGISTER = -2,
    V8086_UNDEFINED_SEGMENT_REGISTER = -3,
    V8086_UNABLE_GET_MEMORY = -4,
    V8086_BAD_MOD_RM = -5,
    V8086_BAD_RM = -6,
    V8086_BAD_MOD = -7,
    V8086_BAD_REG = -8,
    V8086_BAD_WIDTH = -9,
    V8086_BAD_INDEX = -10,
    V8086_BAD_BASE = -11,
    V8086_UNDEFINED_RECALCULATED_OPCODE = -12,
    V8086_BAD_INT_NUMBER = -13,
    V8086_RELATIVE_JMP_OVERFLOW = -14,
    V8086_BOUND_ERROR = -15,
    V8086_DIVISION_BY_ZERO = -16,
    V8086_DIVISION_OVERFLOW = -17,
    V8086_DEBUG_FILE_OVERFLOW = -99,
    V8086_UNKNOWN_ERROR = -69
} machine_status;

struct DWORDREGS {
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t cflag;
    uint16_t eflags;

} __attribute__((packed));

struct WORDREGS {
    uint16_t ax, _upper_ax;
    uint16_t cx, _upper_cx;
    uint16_t dx, _upper_dx;
    uint16_t bx, _upper_bx;
    uint16_t sp, _upper_sp;
    uint16_t bp, _upper_bp;
    uint16_t si, _upper_si;
    uint16_t di, _upper_di;
    uint16_t cflag, _upper_cflag;
    uint16_t flags;

} __attribute__((packed));

struct BYTEREGS {
    uint8_t al;
    uint8_t ah;
    uint16_t _upper_ax;
    uint8_t cl;
    uint8_t ch;
    uint16_t _upper_cx;
    uint8_t dl;
    uint8_t dh;
    uint16_t _upper_dx;
    uint8_t bl;
    uint8_t bh;
    uint16_t _upper_bx;
    uint16_t sp, _upper_sp;
    uint16_t bp, _upper_bp;
    uint16_t si, _upper_si;
    uint16_t di, _upper_di;
    uint32_t cflag;
    uint16_t flags;

} __attribute__((packed));

union REGS {		/* Compatible with DPMI structure, except cflag */
  struct DWORDREGS d;
  struct WORDREGS x;
  struct WORDREGS w;
  struct BYTEREGS h;
};

struct SREGS {
  uint16_t es;
  uint16_t cs;
  uint16_t ss;
  uint16_t ds;
  uint16_t fs;
  uint16_t gs;
} __attribute__((packed));

struct DWORDIP{
    uint32_t eip;
} __attribute__((packed));

struct WORDIP {
    uint16_t ip;
    uint16_t _upper_ip;
} __attribute__((packed));

union IP {
    struct DWORDIP d;
    struct WORDIP w;
    struct WORDIP x;
};

typedef struct _is{
  uint32_t operand_32_bit;
  uint32_t address_32_bit;
  segment_register_select segment_reg_select;
  repeat_prefix rep_prefix;
  uint16_t IPOffset;
  uint8_t previous_byte_was_prefix;
}__attribute__((packed)) _internal_state;

typedef struct _v8086
{
    union REGS regs;
    struct SREGS sregs;
    union IP IP;
    _internal_state internal_state;
    uint8_t	Memory[0x100000];
    enum instruction_set_compatibility is_compatibility;
    int16_t (*operations[256]) (struct _v8086*, uint8_t);
    int16_t (*operations_0fh[256]) (struct _v8086*, uint8_t);

}__attribute__((packed)) v8086;


v8086* v8086_create_machine();
void v8086_destroy_machine(v8086* machine);
int16_t v8086_call_int(v8086* machine, int16_t num);
void v8086_set_8086_instruction_set(v8086* machine);
void v8086_set_386_instruction_set(v8086* machine);
uint32_t v8086_get_address_of_int(v8086* machine, int16_t num);
int16_t v8086_call_com_program(v8086* machine, char* programPath);

#ifdef DEBUG_V8086
  void send_reg_32(uint32_t reg);
  void send_reg_16(uint16_t reg);
  void send_regs(v8086* machine);
  void send_sregs(v8086* machine);
  int16_t parse_and_execute_instruction(v8086* machine);
  uint8_t read_reg_8();
  uint16_t read_reg_16();
  uint32_t read_reg_32();
  void read_regs(v8086* machine);
  void read_sregs(v8086* machine);
  void setSkipDebugging(bool value);
#endif
#endif