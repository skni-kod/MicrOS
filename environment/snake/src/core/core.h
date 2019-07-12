#ifndef CORE_H
#define CORE_H

#define SCENES_COUNT 5

#include <micros.h>
#include "scenes/scene.h"
#include "scenes/menu/menu.h"
#include "../board/board.h"
#include "../interface/interface.h"
#include "../timer/timer.h"

void core_run();

#endif