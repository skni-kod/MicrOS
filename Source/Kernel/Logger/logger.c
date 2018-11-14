#include "logger.h"

void log_info(char* content)
{
    logger_print_on_screen(content, " INFO ", VGA_COLOR_WHITE);
}

void log_ok(char* content)
{
    logger_print_on_screen(content, "  OK  ", VGA_COLOR_GREEN);
}

void log_warning(char* content)
{
    logger_print_on_screen(content, " WARN ", VGA_COLOR_YELLOW);
}

void log_error(char* content)
{
    logger_print_on_screen(content, " FUCK ", VGA_COLOR_LIGHT_RED);
}

void log_critical(char* content)
{
    logger_print_on_screen(content, " PIGI ", VGA_COLOR_RED);
    __asm__ volatile ( "hlt" );
}

void logger_print_on_screen(char* message, char* id, uint8_t id_color)
{
    vga_color col;
    col.color_without_blink.letter = id_color;
    col.color_without_blink.background = VGA_COLOR_BLACK;
    
    vga_printstring("[");
    vga_printstring_color(id, &col);
    vga_printstring("] ");
    vga_printstring(message);
    vga_printstring("\n");
}