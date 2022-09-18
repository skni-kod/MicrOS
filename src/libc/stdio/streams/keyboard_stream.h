#ifndef KEYBOARD_STREAM_H
#define KEYBOARD_STREAM_H

#include <stdint.h>
#include "../../stdio.h"
#include "../../micros.h"

FILE *streams_set_stream_as_keyboard(FILE *stream);
int streams_keyboard_fetch(FILE *stream);
void streams_keyboard_flush(FILE *stream);

#endif