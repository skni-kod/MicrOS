#ifndef LOGGER_H
#define LOGGER_H

#include "../Drivers/VGA/vga.h"

void log_info(char* content);
void log_ok(char* content);
void log_warning(char* content);
void log_error(char* content);
void log_critical(char* content);
void logger_print_on_screen(char* message, char* id, uint8_t id_color);

#endif