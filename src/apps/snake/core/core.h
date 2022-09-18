#ifndef CORE_H
#define CORE_H

#define SCENES_COUNT 5

#include <micros.h>
#include "scenes/scene.h"
#include "scenes/menu/menu.h"
#include "scenes/game/game.h"
#include "scenes/exit/exit.h"
#include "scenes/about/about.h"
#include "scenes/highscores/highscores.h"

void core_run();

#endif