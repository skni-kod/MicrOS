#ifndef PANIC_SCREEN_H
#define PANIC_SCREEN_H

#include <stdint.h>

void panic_screen_show(uint32_t code, const char *optString);

#endif