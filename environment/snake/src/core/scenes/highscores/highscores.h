#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <micros.h>
#include "../scene_type.h"

void highscores_init();
void highscores_input();
scene_type highscores_logic();
void highscores_draw();
void highscores_delete();

#endif