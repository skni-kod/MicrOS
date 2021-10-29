#ifndef TILEMAP_H
#define TILEMAP_H

#include "../draw_utils/image.h"
#include "../util/rect.h"

#include "../stdafx.h"

/**
 * Structure that hold definition for tiles, it has everything needed to use tile in tilemap.
 * LIMITS:
 * * Tileset texture max of 2048x2048 pixels
 * * One tile of maximum 63x63 pixels
 * * maximum of 2048 tiles defined
 * * animations max 16 frames
 * * alignment to byte
 */

enum TMFLAGS
{
    TM_COLL = 0x01,
};

typedef struct tileID
{
    uint8_t isAnimated : 1;
    uint8_t isPlaying : 1;
    uint8_t hasCollider : 1;
    uint8_t reserved : 2;
    uint16_t tileset_id : 11;
} tileID;

typedef struct animated_tile
{
    uint16_t id;
    uint16_t next_id;
    float frameTime;
} animated_tile;

typedef struct tileset
{
    image* tilesetTex;
    uint16_t rows;
    uint16_t cols;
    uint8_t bgColor;
    image_clip* tile_clips;
    animated_tile* animations;
} tileset;

/**
 * Structure that holds information about tilemap.
 * wid
 */
typedef struct tilemap
{
    int width;     /** Width of tilemap in TILES */
    int height;    /** Height of tilemap in TILES */
    tileID* layers[8];
    tileset* ts;
    tileID invalidID;
    byte mode;
} tilemap;

typedef struct tile
{
    rect* pos;
    uint16_t id;
//Might not be needed, we'll see
//    rect* collider;
} tile;

tileset* buildTileset(char* filename, int rows, int cols, int cellW, int cellH, uint8_t bgColor);

tilemap* createTileMap(int width, int height, tileset* ts);

void drawMapForeground(tilemap* map, rect* camera, bool drawColliders);
void drawMapBackground(tilemap* map, rect* camera);

void animateTiles(tilemap* map);

void saveTilemap(tilemap* map, char* filename);

tilemap* loadTilemap(char* filename, tileset* ts);

#endif