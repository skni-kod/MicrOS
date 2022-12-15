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

#include "util/keyboard.h"
#include "tile/tilemap.h"

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

void updateCameraGame(rect* cam, player* play)
{
    cam->x = (play->pos->x + play->pos->w/2 - cam->w/2);
    cam->y = (play->pos->y + play->pos->h/2 - cam->h/2);

    //TODO MAKE IT LEVEL DEPENDENT
    if(cam->x < 0) cam->x = 0;
    if(cam->x > LEVEL_WIDTH - SCREEN_WIDTH) cam->x = LEVEL_WIDTH - SCREEN_WIDTH;
    if(cam->y < 0) cam->y = 0;
    if(cam->y > LEVEL_HEIGHT - SCREEN_HEIGHT) cam->y = LEVEL_HEIGHT - SCREEN_HEIGHT;
}

static int fd;
static fpos_t pos;

char* sceneName = NULL;

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
            if(!compareString(argv[i], "-p"))
            {
                sceneName = argv[i+1];
                i++;
            }
        }
    }

    partition = argv[1][0];
    
    printf("Enter video mode\n");
    micros_console_set_video_mode(0x13);
    printf("After Enter video mode\n");

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

    if(editorMode)
    {
        runEditor(sceneName);
    }
    else
    {
        rect* camera = (rect*)calloc(1, sizeof(rect));
        camera->x = 0;
        camera->y = 0;
        camera->w = SCREEN_WIDTH;
        camera->h = SCREEN_HEIGHT;
        
        image* playerSprite = loadImage("/GG/CHARAS.ZIM", true);
        player* playerPtr = initPlayer(0, 0,playerSprite,4,3,1);
        tileset* mainTS = buildTileset("/GG/TILESET.ZIM", 22, 56, 32, 32, 0x0B);
        char mapName[128];
        if(sceneName == NULL)
        {
            sprintf(mapName, "/GG/SCENE1.MAP");
        }
        else
        {
            sprintf(mapName, "/GG/%s.MAP", sceneName);
        }
        tilemap* map = loadTilemap(mapName, mainTS);

        LEVEL_WIDTH = map->width * 32;
        LEVEL_HEIGHT = map->height * 32;

        uint32_t msStart, msEnd;

        initInput();

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
            
            animateTiles(map);
            drawMapBackground(map, camera);
            tickPlayer(playerPtr, camera);
            updateCameraGame(camera, playerPtr);
            drawMapForeground(map, camera, false);

            //End of update and draw!
            swap_buffers(gpuBuffer, true);
            msEnd = micros_timer_get_system_clock();
            if (msEnd - msStart < FRAME_MS) micros_process_current_process_sleep(FRAME_MS - (msEnd - msStart));
        }
    }

    micros_console_set_video_mode(0x03);

    return 0;
}