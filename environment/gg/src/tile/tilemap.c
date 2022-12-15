#include "tilemap.h"

#include <stdlib.h>
#include <string.h>
#include "../util/file_utils.h"

#include "../util/screen.h"
#include "../draw_utils/draw_utils.h"

float tilemapUpdateTime = 0.1f;
float tilemapUpdateTimer = 0.0f;

tileset* buildTileset(char* filename, int rows, int cols, int cellW, int cellH, uint8_t bgColor)
{
    tileset* ts = (tileset*)calloc(1, sizeof(tileset));

    ts->tilesetTex = loadImage(filename, true);

    ts->cols = cols;
    ts->rows = rows;
    ts->bgColor = bgColor;

    ts->tile_clips = (image_clip*)malloc(rows*cols*sizeof(image_clip));

    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            image_clip* icPtr = (image_clip*)(ts->tile_clips+(r * cols + c));
            icPtr->x = c * cellW;
            icPtr->y = r * cellH;
            icPtr->w = cellW;
            icPtr->h = cellH;
        }
    }

    ts->animations = calloc(ts->cols*ts->rows, sizeof(animated_tile));

    //load animated tiles
    byte* animData = NULL;
    loadFile("/GG/ANIM.CBF", &animData, false);

    uint16_t entryNum = *(uint16_t*)animData;
    uint32_t dataOffset = 2;

    //Load animations
    for(int i = 0; i < entryNum; i++)
    {
        animated_tile* at = (animated_tile*)(animData+(dataOffset+i*sizeof(animated_tile)));
        memcpy(ts->animations+at->id, at, sizeof(animated_tile));
    }

    //Fill missing animation data.
    for(int i = 0; i < ts->rows*ts->cols; i++)
    {
        //this is workaround, don't have time to write proper handling
        animated_tile* at = ts->animations+i;
        if(at->id == 0)
        {
            at->id = i;
            at->next_id = 2047;
            at->frameTime = 0.0f;
        }
    }

    free(animData);

    return ts;
}

tilemap* createTileMap(int width, int height, tileset* ts)
{
    tilemap* tm = (tilemap*)calloc(1, sizeof(tilemap));
    tm->width = width;
    tm->height = height;
    tm->ts = ts;
    
    tm->invalidID.isAnimated = 0;
    tm->invalidID.isPlaying = 0;
    tm->invalidID.hasCollider = 0;
    tm->invalidID.reserved = 0;
    tm->invalidID.tileset_id = 2047;

    for(int i = 0; i < 8; i++)
    {
        tm->layers[i] = (tileID*)calloc(width*height, sizeof(tileID));
        for(int j = 0; j < width*height; j++)
        {
            *(tm->layers[i]+j) = tm->invalidID;
        }
    }
    return tm;
}

void drawMapBackground(tilemap* map, rect* camera)
{
    int xmin = (camera->x / 32) - 1;
    if(xmin < 0) xmin = 0;

    int xmax = ((camera->x+camera->w) / 32) + 1;
    if(xmax > map->height) xmax = map->width;

    int ymin = (camera->y / 32) - 1;
    if(ymin < 0) ymin = 0;

    int ymax = ((camera->y+camera->h) / 32) + 1;
    if(ymax > map->height) ymax = map->height;

    //speed up this whole thing to be as fast as possible, try to get real time values.
    //This is slow af, multiple calls, and sampling over and over same texture.
    for(int r = ymin; r < ymax; r++)
    {
        for(int c = xmin; c < xmax; c++)
        {
            int offset = r * map->width + c;
            if((map->layers[0]+offset)->tileset_id != 2047) drawClippedTransparent(map->ts->tilesetTex, c*32 - camera->x, r*32 - camera->y, &map->ts->tile_clips[(map->layers[0]+offset)->tileset_id], map->ts->bgColor, camera);   
            if((map->layers[1]+offset)->tileset_id != 2047) drawClippedTransparent(map->ts->tilesetTex, c*32 - camera->x, r*32 - camera->y, &map->ts->tile_clips[(map->layers[1]+offset)->tileset_id], map->ts->bgColor, camera);   
            if((map->layers[2]+offset)->tileset_id != 2047) drawClippedTransparent(map->ts->tilesetTex, c*32 - camera->x, r*32 - camera->y, &map->ts->tile_clips[(map->layers[2]+offset)->tileset_id], map->ts->bgColor, camera);   
            if((map->layers[3]+offset)->tileset_id != 2047) drawClippedTransparent(map->ts->tilesetTex, c*32 - camera->x, r*32 - camera->y, &map->ts->tile_clips[(map->layers[3]+offset)->tileset_id], map->ts->bgColor, camera);   
            if((map->layers[4]+offset)->tileset_id != 2047) drawClippedTransparent(map->ts->tilesetTex, c*32 - camera->x, r*32 - camera->y, &map->ts->tile_clips[(map->layers[4]+offset)->tileset_id], map->ts->bgColor, camera);   
            if((map->layers[5]+offset)->tileset_id != 2047) drawClippedTransparent(map->ts->tilesetTex, c*32 - camera->x, r*32 - camera->y, &map->ts->tile_clips[(map->layers[5]+offset)->tileset_id], map->ts->bgColor, camera);   
        }
    }
}

void drawMapForeground(tilemap* map, rect* camera, bool drawColliders)
{
    int xmin = (camera->x / 32) - 1;
    if(xmin < 0) xmin = 0;

    int xmax = ((camera->x+camera->w) / 32) + 1;
    if(xmax > map->height) xmax = map->width;

    int ymin = (camera->y / 32) - 1;
    if(ymin < 0) ymin = 0;

    int ymax = ((camera->y+camera->h) / 32) + 1;
    if(ymax > map->height) ymax = map->height;

    for(int r = ymin; r < ymax; r++)
    {
        for(int c = xmin; c < xmax; c++)
        {
            int offset = r * map->width + c;
            if((map->layers[6]+offset)->tileset_id != 2047) drawClippedTransparent(map->ts->tilesetTex, c*32 - camera->x, r*32 - camera->y, &map->ts->tile_clips[(map->layers[6]+offset)->tileset_id], map->ts->bgColor, camera);   
            if((map->layers[7]+offset)->tileset_id != 2047) drawClippedTransparent(map->ts->tilesetTex, c*32 - camera->x, r*32 - camera->y, &map->ts->tile_clips[(map->layers[7]+offset)->tileset_id], map->ts->bgColor, camera);   
            
            if((map->layers[7]+offset)->hasCollider == true && map->mode == 1) 
            {
                rect col = {c*32, r*32, 32, 32};
                draw_fill_rect(&col, 0x04, camera);
            }
        }
    }
}


//This also seems to be kinda slow!
void animateTiles(tilemap* map)
{
    if(tilemapUpdateTimer >= tilemapUpdateTime)
    {
        tilemapUpdateTimer = 0.0f;
        for(int i = 0; i < map->width*map->height; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                if((map->layers[j]+i)->isAnimated && map->ts->animations[(map->layers[j]+i)->tileset_id].next_id != 2047)
                {
                    (map->layers[j]+i)->tileset_id = map->ts->animations[(map->layers[j]+i)->tileset_id].next_id;
                }
            }
        }
    }
    tilemapUpdateTimer += FRAME_S;
}

//TODO ADD FUCKING COLLISIONS
void saveTilemap(tilemap* map, char* filename)
{
    if(map == NULL) return;

    byte* tilemapData = (byte*)calloc(1, 65538);

    *(tilemapData) = map->width;
    *(tilemapData+1) = map->height;
    for(int i = 0; i < 8; i++)
    {
        memcpy(tilemapData + 2 + i*(map->width*map->height*sizeof(uint16_t)), map->layers[i], map->width*map->height*sizeof(uint16_t));
    }
    saveFile(filename, tilemapData, 65538, true);
}

tilemap* loadTilemap(char* filename, tileset* ts)
{
    byte* tilemapData = NULL;
    //it assumes file exists
    uint32_t loadedBytes = loadFile(filename, &tilemapData, true);
    
    tilemap* tm = (tilemap*)calloc(1, sizeof(tilemap));
    tm->ts = ts;
    tm->width = *tilemapData;
    tm->height = *(tilemapData + 1);
    tm->invalidID.tileset_id = 2047;
    for(int i = 0; i < 8; i++)
    {
        tm->layers[i] = (tileID*)malloc(tm->width*tm->height*sizeof(uint16_t));
        memcpy(tm->layers[i], tilemapData + 2 + i*(tm->width*tm->height*sizeof(uint16_t)), tm->width*tm->height*sizeof(uint16_t));
    }
    tm->mode = 0;
    return tm;
}