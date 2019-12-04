#include "registerfunctions.h"
#include "assembly/io.h"
#include <string.h>

static unsigned get_fb_seg(void)
{
	unsigned seg;

	io_out_byte(graphicsControllerIndexPort, 6);
	seg = io_in_byte(graphicsControllerDataPort);
	seg >>= 2;
	seg &= 3;
	switch (seg)
	{
	case 0:
	case 1:
		seg = 0xC00A000;
		break;
	case 2:
		seg = 0xC00B000;
		break;
	case 3:
		seg = 0xC00B800;
		break;
	}
	return seg;
}

static void vmemwr(unsigned dst_off, unsigned char *src, unsigned count)
{
	//_vmemwr(get_fb_seg(), dst_off, src, count);
    memcpy((char *)(get_fb_seg()*0x10 + dst_off), src, count);
}

void writeRegisters(uint8_t *registers)
{
	io_out_byte(miscPort, *(registers));
	registers++;

	for (uint8_t i = 0; i < 5; i++)
	{
		io_out_byte(sequencerIndexPort, i);
		io_out_byte(sequencerDataPort, *(registers));
		registers++;
	}

	io_out_byte(crtcIndexPort, 0x03);
	io_out_byte(crtcDataPort, io_in_byte(crtcDataPort) | 0x80);
	io_out_byte(crtcIndexPort, 0x11);
	io_out_byte(crtcDataPort, io_in_byte(crtcDataPort) & ~0x80);

	registers[0x03] = registers[0x03] | 0x80;
	//registers[0x11] = registers[0x11] & ~0x80;

	for (uint8_t i = 0; i < 25; i++)
	{
		io_out_byte(crtcIndexPort, i);
		io_out_byte(crtcDataPort, *(registers));
		registers++;
	}

	for (uint8_t i = 0; i < 9; i++)
	{
		io_out_byte(graphicsControllerIndexPort, i);
		io_out_byte(graphicsControllerDataPort, *(registers));
		registers++;
	}

	for (uint8_t i = 0; i < 21; i++)
	{
		(void)io_in_byte(attributeControllerResetPort);
		io_out_byte(attributeControllerIndexPort, i);
		io_out_byte(attributeControllerWritePort, *(registers));
		registers++;
	}

	io_in_byte(attributeControllerResetPort);
	io_out_byte(attributeControllerIndexPort, 0x20);
}

void writeRegistersText(uint8_t *registers)
{
	io_out_byte(miscPort, *(registers));
	registers++;

	for (uint8_t i = 0; i < 5; i++)
	{
		io_out_byte(sequencerIndexPort, i);
		io_out_byte(sequencerDataPort, *(registers));
		registers++;
	}

	io_out_byte(crtcIndexPortText, 0x03);
	io_out_byte(crtcDataPortText, io_in_byte(crtcDataPortText) | 0x80);
	io_out_byte(crtcIndexPortText, 0x11);
	io_out_byte(crtcDataPortText, io_in_byte(crtcDataPortText) & ~0x80);

	registers[0x03] = registers[0x03] | 0x80;
	registers[0x11] = registers[0x11] & ~0x80;

	for (uint8_t i = 0; i < 25; i++)
	{
		io_out_byte(crtcIndexPort, i);
		io_out_byte(crtcDataPort, *(registers));
		registers++;
	}

	for (uint8_t i = 0; i < 9; i++)
	{
		io_out_byte(graphicsControllerIndexPort, i);
		io_out_byte(graphicsControllerDataPort, *(registers));
		registers++;
	}

	for (uint8_t i = 0; i < 21; i++)
	{
		(void)io_in_byte(attributeControllerResetPort);
		io_out_byte(attributeControllerIndexPort, i);
		io_out_byte(attributeControllerWritePort, *(registers));
		registers++;
	}

	io_in_byte(attributeControllerResetPort);
	io_out_byte(attributeControllerIndexPort, 0x20);
}

void readRegisters(unsigned char *regs)
{
        unsigned int i;
        unsigned char value;
        //MISC
        *regs = io_in_byte(0x3CC);
        regs++;
        //SEQ
        for(i = 0; i<5; i++)
        {
            	io_out_byte(sequencerIndexPort, i);
                value = io_in_byte(sequencerDataPort);
                *regs = value;
                regs++;
        }
        //CRTC
        for(i = 0; i< 25; i++)
        {
                io_out_byte(crtcIndexPort, i);
                *regs = io_in_byte(crtcDataPort);
                regs++;
        }
        //GC
        for(i = 0; i< 9; i++)
        {
                io_out_byte(graphicsControllerIndexPort, i);
                *regs = io_in_byte(graphicsControllerDataPort);
                regs++;
        }
        //AC
        for(i = 0; i< 21; i++)
        {

                (void)io_in_byte(attributeControllerResetPort);
                io_out_byte(attributeControllerIndexPort, i);
                *regs = io_in_byte(attributeControllerReadPort);
                regs++;
        }
        (void)io_in_byte(attributeControllerResetPort);
        io_out_byte(attributeControllerIndexPort, 0x20);
}

void setFont(uint8_t *buf, uint8_t font_height)
{
	unsigned char seq2, seq4, gc4, gc5, gc6;
	unsigned i;

	/* save registers
	set_plane() modifies GC 4 and SEQ 2, so save them as well */
	io_out_byte(sequencerIndexPort, 2);
	seq2 = io_in_byte(sequencerDataPort);

	io_out_byte(sequencerIndexPort, 4);
	seq4 = io_in_byte(sequencerDataPort);
	/* turn off even-odd addressing (set flat addressing)
	assume: chain-4 addressing already off */
	io_out_byte(sequencerDataPort, seq4 | 0x04);

	io_out_byte(graphicsControllerIndexPort, 4);
	gc4 = io_in_byte(graphicsControllerDataPort);

	io_out_byte(graphicsControllerIndexPort, 5);
	gc5 = io_in_byte(graphicsControllerDataPort);
	/* turn off even-odd addressing */
	io_out_byte(graphicsControllerDataPort, gc5 & ~0x10);

	io_out_byte(graphicsControllerIndexPort, 6);
	gc6 = io_in_byte(graphicsControllerDataPort);
	/* turn off even-odd addressing */
	io_out_byte(graphicsControllerDataPort, gc6 & ~0x02);
	/* write font to plane P4 */
	set_plane(2);
	/* write font 0 */
	for (i = 0; i < 256; i++)
	{
		vmemwr(16384u * 0 + i * 32, buf, font_height);
		buf += font_height;
	}

	// TODO: Check if this block is necessary.
	#if 0
	/* write font 1 */
	for(i = 0; i < 256; i++)
	{
		vmemwr(16384u * 1 + i * 32, buf, font_height);
		buf += font_height;
	}
	#endif
	/* restore registers */
	io_out_byte(sequencerIndexPort, 2);
	io_out_byte(sequencerDataPort, seq2);
	io_out_byte(sequencerIndexPort, 4);
	io_out_byte(sequencerDataPort, seq4);
	io_out_byte(graphicsControllerIndexPort, 4);
	io_out_byte(graphicsControllerDataPort, gc4);
	io_out_byte(graphicsControllerIndexPort, 5);
	io_out_byte(graphicsControllerDataPort, gc5);
	io_out_byte(graphicsControllerIndexPort, 6);
	io_out_byte(graphicsControllerDataPort, gc6);
}

void set_plane(unsigned p)
{
	unsigned char pmask;

	p &= 3;
	pmask = 1 << p;
	/* set read plane */
	io_out_byte(graphicsControllerIndexPort, 4);
	io_out_byte(graphicsControllerDataPort, p);
	/* set write plane */
	io_out_byte(sequencerIndexPort, 2);
	io_out_byte(sequencerDataPort, pmask);
}

void set_vga_palette(unsigned char* palette)
{
    io_out_byte(0x3C8, 0);
	for(int i = 0; i<768; i++)
	{
		io_out_byte(0x3C9, palette[i]);
	}
}