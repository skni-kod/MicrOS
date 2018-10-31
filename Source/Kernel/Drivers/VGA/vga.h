#pragma once
#define VGA_BASE_ADDR 0xB8000

void vga_printchar(char c);
void vga_printstring(char* str);