#include "keyboard_stream.h"

FILE* streams_set_stream_as_keyboard(FILE* stream)
{
    stream->buffering_mode = file_buffering_mode_none;
    stream->fetch = streams_keyboard_fetch;
    stream->flush = streams_keyboard_flush;
}

void streams_keyboard_fetch(FILE* stream)
{
    bool processing = true;
    while(processing)
    {
        micros_keyboard_scan_ascii_pair key_pair;
        micros_keyboard_wait_for_key_press(&key_pair);
        
        switch (key_pair.ascii)
        {
            // Enter
            case 13:
            {
                processing = false;
                break;
            }
            
            // Backspace
            case 8:
            {
                micros_console_position cursor_position;
                micros_console_get_cursor_position(&cursor_position);
                
                cursor_position.x--;
                micros_console_set_cursor_position(&cursor_position);
                
                micros_console_print_char(' ');
                micros_console_set_cursor_position(&cursor_position);
                
                stream->buffer[pos--] = 0;
                break;
            }
            
            default:
            {
                micros_console_print_char(key_pair.ascii);
                stream->buffer[pos++] = key_pair.ascii;
            }
        }
    }
}

void streams_keyboard_flush(FILE* stream)
{
    // Keyboard isn't a valid device to write anything
}