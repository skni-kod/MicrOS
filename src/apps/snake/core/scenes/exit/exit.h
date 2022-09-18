#ifndef EXIT_H
#define EXIT_H

#include <micros.h>
#include <stdlib.h>
#include "../scene_type.h"

void exit_init();
void exit_input();
scene_type exit_logic();
void exit_draw();
void exit_delete();

#endif