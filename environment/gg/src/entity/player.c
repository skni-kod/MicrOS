#include "player.h"
#include <micros.h>

#include "../util/screen.h"

extern rect* cameraRect;

extern uint32_t LEVEL_WIDTH;
extern uint32_t LEVEL_HEIGHT;

player* initPlayer(int32_t x, int32_t y, image* img, int32_t r, int32_t c, int32_t cf)
{
    player* playerPtr = (player*)calloc(1, sizeof(player));
    playerPtr->pos = (player*)calloc(1, sizeof(rect));
    playerPtr->tex = img;
    playerPtr->pos->x = x;
    playerPtr->pos->y = y;
    //Maybe add this to init code
    playerPtr->direction = 2;
    playerPtr->speed = 120;
    playerPtr->currentFrame = cf;
    playerPtr->animationSpeed = 0.2f;
    playerPtr->direction = 0;

    playerPtr->frames = (image_clip*)calloc(r*c, sizeof(image_clip));

    playerPtr->pos->w = playerPtr->tex->width / (float)c;
    playerPtr->pos->h = playerPtr->tex->height / (float)r;
    for(int i = 0; i < r*c; i++)
    {
        (playerPtr->frames+i)->x = playerPtr->pos->w * (i%c);
        (playerPtr->frames+i)->y = playerPtr->pos->h * (i/c);
        
        (playerPtr->frames+i)->w = playerPtr->pos->w;
        (playerPtr->frames+i)->h = playerPtr->pos->h;
    }

    playerPtr->animTime = 0.0f;
    playerPtr->maxAnimFrame = c;

    return playerPtr;
}

void tickPlayer(player* player)
{
    int8_t dx = 0, dy = 0;
    if(micros_keyboard_get_key_state(key_w)) dy = -1;
    else if(micros_keyboard_get_key_state(key_s)) dy = 1;

    if(micros_keyboard_get_key_state(key_a)) dx = -1;
    else if(micros_keyboard_get_key_state(key_d)) dx = 1;

    bool moving = false;
    if(dx == -1) {player->direction = 3; moving = true;}
    else if(dx == 1) {player->direction = 1; moving = true;}
    else if(dy == -1) {player->direction = 0; moving = true;}
    else if(dy == 1) {player->direction = 2; moving = true;}

    if(!dx && !dy) {player->currentFrame = 1; moving = false;}

    player->pos->x += dx * player->speed * FRAME_S;
    player->pos->y += dy * player->speed * FRAME_S;

    image_clip* cfPtr = player->frames+(player->direction*player->maxAnimFrame + player->currentFrame);
    //lock movement outside screen.
    if(player->pos->x < 0 ) player->pos->x = 0;
    if(player->pos->x > LEVEL_WIDTH - cfPtr->w) player->pos->x = LEVEL_WIDTH - cfPtr->w;
    
    if(player->pos->y < 0 ) player->pos->y = 0;
    if(player->pos->y > LEVEL_HEIGHT - cfPtr->h) player->pos->y = LEVEL_HEIGHT - cfPtr->h;

    if(moving)
    {
        if(player->animTime >= player->animationSpeed)
        {
            player->animTime = 0.0f;
            player->currentFrame = (player->currentFrame+1)%player->maxAnimFrame;
        }
        player->animTime += FRAME_S;
    }
    //Draw
    drawClippedTransparent(player->tex, player->pos->x - cameraRect->x, player->pos->y - cameraRect->y, cfPtr, 3);
}