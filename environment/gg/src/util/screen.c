#include "screen.h"

#include <micros.h>
#include <micros/micros_console.h>
#include <micros/micros_generic_vga.h>

void swap_buffers(byte* buffer, bool vsync)
{
    if(vsync)
    {
        while (micros_generic_vga_is_vretrace());
        while (!micros_generic_vga_is_vretrace());
    }
    micros_console_copy_from_buffer(buffer, GPU_BUFFER_SIZE);
}