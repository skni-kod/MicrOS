#ifndef FILE_STREAM_H
#define FILE_STREAM_H

#define CHUNK_SIZE 20480

#include <stdint.h>
#include "../../stdio.h"
#include "../../string.h"
#include "../../micros.h"

FILE *streams_set_stream_as_file(const char *filename, const char *mode, FILE *stream);
int streams_file_fetch(FILE *stream);
void streams_file_flush(FILE *stream);

#endif