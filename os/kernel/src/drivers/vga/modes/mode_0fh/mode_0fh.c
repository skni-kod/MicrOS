#include "mode_0fh.h"
#include "drivers/vga/modes/registerfunctions.h"
#include "memory/heap/heap.h"
#include "drivers/dal/videocard/videocard.h"

//REGISTER VALUES
unsigned char g_640x350x3[] =
	{
	/* MISC */
		0xA2,
	/* SEQ */
		0x03, 0x01, 0x0F, 0x00, 0x06,
	/* CRTC */
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF,
	/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05,
		0xFF,
	/* AC */
		0x00, 0x01, 0x00, 0x03, 0x18, 0x05, 0x00, 0x07,
		0x00, 0x09, 0x00, 0x0B, 0x00, 0x0D, 0x00, 0x0F,
		0x0B, 0x01, 0x05, 0x13, 0x00,
	};

//PALETTE
unsigned char palette0FH[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
    0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
    0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//DOUBLE BUFFER POINTER;

unsigned char *mode0fh_buffer[] = {NULL, NULL};
unsigned char buffer_turned_on_0Fh = 0;

int8_t mode0fh_set_mode()
{
    writeRegisters(g_640x350x3);
    set_vga_palette(palette0FH);
    mode0fh_clear_screen();
    video_card_set_turn_on_buffer_func(&mode0fh_turn_on_buffer);
    video_card_set_turn_off_buffer_func(&mode0fh_turn_off_buffer);
    video_card_set_is_buffer_on_func(&mode0fh_is_buffer_on);
    video_card_set_swap_buffers_func(&mode0fh_swap_buffers);
    video_card_set_draw_pixel_func(&mode0fh_draw_pixel);
    video_card_set_draw_line_func(&mode0fh_draw_line);
    video_card_set_draw_circle_func(&mode0fh_draw_circle);
    video_card_set_draw_rectangle_func(&mode0fh_draw_rectangle);
    video_card_set_clear_screen_func(&mode0fh_clear_screen);

    video_card_set_print_char_func(NULL);
	video_card_set_print_char_color_func(NULL);
	video_card_set_print_string_func(NULL);
	video_card_set_print_string_color_func(NULL);
	video_card_set_set_char_func(NULL);
	video_card_set_get_char_func(NULL);
	video_card_set_set_color_func(NULL);
	video_card_set_get_color_func(NULL);
	video_card_set_set_char_and_color_func(NULL);
	video_card_set_get_char_and_color_func(NULL);
	video_card_set_set_cursor_pos_func(NULL);
	video_card_set_get_cursor_pos_func(NULL);
	video_card_set_turn_cursor_on_func(NULL);
	video_card_set_turn_cursor_off_func(NULL);

    video_card_set_print_char_external_buffer(NULL);
	video_card_set_print_char_color_external_buffer(NULL);
	video_card_set_print_string_external_buffer(NULL);
	video_card_set_print_string_color_external_buffer(NULL);
	video_card_set_set_char_external_buffer(NULL);
	video_card_set_get_char_external_buffer(NULL);
	video_card_set_set_color_external_buffer(NULL);
	video_card_set_get_color_external_buffer(NULL);
	video_card_set_set_char_and_color_external_buffer(NULL);
	video_card_set_get_char_and_color_external_buffer(NULL);
    return 0x0F;
}

int8_t mode0fh_turn_on_buffer()
{
    if(buffer_turned_on_0Fh) return -1;
    for(int i = 1; i >= 0; i--)
    {
        mode0fh_buffer[i] = heap_kernel_alloc(MODE0FH_HEIGHT * MODE0FH_WIDTH / 8, 0);
        if(mode0fh_buffer[i] == NULL)
        {
            for(int j = 1; j >= i; j--)
            {
                heap_kernel_dealloc(mode0fh_buffer[j]);
                mode0fh_buffer[j] = NULL;
            }
            return -1;
        }
    }
    video_card_set_draw_pixel_func(&mode0fh_draw_pixel_buffered);
    video_card_set_draw_line_func(&mode0fh_draw_line_buffered);
    video_card_set_draw_circle_func(&mode0fh_draw_circle_buffered);
    video_card_set_draw_rectangle_func(&mode0fh_draw_rectangle_buffered);
    video_card_set_clear_screen_func(&mode0fh_clear_screen_buffered);
    buffer_turned_on_0Fh = 1;
    return 0;
}

int8_t mode0fh_turn_off_buffer()
{
    if(!buffer_turned_on_0Fh) return -1;
    for(int i = 1; i >= 0; i--)
    {
        heap_kernel_dealloc(mode0fh_buffer[i]);
        mode0fh_buffer[i] = NULL;
    }
    video_card_set_draw_pixel_func(&mode0fh_draw_pixel);
    video_card_set_draw_line_func(&mode0fh_draw_line);
    video_card_set_draw_circle_func(&mode0fh_draw_circle);
    video_card_set_draw_rectangle_func(&mode0fh_draw_rectangle);
    video_card_set_clear_screen_func(&mode0fh_clear_screen);
    buffer_turned_on_0Fh = 0;
    return 0;
}

uint8_t mode0fh_is_buffer_on()
{
    return buffer_turned_on_0Fh;
}

int8_t mode0fh_swap_buffers()
{
    if(!buffer_turned_on_0Fh) return -1;
    for(uint8_t p = 1; p < 2; p--)
    {
        set_plane(p*2);
        memcpy(VGA_VRAM, mode0fh_buffer[p], MODE0FH_WIDTH * MODE0FH_HEIGHT / 8);
    }
    //memcpy(VGA_VRAM, mode13h_buffer, MODE13H_HEIGHT * MODE13H_WIDTH);
    return 0;
}

int8_t mode0fh_draw_pixel(uint8_t color, uint16_t x, uint16_t y)
{
    if((x>=MODE0FH_WIDTH) || (y >=MODE0FH_HEIGHT))
        return -1;
    unsigned char *fb = (unsigned char *) VGA_VRAM;
    unsigned int offset = (y * MODE0FH_WIDTH + x)/8;
	unsigned bit_no = x % 8;
	for(char p = 1; p >= 0; p--)
	{
		set_plane(p*2);
		bit_write(fb[offset], 1<<(7-bit_no), (bit_get(color, 1 << p)));
	}
    return 0;
}

int8_t mode0fh_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    if(ax == bx) return -1;
    int32_t dx = (int32_t)bx - ax;
    int32_t dy = (int32_t)by - ay;
    if(_abs(dx) >= _abs(dy))
    {
        float a = dy/(float)(dx);
        float b = ay - a * ax;
        if(ax > bx)
            for(int x = bx; x <= ax; ++x)
                mode0fh_draw_pixel(color, x, a * x + b);
        else
            for(int x = ax; x <= bx; ++x)
                mode0fh_draw_pixel(color, x, a * x + b);
    }
    else
    {
        float a = dx/(float)(dy);
        float b = ax - a * ay;
        if(ay > by)
            for(int y = by; y <= ay; ++ y)
                mode0fh_draw_pixel(color, a * y + b, y);
        else
            for(int y = ay; y <= by; ++ y)
                mode0fh_draw_pixel(color, a * y + b, y);
    }
    return 0;
}

int8_t mode0fh_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius)
{
    return 0;
}

int8_t mode0fh_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return 0;
}
int8_t mode0fh_clear_screen()
{
    for(uint8_t p = 1; p < 2; p--)
    {
        set_plane(p * 2);
        memset(VGA_VRAM, 0, 64 * 1024);
    }
    return 0;
}

int8_t mode0fh_draw_pixel_buffered(uint8_t color, uint16_t x, uint16_t y)
{
    if((!buffer_turned_on_0Fh) || (x>=MODE0FH_WIDTH) || (y >=MODE0FH_HEIGHT))
        return -1;
    unsigned int offset = (y * MODE0FH_WIDTH + x)/8;
	unsigned bit_no = x % 8;
	for(uint8_t p = 1; p < 2; p--)
		bit_write(mode0fh_buffer[p][offset], 1<<(7-bit_no), (bit_get(color, 1 << p)));
    return 0;
}

int8_t mode0fh_draw_line_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    if(!buffer_turned_on_0Fh) return -1;
    if(ax == bx) return -1;
    int32_t dx = (int32_t)bx - ax;
    int32_t dy = (int32_t)by - ay;
    if(_abs(dx) >= _abs(dy))
    {
        float a = dy/(float)(dx);
        float b = ay - a * ax;
        if(ax > bx)
            for(int x = bx; x <= ax; ++x)
                mode0fh_draw_pixel_buffered(color, x, a * x + b);
        else
            for(int x = ax; x <= bx; ++x)
                mode0fh_draw_pixel_buffered(color, x, a * x + b);
    }
    else
    {
        float a = dx/(float)(dy);
        float b = ax - a * ay;
        if(ay > by)
            for(int y = by; y <= ay; ++ y)
                mode0fh_draw_pixel_buffered(color, a * y + b, y);
        else
            for(int y = ay; y <= by; ++ y)
                mode0fh_draw_pixel_buffered(color, a * y + b, y);
    }
    return 0;
}
int8_t mode0fh_draw_circle_buffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius)
{
    return 0;
}
int8_t mode0fh_draw_rectangle_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return 0;
}
int8_t mode0fh_clear_screen_buffered()
{
    if(!buffer_turned_on_0Fh) return -1;
    for(uint8_t p = 1; p < 2; p--)
        memset(mode0fh_buffer[p], 0, 64*1024);
    return 0;
}

int8_t mode0fh_draw_pixel_external_buffer(uint8_t* buffer, uint16_t mode, int8_t color, uint16_t x, uint16_t y){
    if((!buffer_turned_on_0Fh) || (x>=MODE0FH_WIDTH) || (y >=MODE0FH_HEIGHT))
        return -1;
    unsigned int offset = (y * MODE0FH_WIDTH + x)/8;
	unsigned bit_no = x % 8;
	for(uint8_t p = 1; p < 2; p--)
		bit_write(buffer[p * MODE0FH_HEIGHT * MODE0FH_WIDTH / 8 + offset], 1<<(7-bit_no), (bit_get(color, 1 << p)));
    return 0;
}
int8_t mode0fh_draw_line_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by){
    if(!buffer_turned_on_0Fh) return -1;
    if(ax == bx) return -1;
    int32_t dx = (int32_t)bx - ax;
    int32_t dy = (int32_t)by - ay;
    if(_abs(dx) >= _abs(dy))
    {
        float a = dy/(float)(dx);
        float b = ay - a * ax;
        if(ax > bx)
            for(int x = bx; x <= ax; ++x)
                mode0fh_draw_pixel_external_buffer(buffer, mode, color, x, a * x + b);
        else
            for(int x = ax; x <= bx; ++x)
                mode0fh_draw_pixel_external_buffer(buffer, mode, color, x, a * x + b);
    }
    else
    {
        float a = dx/(float)(dy);
        float b = ax - a * ay;
        if(ay > by)
            for(int y = by; y <= ay; ++ y)
                mode0fh_draw_pixel_external_buffer(buffer, mode, color, a * y + b, y);
        else
            for(int y = ay; y <= by; ++ y)
                mode0fh_draw_pixel_external_buffer(buffer, mode, color, a * y + b, y);
    }
    return 0;
}
int8_t mode0fh_draw_circle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t x, uint16_t y, uint16_t radius){
    return 0;
}
int8_t mode0fh_draw_rectangle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by){
    return 0;
}
int8_t mode0fh_clear_screen_external_buffer(uint8_t* buffer, uint16_t mode){
    memset(buffer, 0, 2*MODE0FH_WIDTH*MODE0FH_HEIGHT / 8);
    return 0;
}

int8_t mode0fh_swap_external_buffer(uint8_t* buffer, uint16_t mode){
    for(uint8_t p = 1; p < 2; p--)
    {
        set_plane(p);
        memcpy(VGA_VRAM, buffer + (p*MODE0FH_WIDTH * MODE0FH_HEIGHT / 8), MODE0FH_WIDTH * MODE0FH_HEIGHT / 8);
    }
    return 0;
}

uint8_t* mode0fh_create_external_buffer(uint16_t mode){
    return heap_kernel_alloc(2 * MODE0FH_HEIGHT * MODE0FH_WIDTH / 8, 0);
}