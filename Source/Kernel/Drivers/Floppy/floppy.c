#include "floppy.h"

floppy_header* floppy_header_data = 0x7c00;
uint8_t* dma_buffer = 0x0500;

void floppy_init()
{
    floppy_init_dma();
}

void floppy_init_dma()
{
    // Tell DMA that we want to configure floppy (channel 2)
    outb(0x0a, 0x06);

    // Reset Flip-Flop register
	outb(0xd8, 0xff);

    //address=0x1000 
	outb(0x04, 0);
	outb(0x04, 0x10);

    // Reset Flip-Flop register
	outb(0xd8, 0xff);

    // Count to 0x23ff (number of bytes in a 3.5" floppy disk track)
	outb(0x05, 0xff);
	outb(0x05, 0x23);

    // We don't want to have external page register
	outb(0x80, 0);
    
    // Release channel
	outb(0x0a, 0x02);
}

void floppy_dma_read() {
 
    // Tell DMA that we want to configure floppy (channel 2)
	outb(0x0a, 0x06);

	// | MOD1 | MOD0 | DOWN | AUTO | TRA1 | TRA0 | SEL1 | SEL0 |
    // |  0   |  1   |  0   |  1   |  0   |  1   |  1   |  0   | = 0x56
    // MOD0, MOD1 - mode
    //  00 - transfer on demand
    //  01 - single DMA transfer
    //  10 - block DMA transfer
    //  11 - cascade mode (with another DMA controller)
    // DOWN - set order of data
    // AUTO - reset address after transfer if set
    // TRA0, TRA1 - transfer type
    //  00 - self test of the controller
    //  01 - writing to memory
    //  10 - reading from memory
    //  11 - invalid
    // SEL0, SEL1 - channel to change
    outb(0x0b, 0x56); 

    // Release channel
	outb(0x0a, 0x02);
}