#ifndef GAME_H
#define GAME_H

#include <micros.h>
#include "splashscreens.h"
#include "../board/board.h"
#include "../interface/interface.h"
#include "../timer/timer.h"

void game_run();
void game_display_lost_splashscreen();

#endif