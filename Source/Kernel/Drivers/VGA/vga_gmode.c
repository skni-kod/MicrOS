#include "vga_gmode.h"
#include "../../Assembly/io.h"

void writeRegisters(uint8_t* registers)
{
    outb(miscPort, *(registers++));

    for(uint8_t i=0; i<5; i++)
    {
        outb(sequencerIndexPort, i);
        outb(sequencerDataPort, *(registers++));
    }

    outb(crtcIndexPort, 0x03);
    outb(crtcDataPort, inb(crtcDataPort) | 0x80);
    outb(crtcIndexPort, 0x11);
    outb(crtcDataPort, inb(crtcDataPort) & ~0x80);

    registers[0x03] = registers[0x03] | 0x80;
    registers[0x11] = registers[0x11] & ~0x80;

    for(uint8_t i=0; i<25; i++)
    {
        outb(crtcIndexPort, i);
        outb(crtcDataPort, *(registers++));
    }

    for(uint8_t i=0; i<9; i++)
    {
        outb(graphicsControllerIndexPort, i);
        outb(graphicsControllerDataPort, *(registers++));
    }

    for(uint8_t i=0; i<21; i++)
    {
        inb(attributeControllerResetPort);
        outb(attributeControllerIndexPort, i);
        outb(attributeControllerWritePort, *(registers++));
    }

    inb(attributeControllerResetPort);
    outb(attributeControllerIndexPort, 0x20);
}

void set13HVideoMode()
{
    unsigned char g_320x200x256[] =
    {
        /* MISC */
            0x63,
        /* SEQ */
            0x03, 0x01, 0x0F, 0x00, 0x0E,
        /* CRTC */
            0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
            0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
            0xFF,
        /* GC */
            0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
            0xFF,
        /* AC */
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
            0x41, 0x00, 0x0F, 0x00, 0x00
    };

    writeRegisters(g_320x200x256);
}

#define VGA_VRAM 0xA0000
#define PITCH 320
void pixel_256 (unsigned char color, unsigned int x, unsigned int y) {
   unsigned char* fb       = (unsigned char*) VGA_VRAM;
   unsigned int offset     = y * PITCH + x;
   fb [offset] = color;
}

void drawDupaIn13H(int color)
{
    for(int x = 0; x<320; x++)
    {
        for(int y = 0; y<200; y++)
            pixel_256(color, x, y);
    }
}