#include "util.h"

#include <micros/micros_console.h>

void error(uint8_t code)
{
    micros_console_set_video_mode(0x03);
    exit(code);
}