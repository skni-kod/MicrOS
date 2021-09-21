#include "sb16_driver.h"
#include "../../assembly/io.h"
#include <ktime.h>
#include "cpu/timer/timer.h"

bool _is_present = false;

void sb16_init()
{
    uint8_t dupa;
    io_out_byte(SB16_DSP_RESET_PORT, 1);
    sleep(1);
    io_out_byte(SB16_DSP_RESET_PORT, 0);

    uint32_t before = timer_get_system_clock();
    while(timer_get_system_clock() - before <= 200)
    {
        if(io_in_byte(SB16_DSP_READ_STATUS_PORT) & 0x80) break;
    }

    before = timer_get_system_clock();
    while(timer_get_system_clock() - before <= 200)
    {
        dupa = io_in_byte(SB16_DSP_READ_PORT);
        if(dupa == 0xAA)
        {
            _is_present = true;
            break;
        }
    }
    while(1);
}

bool sb16_is_present()
{
    return _is_present;
}