#include <micros.h>
#include <micros/micros_console.h>
#include <micros/micros_generic_vga.h>
#include <stdlib.h>
#include <stdio.h>

#include "draw_utils/image.h"
#include "draw_utils/draw_utils.h"
#include "util/screen.h"

#include "entity/player.h"
#include "util/rect.h"

#include "editor/editor.h"

#include "stdafx.h"


byte* gpuBuffer;

rect* cameraRect;

uint32_t LEVEL_WIDTH;
uint32_t LEVEL_HEIGHT;

char partition;

//string.h is supposed to be bugged, so I implement my own strcmp
int compareString(char* a, char*b)
{
    if(a == NULL || b == NULL) return -2;
    int i = 0;
    while((a[i] != NULL) && (b[i] != NULL))
    {
        if(a[i] < b[i]) return -1;
        if(a[i] > b[i]) return 1;
        i++;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    uint8_t editorMode = 0;
    if(argc > 1)
    {
        for (int i = 1; i < argc; ++i)
        {
            if(argv[i] == NULL) continue;
            if(!compareString(argv[i], "-editor"))
            {
                editorMode = 1;
            }
        }
    }
    
    partition = argv[1][0];
    
    printf("Enter video mode\n");
    micros_console_set_video_mode(0x13);
    printf("After Enter video mode\n");

    cameraRect = (rect*)calloc(1, sizeof(rect));
    cameraRect->x = 0;
    cameraRect->y = 0;
    cameraRect->w = SCREEN_WIDTH;
    cameraRect->h = SCREEN_HEIGHT;

    gpuBuffer = (byte*)calloc(GPU_BUFFER_SIZE , sizeof(byte));

    //Init gpu buffer, so we see anything on screen when loading.
    for(int i = 0; i < GPU_BUFFER_SIZE; i++)
    {
       *(gpuBuffer+i) = i%256;
    }
    swap_buffers(gpuBuffer, true);

    //temp, just to be safe.
    LEVEL_WIDTH = 500;
    LEVEL_HEIGHT = 500;

    runEditor();

    // if(!editorMode)
    // {
    //     playerSprite = loadImage("/GG/CHARAS.ZIM", true);
    //     player = initPlayer(LEVEL_WIDTH/2, LEVEL_HEIGHT/2,playerSprite,4,3,1);
    // }

        // //We're not in editor mode, so launch normal game loop;
        // if(!editorMode)
        // {
        //     //run game logic
        //     //temporary
        //     draw(tileset, 0 - (int32_t)cameraRect->x , 0 - (int32_t)cameraRect->y);
        //     //Update and draw everything here

        //     tickPlayer(player);
        //     updateCamera(cameraRect, player);
        // }

    //return to text mode
    micros_console_set_video_mode(0x03);

    return 0;
}