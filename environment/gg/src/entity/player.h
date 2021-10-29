#ifndef PLAYER_H
#define PLAYER_H

#include "../draw_utils/image.h"
#include "../util/rect.h"

typedef struct player
{
    rect* pos;
    int8_t direction;
    image* tex;
    image_clip* frames;
    int32_t speed;
    uint8_t currentFrame;
    uint8_t maxAnimFrame;
    float animationSpeed;
    float animTime;
} player;

player* initPlayer(int32_t x, int32_t y, image* img, int32_t r, int32_t c, int32_t cf);

void tickPlayer(player* player, rect* camera);


#endif