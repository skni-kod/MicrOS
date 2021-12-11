#include "mode_0eh.h"
#include "drivers/vga/modes/registerfunctions.h"
#include "memory/heap/heap.h"
#include "drivers/dal/videocard/videocard.h"

//REGISTER VALUES
unsigned char g_640x200x16[] =
	{
	/* MISC */
		0x63,
	/* SEQ */
		0x03, 0x01, 0x0F, 0x00, 0x06,
	/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
		0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9C, 0x8E, 0x8F, 0x28, 0x00, 0x96, 0xB9, 0xE3,
		0xFF,
	/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F,
		0xFF,
	/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		0x01, 0x00, 0x0F, 0x00, 0x00,
	};

//PALETTE
unsigned char palette0EH[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x2A, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x2A,
    0x00, 0x2A, 0x2A, 0x15, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x2A,
    0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x2A, 0x00, 0x2A, 0x2A, 0x15, 0x00, 0x2A, 0x2A, 0x2A,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x3F, 0x15, 0x3F, 0x15, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x3F,
    0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x15, 0x15, 0x15, 0x3F, 0x15, 0x3F,
    0x15, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x3F, 0x3F, 0x3F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x2A, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x2A,
    0x00, 0x2A, 0x2A, 0x15, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x2A,
    0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x2A, 0x00, 0x2A, 0x2A, 0x15, 0x00, 0x2A, 0x2A, 0x2A,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x3F, 0x15, 0x3F, 0x15, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x3F,
    0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x15, 0x15, 0x15, 0x3F, 0x15, 0x3F,
    0x15, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x3F, 0x3F, 0x3F,
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

unsigned char *mode0eh_buffer[] = {NULL, NULL, NULL, NULL};
unsigned char buffer_turned_on_0Eh = 0;

int8_t mode0eh_set_mode()
{
    writeRegisters(g_640x200x16);
    set_vga_palette(palette0EH);
    mode0eh_clear_screen();
    video_card_set_turn_on_buffer_func(&mode0eh_turn_on_buffer);
    video_card_set_turn_off_buffer_func(&mode0eh_turn_off_buffer);
    video_card_set_is_buffer_on_func(&mode0eh_is_buffer_on);
    video_card_set_swap_buffers_func(&mode0eh_swap_buffers);
    video_card_set_draw_pixel_func(&mode0eh_draw_pixel);
    video_card_set_draw_line_func(&mode0eh_draw_line);
    video_card_set_draw_circle_func(&mode0eh_draw_circle);
    video_card_set_draw_rectangle_func(&mode0eh_draw_rectangle);
    video_card_set_clear_screen_func(&mode0eh_clear_screen);

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


    return 0x0E;
}

int8_t mode0eh_turn_on_buffer()
{
    if(buffer_turned_on_0Eh) return -1;
    for(int i = 3; i >= 0; i--)
    {
        mode0eh_buffer[i] = heap_kernel_alloc(MODE0EH_HEIGHT * MODE0EH_WIDTH / 8, 0);
        if(mode0eh_buffer[i] == NULL)
        {
            for(int j = 3; j >= i; j--)
            {
                heap_kernel_dealloc(mode0eh_buffer[j]);
                mode0eh_buffer[j] = NULL;
            }
            return -1;
        }
    }
    video_card_set_draw_pixel_func(&mode0eh_draw_pixel_buffered);
    video_card_set_draw_line_func(&mode0eh_draw_line_buffered);
    video_card_set_draw_circle_func(&mode0eh_draw_circle_buffered);
    video_card_set_draw_rectangle_func(&mode0eh_draw_rectangle_buffered);
    video_card_set_clear_screen_func(&mode0eh_clear_screen_buffered);
    buffer_turned_on_0Eh = 1;
    return 0;
}

int8_t mode0eh_turn_off_buffer()
{
    if(!buffer_turned_on_0Eh) return -1;
    for(int i = 3; i >= 0; i--)
    {
        heap_kernel_dealloc(mode0eh_buffer[i]);
        mode0eh_buffer[i] = NULL;
    }
    video_card_set_draw_pixel_func(&mode0eh_draw_pixel);
    video_card_set_draw_line_func(&mode0eh_draw_line);
    video_card_set_draw_circle_func(&mode0eh_draw_circle);
    video_card_set_draw_rectangle_func(&mode0eh_draw_rectangle);
    video_card_set_clear_screen_func(&mode0eh_clear_screen);
    buffer_turned_on_0Eh = 0;
    return 0;
}

uint8_t mode0eh_is_buffer_on()
{
    return buffer_turned_on_0Eh;
}

int8_t mode0eh_swap_buffers()
{
    if(!buffer_turned_on_0Eh) return -1;
    for(uint8_t p = 3; p < 4; p--)
    {
        set_plane(p);
        memcpy(VGA_VRAM, mode0eh_buffer[p], MODE0EH_WIDTH * MODE0EH_HEIGHT / 8);
    }
    return 0;
}

int8_t mode0eh_draw_pixel(uint8_t color, uint16_t x, uint16_t y)
{
    if((x>=MODE0EH_WIDTH) || (y >=MODE0EH_HEIGHT))
        return -1;
    unsigned char *fb = (unsigned char *) VGA_VRAM;
    unsigned int offset = (y * MODE0EH_WIDTH + x)/8;
	unsigned bit_no = x % 8;
	for(char p = 3; p >= 0; p--)
	{
		set_plane(p);
		bit_write(fb[offset], 1<<(7-bit_no), (bit_get(color, 1 << p)));
	}
    return 0;
}

int8_t mode0eh_draw_line(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
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
                mode0eh_draw_pixel(color, x, a * x + b);
        else
            for(int x = ax; x <= bx; ++x)
                mode0eh_draw_pixel(color, x, a * x + b);
    }
    else
    {
        float a = dx/(float)(dy);
        float b = ax - a * ay;
        if(ay > by)
            for(int y = by; y <= ay; ++ y)
                mode0eh_draw_pixel(color, a * y + b, y);
        else
            for(int y = ay; y <= by; ++ y)
                mode0eh_draw_pixel(color, a * y + b, y);
    }
    return 0;
}

int8_t mode0eh_draw_circle(uint8_t color, uint16_t x, uint16_t y, uint16_t radius)
{
    return 0;
}

int8_t mode0eh_draw_rectangle(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return 0;
}
int8_t mode0eh_clear_screen()
{
    for(uint8_t p = 3; p < 4; p--)
    {
        set_plane(p);
        memset(VGA_VRAM, 0, 64 * 1024);
    }
    return 0;
}

int8_t mode0eh_draw_pixel_buffered(uint8_t color, uint16_t x, uint16_t y)
{
    if((!buffer_turned_on_0Eh) || (x>=MODE0EH_WIDTH) || (y >=MODE0EH_HEIGHT))
        return -1;
    unsigned int offset = (y * MODE0EH_WIDTH + x)/8;
	unsigned bit_no = x % 8;
	for(uint8_t p = 3; p < 4; p--)
		bit_write(mode0eh_buffer[p][offset], 1<<(7-bit_no), (bit_get(color, 1 << p)));
    return 0;
}

int8_t mode0eh_draw_line_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    if(!buffer_turned_on_0Eh) return -1;
    if(ax == bx) return -1;
    int32_t dx = (int32_t)bx - ax;
    int32_t dy = (int32_t)by - ay;
    if(_abs(dx) >= _abs(dy))
    {
        float a = dy/(float)(dx);
        float b = ay - a * ax;
        if(ax > bx)
            for(int x = bx; x <= ax; ++x)
                mode0eh_draw_pixel_buffered(color, x, a * x + b);
        else
            for(int x = ax; x <= bx; ++x)
                mode0eh_draw_pixel_buffered(color, x, a * x + b);
    }
    else
    {
        float a = dx/(float)(dy);
        float b = ax - a * ay;
        if(ay > by)
            for(int y = by; y <= ay; ++ y)
                mode0eh_draw_pixel_buffered(color, a * y + b, y);
        else
            for(int y = ay; y <= by; ++ y)
                mode0eh_draw_pixel_buffered(color, a * y + b, y);
    }
    return 0;
}
int8_t mode0eh_draw_circle_buffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius)
{
    return 0;
}
int8_t mode0eh_draw_rectangle_buffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return 0;
}
int8_t mode0eh_clear_screen_buffered()
{
    if(!buffer_turned_on_0Eh) return -1;
    for(uint8_t p = 3; p < 4; p--)
        memset(mode0eh_buffer[p], 0, 64*1024);
    return 0;
}

int8_t mode0eh_draw_pixel_external_buffer(uint8_t* buffer, uint16_t mode, int8_t color, uint16_t x, uint16_t y){
    if((x>=MODE0EH_WIDTH) || (y >=MODE0EH_HEIGHT))
        return -1;
    unsigned int offset = (y * MODE0EH_WIDTH + x)/8;
	unsigned bit_no = x % 8;
	for(uint8_t p = 3; p < 4; p--)
		bit_write(buffer[p * MODE0EH_HEIGHT * MODE0EH_WIDTH / 8 + offset], 1<<(7-bit_no), (bit_get(color, 1 << p)));
    return 0;
}
int8_t mode0eh_draw_line_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by){
    if(ax == bx) return -1;
    int32_t dx = (int32_t)bx - ax;
    int32_t dy = (int32_t)by - ay;
    if(_abs(dx) >= _abs(dy))
    {
        float a = dy/(float)(dx);
        float b = ay - a * ax;
        if(ax > bx)
            for(int x = bx; x <= ax; ++x)
                mode0eh_draw_pixel_external_buffer(buffer, mode, color, x, a * x + b);
        else
            for(int x = ax; x <= bx; ++x)
                mode0eh_draw_pixel_external_buffer(buffer, mode, color, x, a * x + b);
    }
    else
    {
        float a = dx/(float)(dy);
        float b = ax - a * ay;
        if(ay > by)
            for(int y = by; y <= ay; ++ y)
                mode0eh_draw_pixel_external_buffer(buffer, mode, color, a * y + b, y);
        else
            for(int y = ay; y <= by; ++ y)
                mode0eh_draw_pixel_external_buffer(buffer, mode, color, a * y + b, y);
    }
    return 0;
}
int8_t mode0eh_draw_circle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t x, uint16_t y, uint16_t radius){
    return 0;
}
int8_t mode0eh_draw_rectangle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by){
    return 0;
}

int8_t mode0eh_clear_screen_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y){
    memset(buffer, 0, 4*MODE0EH_WIDTH*MODE0EH_HEIGHT / 8);
    return 0;
}

int8_t mode0eh_swap_external_buffer(uint8_t* buffer, uint16_t mode){
    for(uint8_t p = 3; p < 4; p--)
    {
        set_plane(p);
        memcpy(VGA_VRAM, buffer + (p*MODE0EH_WIDTH * MODE0EH_HEIGHT / 8), MODE0EH_WIDTH * MODE0EH_HEIGHT / 8);
    }
    return 0;
}
uint8_t* mode0eh_create_external_buffer(uint16_t mode){
    uint8_t* ptr = heap_kernel_alloc(4 * MODE0EH_HEIGHT * MODE0EH_WIDTH / 8, 0);
    memset(ptr, 0, 4 * MODE0EH_HEIGHT * MODE0EH_WIDTH / 8);
    return ptr;
}