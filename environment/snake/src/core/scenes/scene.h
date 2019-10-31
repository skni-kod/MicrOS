#ifndef SCENE_H
#define SCENE_H

#include "scene_type.h"

typedef struct scene
{
    scene_type type;
    
    void (*scene_init)();
    void (*scene_input)();
    scene_type (*scene_logic)();
    void (*scene_draw)();
    void (*scene_delete)();
} scene;

#endif