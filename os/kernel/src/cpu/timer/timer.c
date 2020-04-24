#include "timer.h"

uint32_t system_clock = 0;
uint32_t system_clock_fractions = 0;

uint16_t reload_time = 0;
uint32_t expected_clock_fraction = 0;

void timer_init()
{
    pic_enable_irq(0);
    idt_attach_interrupt_handler(0, timer_interrupt);

    // | CHA0 | CHA1 | ACM0 | ACM1 | OPE0 | OPE1 | OPE2 | BCD  |
    // |  0   |  0   |  1   |  1   |  0   |  1   |  0   |  0   | = 0x34
    // CHA0, CHA1 - selected channel
    //  00 - channel 0
    //  01 - channel 1
    //  10 - channel 2
    //  11 - read-back command
    // ACM0, ACM1 - access mode
    //  00 - latch count value command
    //  01 - low byte only
    //  10 - high byte only
    //  11 - low byte and high byte
    // OPE0, OPE1, OPE2 - operating mode
    //  000 - mode 0 - interrupt on terminal count
    //  001 - mode 1 - hardware re-triggerable one-shot
    //  010 - mode 2 - rate generator
    //  011 - mode 3 - square wave generator
    //  100 - mode 4 - software triggered strobe
    //  101 - mode 5 - hardware triggered strobe
    // BCD - use BCD format or binary
    io_out_byte(MODE_COMMAND_REGISTER, 0x36);

    reload_time = timer_get_reload_value();
    expected_clock_fraction = timer_get_expected_clock_fraction();

    io_out_byte(CHANNEL0_PORT, reload_time & 0xFF);
    io_out_byte(CHANNEL0_PORT, reload_time >> 8);
}

uint16_t timer_get_reload_value()
{
    // We want interrupt every 1 ms (1000 Hz)
    return TIMER_FREQUENCY_HZ / TARGET_FREQUENCY;
}

uint32_t timer_get_expected_clock_fraction()
{
    return TIMER_FREQUENCY_HZ - (reload_time * TARGET_FREQUENCY);
}

uint32_t timer_get_system_clock()
{
    return system_clock;
}

bool timer_interrupt()
{
    system_clock++;
    system_clock_fractions += expected_clock_fraction;

    if (system_clock_fractions > TARGET_FREQUENCY)
    {
        system_clock++;
        system_clock_fractions -= TARGET_FREQUENCY;
    }
    
    return false;
}