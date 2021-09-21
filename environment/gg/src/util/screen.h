#ifndef SCREEN_H
#define SCREEN_H

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define GPU_BUFFER_SIZE SCREEN_WIDTH * SCREEN_HEIGHT

#define FPS 60
#define FRAME_MS 1000 / FPS
#define FRAME_S 1.0 / FPS

#include "../stdafx.h"

void swap_buffers(byte* buffer, bool vsync);

#endif