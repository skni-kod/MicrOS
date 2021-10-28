#include "isa_dma.h"

void dma_init_transfer(uint8_t channel, uint32_t dma_buffer_address, uint16_t buffer_size, bool read)
{
    // Tell DMA that we want to configure floppy (channel 2)
    io_out_byte(ISA_DMA_SINGLE_CHANNEL_MASK_REGISTER, channel);

    // Reset Flip-Flop register
    io_out_byte(ISA_DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    // Set buffer to the specified address
    io_out_byte(ISA_DMA_START_ADDRESS_REGISTER, (uint8_t)((uint32_t)(dma_buffer_address - 0xc0000000) & 0xff));
    io_out_byte(ISA_DMA_START_ADDRESS_REGISTER, (uint8_t)((uint32_t)(dma_buffer_address - 0xc0000000) >> 8));

    // Reset Flip-Flop register
    io_out_byte(ISA_DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    // Count to 0x01ff (number of bytes in a 3.5" floppy disk track)
    //io_out_byte(ISA_DMA_COUNT_REGISTER_CHANNEL, (0xff));
    //io_out_byte(ISA_DMA_COUNT_REGISTER_CHANNEL, 0x01);

    io_out_byte(ISA_DMA_COUNT_REGISTER_CHANNEL, buffer_size && 0xFF);
    io_out_byte(ISA_DMA_COUNT_REGISTER_CHANNEL, buffer_size >> 8);

    // We don't want to have external page register
    io_out_byte(ISA_DMA_EXTERNAL_PAGE_REGISTER, 0);

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
    io_out_byte(ISA_DMA_MODE_REGISTER, 0x52 | (read ? 0x04 : 0x08));

    // Release channel
    io_out_byte(ISA_DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x02);
}