#include "vga_calls.h"

void syscall_vga_print_char(interrupt_state *state)
{
    vga_color color;
    color.color_without_blink.letter = state->registers.ecx;
    color.color_without_blink.background = state->registers.edx;

    vga_printchar_color(state->registers.ebx, &color);
}

void syscall_vga_print_string(interrupt_state *state)
{
    vga_color color;
    color.color_without_blink.letter = state->registers.ecx;
    color.color_without_blink.background = state->registers.edx;

    vga_printstring_color((const char *)state->registers.ebx, &color);
}

void syscall_vga_get_char_at_position(interrupt_state *state)
{
    state->registers.eax = vga_get_char(state->registers.ebx, state->registers.ecx);
}

void syscall_vga_set_cursor_position(interrupt_state *state)
{
    vga_set_cursor_pos(state->registers.ebx, state->registers.ecx);
}

void syscall_vga_get_cursor_position(interrupt_state *state)
{
    vga_screen_pos current_cursor_position = vga_get_cursor_pos();
    vga_screen_pos *passed_position_structure = (vga_screen_pos *)state->registers.ebx;

    passed_position_structure->x = current_cursor_position.x;
    passed_position_structure->y = current_cursor_position.y;
}

void syscall_vga_clear()
{
    vga_clear_screen();
}

void syscall_vga_set_cursor_visibility(interrupt_state *state)
{
    state->registers.ebx ? vga_cursor_on() : vga_cursor_off();
}