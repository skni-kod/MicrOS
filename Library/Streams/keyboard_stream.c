#include "keyboard_stream.h"

FILE* streams_set_stream_as_keyboard(FILE* stream)
{
    stream->buffering_mode = file_buffering_mode_none;
    stream->fetch = streams_keyboard_fetch;
    stream->flush = streams_keyboard_flush;
}

void streams_keyboard_fetch(FILE* stream)
{
    
}

void streams_keyboard_flush(FILE* stream)
{
    
}