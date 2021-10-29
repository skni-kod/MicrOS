#include "editor.h"
#include "../util/screen.h"

#include "../draw_utils/draw_utils.h"
#include <micros.h>
#include <micros/micros_console.h>
#include <micros/micros_generic_vga.h>

#include <string.h>
#include <stdio.h>

#include "../util/keyboard.h"

#include "../stdafx.h"

//ADD Edtior modes, player spawn point, colliders, etc.

extern byte* gpuBuffer;

//should these be here?
float cursorBlinkTime = 0.3f;
float cursorTimer = 0.0f;
bool drawCursor = true;

tileset* mainTS;
tilemap* map;

rect* camera;
rect* cursor;

//Cursor index on tilemap, NEEDED
int cix = 0, ciy = 0;

bool snapMode = true;

void runEditor()
{
    initInput();

    camera = (rect*)calloc(1, sizeof(rect));
    camera->x = 0;
    camera->y = 0;
    camera->w = 224;
    camera->h = SCREEN_HEIGHT;

    rect* editorbg = (rect*)malloc(sizeof(rect));
    editorbg->x = 224;
    editorbg->y = 0; 
    editorbg->w = 96;
    editorbg->h = 200; 
    uint32_t msStart, msEnd;

    cursor = (rect*)malloc(sizeof(rect));
    cursor->x = 0;
    cursor->y = 0;
    cursor->w = 32;
    cursor->h = 32;

    rect* tilePlaceholder = (rect*)malloc(sizeof(rect));
    tilePlaceholder->x = 283;
    tilePlaceholder->y = 25;
    tilePlaceholder->h = 32;
    tilePlaceholder->w = 32;

    //load editor assets:
    bitmap_font* mainFont = load_font("/GG/FONT.ZIM", 8, 14);

    //test?
    tileset* mainTS = buildTileset("/GG/TILESET.ZIM", 22, 56, 32, 32, 0x0B);
    //THIS SUCKS
    FILE* tmp = fopen("A:/GG/SCENE1.MAP", "r");
    if( tmp == 0)
    {
        map = createTileMap(64,64, mainTS);
    }
    else
    {
        map = loadTilemap("/GG/SCENE1.MAP", mainTS);
        fclose(tmp);
    }
    int tsX = 0, tsY = 0, layer = 0;
    uint8_t isAnimated = 0, isPlaying = 0;
    char tileInfo[64];
    char cameraInfo[64];
    char tileFlagsInfo[64];
    uint32_t lastTime = 0;
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

        draw_fill_rect(editorbg, 0x16, NULL);

        //still placeholder, but better view
        if(getKeyDown(key_pause))
        {
            tsX--;
            if(tsX < 0) tsX = 0;
        }

        if(getKeyDown(key_equal))
        { 
            tsX++;
            if(tsX > (mainTS->cols - 1)) tsX = mainTS->cols - 1;
        }
        
        if(getKeyDown(key_left_sq_bracket))
        {
            tsY--;
            if(tsY < 0) tsY = 0;
        }

        if(getKeyDown(key_right_sq_bracket))
        { 
            tsY++;
            if(tsY > (mainTS->rows - 1)) tsY = mainTS->rows - 1;
        }

        //animation
        if(getKeyDown(key_o))
        { 
            isAnimated ^= 1;
        }

        //play anim
        if(getKeyDown(key_p))
        { 
            isPlaying ^= 1;
        }

        //choosing layer
        if(getKeyDown(key_comma))
        {
            layer--;
            if(layer < 0) layer = 0;
        }
        if(getKeyDown(key_dot))
        {
            layer++;
            if(layer > 7) layer = 7;
        }

        if(getKeyDown(key_enter))
        {
            tileID id;
            id.isAnimated = isAnimated;
            id.isPlaying = isPlaying;
            id.reserved = 0;
            id.hasCollider = 0;
            id.tileset_id = tsY*mainTS->cols + tsX;
            placeTile(cix, ciy, layer, id, map);
        }

        if(getKeyDown(key_c))
        {
            insertCollider(cix, ciy, map);
        }

        if(getKeyDown(key_m)) map->mode ^= 1;

        if(getKeyDown(key_backspace))
        {
            deleteTile(cix, ciy, layer, map);
        }

        if(getKeyDown(key_y))
        {
            draw_text("SAVING...", 140, 5, mainFont, 0x0F);
            saveTilemap(map, "/GG/SCENE1.MAP");
        }

        if(getKeyDown(key_z))
        {
            for(int i = 0; i < 8; i++)
            {
                memset(map->layers[i], 0, map->width*map->height*sizeof(uint16_t));
            }
        }

        //render tilemap  (SLOW)
        //animateTiles(map);
        drawMapBackground(map, camera);
        drawMapForeground(map, camera, map->mode == 1);

        draw_fill_rect(tilePlaceholder, 0x1C, NULL);
        drawClippedTransparent(mainTS->tilesetTex, 283, 25, mainTS->tile_clips+(tsY*mainTS->cols + tsX), mainTS->bgColor, NULL);

        draw_text("EDITOR", 250, 5, mainFont, 0x0F);

        sprintf(tileInfo, "TX: %d\nTY: %d\nCT: %d", tsX, tsY, tsY*mainTS->cols + tsX);
        sprintf(cameraInfo, "CX: %d\nCY: %d", (int)camera->x, (int)camera->y);
        sprintf(tileFlagsInfo, "ANIM: %d, PLAY: %d\n\nLAYER: %d\nLAST TIME: %d", isAnimated, isPlaying, layer, lastTime);

        draw_text(tileInfo, 229, 25, mainFont, 0x0F);
        draw_text(tileFlagsInfo, 229, 58, mainFont, 0x0F);
        draw_text(cameraInfo, 229, 167, mainFont, 0x0F);

        tickCursor(cursor);

        //End of update and draw!
        swap_buffers(gpuBuffer, true);
        msEnd = micros_timer_get_system_clock();
        lastTime = (msEnd-msStart);
        if (msEnd - msStart < FRAME_MS) micros_process_current_process_sleep(FRAME_MS - (msEnd - msStart));
    }
}

void tickCursor()
{
    //This is waaay too fast, needs proper get key down logic
    if(getKeyDown(key_w)) ciy -= 1;
    if(getKeyDown(key_s)) ciy += 1;
    if(getKeyDown(key_d)) cix += 1;
    if(getKeyDown(key_a)) cix -= 1;

    if(cix > map->width - 1) cix = map->width - 1;
    if(cix < 0) cix = 0;
    if(ciy > map->height - 1) ciy = map->height - 1;
    if(ciy < 0) ciy = 0;
    
    cursor->x = cix * 32.0f;
    cursor->y = ciy * 32.0f;

    if(cursorTimer >= cursorBlinkTime)
    {
        cursorTimer = 0.0f;
        drawCursor ^= 1;
    }
    updateCameraEditor();
    if(drawCursor) draw_rect(cursor, 0x0F, camera);
    cursorTimer += FRAME_S;
}

void updateCameraEditor()
{
    if(snapMode)
    {
        if(cursor->x + cursor->w > camera->x + camera->w) camera->x += 32.0f;
        if(cursor->x < camera->x) camera->x -= 32.0f;
        if(cursor->y < camera->y) camera->y -= 32.0f;
        if(cursor->y + cursor->h > camera->y + camera->h) camera->y += 32.0f;
    }
}

void placeTile(int x, int y, int layer, tileID id, tilemap* map)
{
    *(map->layers[layer]+(y * map->width + x)) = id;
}

void deleteTile(int x, int y, int layer, tilemap* map)
{
    *(map->layers[layer]+(y * map->width + x)) = map->invalidID;
}

void insertCollider(int x, int y, tilemap* map)
{
    (map->layers[7]+(y * map->width + x))->hasCollider ^= 1;
}