#include "sb16_driver.h"
#include "../../assembly/io.h"
#include <ktime.h>
#include "cpu/timer/timer.h"

bool _is_present = false;

void sb16_init()
{
    uint8_t dupa;
    io_out_byte(SB16_DSP_RESET_PORT, 1);
    sleep(10);
    io_out_byte(SB16_DSP_RESET_PORT, 0);

    uint32_t before = timer_get_system_clock();;
    while(timer_get_system_clock() - before <= 2000)
    {
        if(io_in_byte(SB16_DSP_READ_STATUS_PORT) & 0x80) break;
    }

    before = timer_get_system_clock();
    while(timer_get_system_clock() - before <= 2000)
    {
        if(io_in_byte(SB16_DSP_READ_PORT) == 0xAA)
        {
            _is_present = true;
            break;
        }
    }

    if(_is_present)
    {
        idt_attach_interrupt_handler(0x10, sb16_interrupt_handler);
        sb16_write_mixer_register(0x80, 0b1000); //set IRQ 10
    }
}

bool sb16_is_present()
{
    return _is_present;
}

void sb16_write_dsp(uint8_t value)
{
    while(io_in_byte(SB16_DSP_READ_STATUS_PORT) & 0x80 != 0);
    io_out_byte(SB16_DSP_WRTIE_PORT, value);
}

uint8_t sb16_read_dsp()
{
    while(io_in_byte(SB16_DSP_READ_STATUS_PORT) & 0x80);
    return io_in_byte(SB16_DSP_READ_PORT);
}

void sb16_write_mixer_register(uint8_t index, uint8_t value)
{
    io_out_byte(SB16_DSP_MIXER_PORT, index);
    io_out_byte(SB16_DSP_MIXER_DATA_PORT, value);
}

uint8_t sb16_read_mixer_register(uint8_t index)
{
    io_out_byte(SB16_DSP_MIXER_PORT, index);
    return io_in_byte(SB16_DSP_MIXER_DATA_PORT);
}

bool sb16_interrupt_handler(interrupt_state* state)
{
    return false;
}