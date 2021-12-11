#include "terminal_calls.h"

void syscall_terminal_print_char(interrupt_state *state)
{
    // if(!process_manager_is_current_process_active())
    // {
    //     return;
    // }
    
    vga_color color;
    color.color_without_blink.letter = state->registers.ecx;
    color.color_without_blink.background = state->registers.edx;

    //vga_printchar_color(state->registers.ebx, &color);
    terminal_manager_print_char_color(process_manager_get_current_process()->id, state->registers.ebx, color.value);
}

void syscall_terminal_print_string(interrupt_state *state)
{
    // if(!process_manager_is_current_process_active())
    // {
    //     return;
    // }
    
    vga_color color;
    color.color_without_blink.letter = state->registers.ecx;
    color.color_without_blink.background = state->registers.edx;

    // vga_printstring_color((const char *)state->registers.ebx, &color);
    terminal_manager_print_string_color(process_manager_get_current_process()->id, (const char*)state->registers.ebx, color.value);
}

void syscall_terminal_get_char_at_position(interrupt_state *state)
{
    //state->registers.eax = vga_get_char(state->registers.ebx, state->registers.ecx);
    
    char c;
    terminal_manager_get_char(process_manager_get_current_process()->id, state->registers.ebx, state->registers.ecx, &c);
    state->registers.eax = (uint32_t)c;
}

void syscall_terminal_set_cursor_position(interrupt_state *state)
{
    //if(!process_manager_is_current_process_active())
    //{
    //    return;
    //}
    
    //vga_set_cursor_pos(state->registers.ebx, state->registers.ecx);
    terminal_manager_set_cursor_pos(process_manager_get_current_process()->id, state->registers.ebx, state->registers.ecx);
}

void syscall_terminal_get_cursor_position(interrupt_state *state)
{
    // vga_screen_pos current_cursor_position = vga_get_cursor_pos();
    vga_screen_pos *passed_position_structure = (vga_screen_pos *)state->registers.ebx;

    // passed_position_structure->x = current_cursor_position.x;
    // passed_position_structure->y = current_cursor_position.y;
    
    terminal_manager_get_cursor_pos(process_manager_get_current_process()->id, &passed_position_structure->x, &passed_position_structure->y);
}

void syscall_terminal_clear()
{
    // if(!process_manager_is_current_process_active())
    // {
    //     return;
    // }
    
    // vga_clear_screen();
    terminal_manager_clear_screen(process_manager_get_current_process()->id);
}

void syscall_terminal_set_cursor_visibility(interrupt_state *state)
{
    // if(!process_manager_is_current_process_active())
    // {
    //     return;
    // }
    
    // state->registers.ebx ? vga_cursor_on() : vga_cursor_off();
    if (state->registers.ebx)
    {
        terminal_manager_turn_cursor_on(process_manager_get_current_process()->id);
    }
    else
    {
        terminal_manager_turn_cursor_off(process_manager_get_current_process()->id);
    }
}

void syscall_terminal_set_video_mode(interrupt_state *state)
{
    terminal_manager_set_mode(process_manager_get_current_process()->id, state->registers.ebx);
}

void syscall_terminal_copy_from_buffer(interrupt_state *state)
{
    terminal_manager_copy_from_buffer(process_manager_get_current_process()->id, (uint8_t*)(state->registers.ebx), state->registers.ecx);
}