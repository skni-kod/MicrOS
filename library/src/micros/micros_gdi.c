#include "micros_gdi.h"

void micros_gdi_set_video_mode(uint16_t mode)
{
    micros_interrupt_1a(0x17, (uint32_t)mode);
}

int8_t micros_gdi_find_video_mode(video_mode_descriptor* vmd)
{
    return micros_interrupt_1a(0x1B, vmd);
}

bool micros_generic_vga_is_text_mode()
{
    return micros_interrupt_0a(0x18);
}

uint16_t micros_generic_vga_get_current_video_mode()
{
    return micros_interrupt_0a(0x19);
}

uint8_t micros_generic_vga_is_vretrace()
{
    return micros_interrupt_0a(0x1A);
}