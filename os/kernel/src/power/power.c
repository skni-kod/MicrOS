#include "power.h"

void power_reboot()
{
    uint8_t good = 0x02;
    while (good & 0x02)
    {
        good = io_in_byte(0x64);
    }
    
    io_out_byte(0x64, 0xFE);
    __asm__ ( "hlt" );
}

void power_shutdown()
{
    // Bochs workaround
    io_out_word(0xB004, 0x2000);
    
    // QEMU workaround
    io_out_word(0x604, 0x2000);
    
    // VirtualBox workaround
    io_out_word(0x4004, 0x3400);
}