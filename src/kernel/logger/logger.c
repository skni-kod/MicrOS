#include "logger.h"

vga_screen_pos last_log_pos;

void logger_log_info(char *content)
{
    logger_print_on_screen(content, " INFO ", VGA_COLOR_WHITE);
}

void logger_log_ok(char *content)
{
    logger_print_on_screen(content, "  OK  ", VGA_COLOR_GREEN);
}

void logger_log_warning(char *content)
{
    logger_print_on_screen(content, " WARN ", VGA_COLOR_YELLOW);
}

void logger_log_error(char *content)
{
    logger_print_on_screen(content, " FUCK ", VGA_COLOR_LIGHT_RED);
}

void logger_replace_info(char *content)
{
    logger_replace_last_log(content, " INFO ", VGA_COLOR_WHITE);
}

void logger_replace_ok(char *content)
{
    logger_replace_last_log(content, "  OK  ", VGA_COLOR_GREEN);
}

void logger_replace_warning(char *content)
{
    logger_replace_last_log(content, " WARN ", VGA_COLOR_YELLOW);
}

void logger_replace_error(char *content)
{
    logger_replace_last_log(content, " FUCK ", VGA_COLOR_LIGHT_RED);
}

void logger_log_critical(char *content)
{
    logger_print_on_screen(content, " PIGI ", VGA_COLOR_RED);
    __asm__ volatile("hlt");
}

void logger_print_on_screen(char *message, char *id, uint8_t id_color)
{
    vga_color col;
    col.color_without_blink.letter = id_color;
    col.color_without_blink.background = VGA_COLOR_BLACK;

    vga_printstring("[");
    vga_printstring_color(id, &col);
    vga_printstring("] ");
    vga_printstring(message);
    vga_printstring("\n");
    last_log_pos = vga_get_cursor_pos();
    last_log_pos.y -= 1;
}

void logger_replace_last_log(char *message, char *id, uint8_t id_color)
{
    vga_color col;
    col.color_without_blink.letter = id_color;
    col.color_without_blink.background = VGA_COLOR_BLACK;

    vga_screen_pos _tmp_pos = vga_get_cursor_pos();

    vga_set_cursor_pos_struct(last_log_pos);
    vga_clearline();
    vga_printstring("[");
    vga_printstring_color(id, &col);
    vga_printstring("] ");
    vga_printstring(message);
    vga_set_cursor_pos_struct(_tmp_pos);
}