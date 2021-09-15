#include <micros.h>
#include <micros/micros_console.h>
#include <micros/micros_generic_vga.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "util/bmp.h"

#define GPU_BUFFER_SIZE 320*200

typedef uint8_t byte;

byte* gpuBuffer;

char partition;

void swap_buffers(byte* buffer)
{
    while (micros_generic_vga_is_vretrace());
    while (!micros_generic_vga_is_vretrace());
    micros_console_copy_from_buffer(buffer, GPU_BUFFER_SIZE);
}

int main(int argc, char *argv[])
{
    partition = argv[1][0];

    micros_console_set_video_mode(0x13);
    
    gpuBuffer = (byte*)calloc(GPU_BUFFER_SIZE , sizeof(byte));

    for(int i = 0; i < GPU_BUFFER_SIZE; i++)
    {
       *(gpuBuffer+i) = i%256;
    }
    swap_buffers(gpuBuffer);

    byte* playerSprite = (byte*)loadBMPToByteArray("/GG/IMG/ANIMU.BMP");

    for(int i = 0; i < GPU_BUFFER_SIZE; i++)
    {
        *(gpuBuffer+i) = playerSprite[i];
    }
    swap_buffers(gpuBuffer);

    while(1);


    return 0;
}