#ifndef V8086_H
#define V8086_H

#include <stdint.h>

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
  unsigned short es;
  unsigned short ds;
  unsigned short fs;
  unsigned short gs;
  unsigned short cs;
  unsigned short ss;
};

typedef struct _v8086
{
    union REGS regs;
    struct SREGS sregs;
    uint16_t IP;
    uint8_t	Memory[0x100000];
} v8086;


v8086* v8086_create_machine();
int16_t v8086_call_int(v8086* machine, int16_t num)

#endif