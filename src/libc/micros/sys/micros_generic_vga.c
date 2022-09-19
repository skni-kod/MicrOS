#include "micros_generic_vga.h"

void micros_generic_vga_set_video_mode(uint16_t mode)
{
#ifndef __libk 

    micros_interrupt_1a(0x0107, (uint32_t)mode);

#else

#endif
}

bool micros_generic_vga_is_text_mode()
{
#ifndef __libk 

    return micros_interrupt_0a(0x0108);

#else

#endif
}

uint16_t micros_generic_vga_get_current_video_mode()
{
#ifndef __libk 

    return micros_interrupt_0a(0x0109);

#else

#endif
}

uint8_t micros_generic_vga_is_vretrace()
{
#ifndef __libk 

    return micros_interrupt_0a(0x010A);

#else

#endif
}