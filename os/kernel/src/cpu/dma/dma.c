#include "dma.h"

uint32_t dma_buffer_address;

void dma_init(uint32_t buffer_address)
{
    dma_buffer_address = buffer_address;
}

void dma_init_transfer(uint8_t channel, bool read, uint16_t dataLen)
{
    // Tell DMA that we want to configure floppy (channel 2)
    io_out_byte(DMA_SINGLE_CHANNEL_MASK_REGISTER, channel);

    // Reset Flip-Flop register
    io_out_byte(DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    // Set buffer to the specified address
    io_out_byte(DMA_START_ADDRESS_REGISTER, (uint8_t)((uint32_t)(dma_buffer_address - 0xc0000000) & 0xff));
    io_out_byte(DMA_START_ADDRESS_REGISTER, (uint8_t)((uint32_t)(dma_buffer_address - 0xc0000000) >> 8));

    // Reset Flip-Flop register
    io_out_byte(DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    // Count to 0x01ff (number of bytes in a 3.5" floppy disk track)
    io_out_byte(DMA_COUNT_REGISTER_CHANNEL, ((dataLen-1) & 0xff));
    io_out_byte(DMA_COUNT_REGISTER_CHANNEL, (((dataLen-1) >> 8 ) & 0xff));

    // We don't want to have external page register
    io_out_byte(DMA_EXTERNAL_PAGE_REGISTER, 0);

    // | MOD1 | MOD0 | DOWN | AUTO | TRA1 | TRA0 | SEL1 | SEL0 |
    // |  0   |  1   |  0   |  1   |  x   |  x   |  1   |  0   | = 0x56
    // MOD1, MOD0 - mode
    //  00 - transfer on demand
    //  01 - single DMA transfer
    //  10 - block DMA transfer
    //  11 - cascade mode (with another DMA controller)
    // DOWN - set order of data
    // AUTO - reset address after transfer if set
    // TRA1, TRA0 - transfer type
    //  00 - self test of the controller
    //  01 - reading from memory
    //  10 - writing to memory
    //  11 - invalid
    // SEL0, SEL1 - channel to change
    io_out_byte(DMA_MODE_REGISTER, 0x52 | (read ? 0x04 : 0x08));

    // Release channel
    io_out_byte(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x02);
}

uint8_t *dma_get_buffer()
{
    return (uint8_t *)dma_buffer_address;
}