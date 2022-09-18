#ifndef ABOUT_H
#define ABOUT_H

#include <micros.h>
#include <stdio.h>
#include "../scene_type.h"

void about_init();
void about_input();
scene_type about_logic();
void about_draw();
void about_delete();

#endif