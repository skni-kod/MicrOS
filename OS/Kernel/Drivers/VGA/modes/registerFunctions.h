#ifndef REGISTERFUNCTIONS_H
#define REGISTERFUNCTIONS_H

#include <stdint.h>

#define VGA_VRAM (unsigned char*)0xC00A0000
#define VGA_VRAM_2 (unsigned char*)0xC00B8000

#define miscPort 0x3c2
#define crtcIndexPort 0x3d4
#define crtcDataPort 0x3d5
#define crtcIndexPortText 0x3b4
#define crtcDataPortText 0x3b5
#define sequencerIndexPort 0x3c4
#define sequencerDataPort 0x3c5
#define graphicsControllerIndexPort 0x3ce
#define graphicsControllerDataPort 0x3cf
#define attributeControllerIndexPort 0x3c0
#define attributeControllerReadPort 0x3c1
#define attributeControllerWritePort 0x3c0
#define attributeControllerResetPort 0x3da

#define peekb(S, O) *(unsigned char *)(16uL * (S) + (O))
#define pokeb(S, O, V) *(unsigned char *)(16uL * (S) + (O)) = (V)
#define pokew(S, O, V) *(unsigned short *)(16uL * (S) + (O)) = (V)

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(p,m,v) ((v) ? bit_set(p,m) : bit_clear(p,m))

void writeRegisters(uint8_t *registers);
void writeRegistersText(uint8_t *registers);
void readRegisters(unsigned char *regs);

void setFont(uint8_t *buf, uint8_t font_height);
void set_plane(unsigned p);
void set_vga_palette(unsigned char* palette);

static inline int32_t _abs(int32_t _x)
{
    return (_x < 0) ? _x * (-1) : _x;
}

#endif