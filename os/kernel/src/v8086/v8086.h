#ifndef V8086_H
#define V8086_H

#include <stdint.h>

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(p,m,v) (v ? bit_set(p,m) : bit_clear(p,m))

#define CARRY_FLAG_BIT 0u
#define PARITY_FLAG_BIT 2u
#define AUX_CARRY_FLAG_BIT 4u
#define ZERO_FLAG_BIT 6u
#define SIGN_FLAG_BIT 7u
#define INTERRUPT_FLAG_BIT 9u
#define DIRECTION_FLAG_BIT 10u
#define OVERFLOW_FLAG_BIT 11u

typedef enum _segment_register_select {
  ES, CS, SS, DS, FS, GS, DEFAULT
} segment_register_select;

typedef enum _repeat_prefix {
  NONE, REPNE, REP_REPE
} repeat_prefix;

enum instruction_set_compatibility{
    IS8086, IS386
};

enum BYTE_REGISTERS {AL=0, CL, DL, BL, AH, CH, DH, BH};
enum WORD_REGISTERS {AX=0, CX, DX, BX, SP, BP, SI, DI};
enum DWORD_REGISTERS {EAX=0, ECX, EDX, EBX, ESP, EBP, ESI, EDI};

typedef enum _machine_status {
    OK = 0,
    UNDEFINED_OPCODE = -1,
    UNDEFINED_REGISTER = -2,
    UNDEFINED_SEGMENT_REGISTER = -3,
    UNABLE_GET_MEMORY = -4,
    BAD_MOD_RM = -5,
    BAD_RM = -6,
    BAD_MOD = -7,
    BAD_REG = -8,
    BAD_WIDTH = -9,
    BAD_INDEX = -10,
    BAD_BASE = -11,
    UNDEFINED_RECALCULATED_OPCODE = -12,
    BAD_INT_NUMBER = -13,
    RELATIVE_JMP_OVERFLOW = -14,
    UNKNOWN_ERROR = -69
} machine_status;

struct DWORDREGS {
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t cflag;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;
  uint16_t eflags;
  uint32_t esp;
};

struct WORDREGS {
  uint16_t di, _upper_di;
  uint16_t si, _upper_si;
  uint16_t bp, _upper_bp;
  uint16_t cflag, _upper_cflag;
  uint16_t bx, _upper_bx;
  uint16_t dx, _upper_dx;
  uint16_t cx, _upper_cx;
  uint16_t ax, _upper_ax;
  uint16_t flags;
  uint16_t sp, _upper_sp;
};

struct BYTEREGS {
  uint16_t di, _upper_di;
  uint16_t si, _upper_si;
  uint16_t bp, _upper_bp;
  uint32_t cflag;
  uint8_t bl;
  uint8_t bh;
  uint16_t _upper_bx;
  uint8_t dl;
  uint8_t dh;
  uint16_t _upper_dx;
  uint8_t cl;
  uint8_t ch;
  uint16_t _upper_cx;
  uint8_t al;
  uint8_t ah;
  uint16_t _upper_ax;
  uint16_t flags;
  uint16_t sp, _upper_sp;
};

union REGS {		/* Compatible with DPMI structure, except cflag */
  struct DWORDREGS d;
  struct WORDREGS x;
  struct WORDREGS w;
  struct BYTEREGS h;
};

struct SREGS {
  uint16_t es;
  uint16_t ds;
  uint16_t fs;
  uint16_t gs;
  uint16_t cs;
  uint16_t ss;
};

struct DWORDIP{
    uint32_t eip;
};

struct WORDIP {
    uint16_t ip;
    uint16_t _upper_ip;
};

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
} _internal_state;

typedef struct _v8086
{
    union REGS regs;
    struct SREGS sregs;
    union IP IP;
    _internal_state internal_state;
    uint8_t	Memory[0x100000];
    enum instruction_set_compatibility is_compatibility;
    int16_t (*operations[256]) (struct _v8086*, uint8_t);
} v8086;


v8086* v8086_create_machine();
int16_t v8086_call_int(v8086* machine, int16_t num);

#endif