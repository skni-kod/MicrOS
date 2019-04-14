#ifndef CONSOLE_STREAM_H
#define CONSOLE_STREAM_H

#include <stdint.h>
#include "../../stdio.h"
#include "../../micros.h"

FILE *streams_set_stream_as_console(FILE *stream);
int streams_console_fetch(FILE *stream);
void streams_console_flush(FILE *stream);

#endif