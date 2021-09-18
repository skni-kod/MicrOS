#include <micros.h>
#include <micros/micros_console.h>
#include <micros/micros_generic_vga.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "draw_utils/image.h"
#include "util/screen.h"

#include "entity/player.h"
#include "util/rect.h"

#define GPU_BUFFER_SIZE 320*200

typedef uint8_t byte;

byte* gpuBuffer;

rect* cameraRect;

uint32_t LEVEL_WIDTH;
uint32_t LEVEL_HEIGHT;

char partition;

void processInput()
{
    if(micros_keyboard_is_key_pressed())
    {
        micros_keyboard_scan_ascii_pair pressed_key;
        micros_keyboard_get_pressed_key(&pressed_key);
    }
}

void swap_buffers(byte* buffer, bool vsync)
{
    if(vsync)
    {
        while (micros_generic_vga_is_vretrace());
        while (!micros_generic_vga_is_vretrace());
    }
    micros_console_copy_from_buffer(buffer, GPU_BUFFER_SIZE);
}

void updateCamera(rect* cam, player* play)
{
    cam->x = (play->pos->x + play->pos->w/2 - cam->w/2);
    cam->y = (play->pos->y + play->pos->h/2 - cam->h/2);

    //TODO MAKE IT LEVEL DEPENDENT
    if(cam->x < 0) cam->x = 0;
    if(cam->x > LEVEL_WIDTH - SCREEN_WIDTH) cam->x = LEVEL_WIDTH - SCREEN_WIDTH;
    if(cam->y < 0) cam->y = 0;
    if(cam->y > LEVEL_HEIGHT - SCREEN_HEIGHT) cam->y = LEVEL_HEIGHT - SCREEN_HEIGHT;
}

int main(int argc, char *argv[])
{
    partition = argv[1][0];
    

    micros_console_set_video_mode(0x13);

    cameraRect = (rect*)calloc(1, sizeof(rect));
    cameraRect->x = 0;
    cameraRect->y = 0;
    cameraRect->w = SCREEN_WIDTH;
    cameraRect->h = SCREEN_HEIGHT;

    
    gpuBuffer = (byte*)calloc(GPU_BUFFER_SIZE , sizeof(byte));

    for(int i = 0; i < GPU_BUFFER_SIZE; i++)
    {
       *(gpuBuffer+i) = i%256;
    }
    swap_buffers(gpuBuffer, true);

    //image*playerSprite = NULL;
    image* playerSprite = loadImage("/GG/IMG/CHARAS.LZ", true);
    image* background = loadImage("/GG/IMG/TILEMAP.LZ", true);
    LEVEL_WIDTH = background->width;
    LEVEL_HEIGHT = background->height;
    player* player = initPlayer(LEVEL_WIDTH/2, LEVEL_HEIGHT/2,playerSprite,4,3,1);

    uint32_t msStart, msEnd;
    while(1)
    {
        if(micros_keyboard_get_key_state(key_esc))
        {
            micros_console_set_video_mode(0x03);
            break;
        }
        msStart = micros_timer_get_system_clock();
        //Clean keybaord buffer
        processInput();
        //clear screen
        memset(gpuBuffer, 0, GPU_BUFFER_SIZE);
        //temporary
        draw(background, 0 - (int32_t)cameraRect->x , 0 - (int32_t)cameraRect->y);
        //Update and draw everything here

        tickPlayer(player);
        updateCamera(cameraRect, player);

        //End of update and draw!
        swap_buffers(gpuBuffer, true);
        msEnd = micros_timer_get_system_clock();
        
        
        micros_process_current_process_sleep(FRAME_MS - (msEnd - msStart));
    }

    //Cleanup
    free(gpuBuffer);
    free(background->data);
    free(background);
    free(playerSprite->data);
    free(playerSprite);

    //return to text mode
    micros_console_set_video_mode(0x03);

    return 0;
}