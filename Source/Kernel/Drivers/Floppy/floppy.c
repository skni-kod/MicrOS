#include "floppy.h"

floppy_header* floppy_header_data = 0x7c00;
uint8_t* dma_buffer = 0x0500;

bool floppy_interrupt_flag = false;

void floppy_init()
{
    floppy_dma_init();
    floppy_reset();
}

void floppy_dma_init()
{
    // Enable floppy interrupts
    pic_enable_irq(6);

    // Tell DMA that we want to configure floppy (channel 2)
    outb(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x06);

    // Reset Flip-Flop register
	outb(DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    // Set buffer to 0x0500 address
	outb(DMA_START_ADDRESS_REGISTER, 0);
	outb(DMA_START_ADDRESS_REGISTER, 0x05);

    // Reset Flip-Flop register
	outb(DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    // Count to 0x23ff (number of bytes in a 3.5" floppy disk track)
	outb(DMA_COUNT_REGISTER_CHANNEL, 0xff);
	outb(DMA_COUNT_REGISTER_CHANNEL, 0x23);

    // We don't want to have external page register
	outb(DMA_EXTERNAL_PAGE_REGISTER, 0);
    
    // Release channel
	outb(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x02);
}

void floppy_reset()
{
    // Disable floppy controller
    outb(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0);

    // Enable floppy controller (reset (0x04) | DMA (0x08))
    outb(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0x0C);

    // Wait for interrupt and continue reset sequence
    floppy_wait_for_interrupt();
}

void floppy_dma_read() {
 
    // Tell DMA that we want to configure floppy (channel 2)
	outb(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x06);

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
    outb(DMA_MODE_REGISTER, 0x56); 

    // Release channel
	outb(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x02);
}

void floppy_wait_for_interrupt()
{
    while(!floppy_interrupt_flag);
    floppy_interrupt_flag = false;
}

void floppy_interrupt()
{
    floppy_interrupt_flag = true;
}