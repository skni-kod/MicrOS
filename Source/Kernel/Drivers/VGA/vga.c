#include "vga.h"
#include "../../Assembly/io.h"

// Position on screen
vga_screen_pos vga_cursor_pos = { .x = 0, .y = 0 };
volatile screen * const vga_video = (volatile screen*)(VGA_BASE_ADDR);
uint8_t vga_current_printing_screen = 0;
uint16_t vga_screen_offset = 0;

void vga_printchar(char c)
{
    vga_printchar_color(c, 0);
}

void vga_printchar_color(char c, vga_color * color)
{
    uint16_t pos = vga_calcualte_position_with_offset(vga_cursor_pos.x, vga_cursor_pos.y);

    if(c != '\n')
    {
        vga_video[pos].c.code = c;
        if(color != 0)
        {
            vga_video[pos].c.color = *color;
        }
        vga_cursor_pos.x += 1;
        if(vga_cursor_pos.x == VGA_SCREEN_COLUMNS)
        {
            vga_newline();
        }
    }
    else
    {
        vga_newline();
    }
}

void vga_printstring(char * str)
{
    char* ptr = str;
    while(*ptr != 0)
    {
        vga_printchar(*ptr);
        ptr++;
    }
}

void vga_printstring_color(char * str, vga_color * color)
{
    char* ptr = str;
    while(*ptr != 0)
    {
        vga_printchar_color(*ptr, color);
        ptr++;
    }
}

void vga_set_char(uint16_t x, uint16_t y, char c)
{
    uint16_t pos = vga_calcualte_position_with_offset(x, y);
    vga_video[pos].c.code = c;
}

void vga_set_char_struct(vga_screen_pos spos, char c)
{
    vga_set_char(spos.x, spos.y, c);
}

char vga_get_char(uint16_t x, uint16_t y)
{
    uint16_t pos = vga_calcualte_position_with_offset(x, y);
    return vga_video[pos].c.code;
}

char vga_get_char_struct(vga_screen_pos spos)
{
    return vga_get_char(spos.x, spos.y);
}

void vga_set_color(uint16_t x, uint16_t y, vga_color col)
{
    uint16_t pos = vga_calcualte_position_with_offset(x, y);
    vga_video[pos].c.color = col;
}

void vga_set_color_struct(vga_screen_pos spos, vga_color col)
{
    vga_set_color(spos.x, spos.y, col);
}

vga_color vga_get_color(uint16_t x, uint16_t y)
{
    uint16_t pos = vga_calcualte_position_with_offset(x, y);
    return vga_video[pos].c.color;
}

vga_color vga_get_color_struct(vga_screen_pos spos)
{
    return vga_get_color(spos.x, spos.y);
}

void vga_set_character(uint16_t x, uint16_t y, vga_character c)
{
    uint16_t pos = vga_calcualte_position_with_offset(x, y);
    vga_video[pos].c = c;
}

void vga_set_character_struct(vga_screen_pos spos, vga_character c)
{
    vga_set_character(spos.x, spos.y, c);
}

vga_character vga_get_character(uint16_t x, uint16_t y)
{
    uint16_t pos = vga_calcualte_position_with_offset(x, y);
    return vga_video[pos].c;
}

vga_character vga_get_character_struct(vga_screen_pos spos)
{
    return vga_get_character(spos.x, spos.y);
}

void vga_set_cursor_pos(uint16_t x, uint16_t y)
{
    vga_cursor_pos.x = x;
    vga_cursor_pos.y = y;
}

void vga_set_cursor_pos_struct(vga_screen_pos spos)
{
    vga_cursor_pos = spos;
}

vga_screen_pos vga_get_cursor_pos()
{
    return vga_cursor_pos;
}

void vga_clear_screen()
{
    vga_color_without_blink col = { .background = VGA_COLOR_BLACK, .letter = VGA_COLOR_LIGHT_GRAY };
    // Clear all rows
    for(uint16_t i = 0; i < VGA_SCREEN_ROWS; ++i)
    {
        // Clear all lines
        for(uint16_t j = 0; j < VGA_SCREEN_COLUMNS; ++j)
        {
            uint16_t pos = vga_calcualte_position_with_offset(j, i);
            // Clear 
            vga_video[pos].c.code = 0;
            vga_video[pos].c.color.color_without_blink = col;
        }
    }

    vga_cursor_pos.x = 0;
    vga_cursor_pos.y = 0;
}

void vga_change_printing_screen(uint8_t a)
{
    if(a < VGA_MAX_SCREENS)
    {
        vga_current_printing_screen = a;
        vga_screen_offset = VGA_SCREEN_OFFSET * (uint16_t)a;
    }
}

void vga_copy_screen(uint8_t from, uint8_t to)
{
    if(from < VGA_MAX_SCREENS && to < VGA_MAX_SCREENS)
    {
        uint16_t offset_from = VGA_SCREEN_OFFSET * (uint16_t)from;
        uint16_t offset_to = VGA_SCREEN_OFFSET * (uint16_t)to;

        // Copy all rows
        for(uint16_t i = 0; i < VGA_SCREEN_ROWS; ++i)
        {
            // Copy all lines
            for(uint16_t j = 0; j < VGA_SCREEN_COLUMNS; ++j)
            {
                uint16_t pos = vga_calcualte_position_without_offset(j, i);
                // Copy 
                vga_video[pos + offset_to].value = vga_video[pos + offset_from].value;
            }
        }
    }
}

void vga_newline()
{
    vga_cursor_pos.x = 0;
    vga_cursor_pos.y++;
    // When we reach end of screen
    if(vga_cursor_pos.y == VGA_SCREEN_ROWS)
    {
        // To current line we copy next line
        for(uint16_t i = 0; i < VGA_SCREEN_ROWS - 1; ++i)
        {
            for(uint16_t j = 0; j < VGA_SCREEN_COLUMNS; ++j)
            {
                uint16_t pos = vga_calcualte_position_with_offset(j, i);
                // Copy byte from next line
                vga_video[pos] = vga_video[pos + VGA_SCREEN_COLUMNS];
            }
        }
        vga_cursor_pos.y = VGA_SCREEN_ROWS - 1;

        // Clear last line
        vga_color_without_blink col = { .background = VGA_COLOR_BLACK, .letter = VGA_COLOR_LIGHT_GRAY };
        for(uint16_t i = 0; i < VGA_SCREEN_COLUMNS; ++i)
        {
            uint16_t pos = vga_calcualte_position_with_offset(i, vga_cursor_pos.y);
            // Clear 
            vga_video[pos].c.code = 0;
            vga_video[pos].c.color.color_without_blink = col;
        }
    }
}

uint16_t vga_calcualte_position_with_offset(uint16_t x, uint16_t y)
{
    return (x + y * VGA_SCREEN_COLUMNS) + vga_screen_offset;
}

uint16_t vga_calcualte_position_without_offset(uint16_t x, uint16_t y)
{
    return x + y * VGA_SCREEN_COLUMNS;
}



/*void writeGeneralRegisters(uint8_t i0, uint8_t i1)
{
    outb(0x3C2, i0);
    outb(0x3BA, i0);
}*/

void writeRegisterDataNext(uint16_t port, uint8_t index, uint8_t data)
{
    outb(port, index);
    outb(port + 1, data);
}

uint8_t getRegisterDataNext(uint16_t port, uint8_t index)
{
    outb(port, index);
    return inb(port + 1);
}

void writeRegisterDataSame(uint16_t port, uint8_t index, uint8_t data)
{
    outb(port, index);
    outb(port, data);
}

void writeSequenceRegisters(uint8_t* i)
{
    for(int j=0; j<=0x5; j++)
    {
        writeRegisterDataNext(0x3C4, j, i[j]);
    }
}

void getSequenceRegisters(uint8_t* i)
{
    for(int j=0; j<=0x5; j++)
    {
        i[j] = getRegisterDataNext(0x3C4, j);
        //writeRegisterDataNext(0x3C4, j, i[j]);
    }
}

void writeCRTCRegisters(uint8_t* i)
{
    for(int j=0; j<=0x18; j++)
    {
        writeRegisterDataNext(0x3B4, j, i[j]);
    }
}

void getCRTCRegisters(uint8_t* i)
{
    for(int j=0; j<=0x18; j++)
    {
        i[j] = getRegisterDataNext(0x3D4, j);
        //writeRegisterDataNext(0x3C4, j, i[j]);
    }
}

void writeGraphicsContRegisters(uint8_t* i)
{
    for(int j=0; j<=0x8; j++)
    {
        writeRegisterDataNext(0x3CE, j, i[j]);
    }
}

void getGraphicsContRegisters(uint8_t* i)
{
    for(int j=0; j<=0x8; j++)
    {
        i[j] = getRegisterDataNext(0x3CE, j);
        //writeRegisterDataNext(0x3C4, j, i[j]);
    }
}

void writeAtributteContRegisters(uint8_t* i)
{
    for(int j=0; j<=0x14; j++)
    {
        inb(0x3BA);
        writeRegisterDataSame(0x3C0, j, i[j]);
    }
}

void getAtributteContRegisters(uint8_t* i)
{
    //inb(0x3C1);
    disable();
    for(int j=0x10; j<=0x14; j++)
    {
        uint8_t a = inb(0x3DA);
        a++;
        i[j] = getRegisterDataNext(0x3C0, j);
        //writeRegisterDataNext(0x3C4, j, i[j]);
    }
    enable();
}

#define VGA_VRAM 0xA0000
#define PITCH 320
void pixel_256 (unsigned char color, unsigned int x, unsigned int y) {
   unsigned char* fb       = (unsigned char*) VGA_VRAM;
   unsigned int offset     = y * PITCH + x;
   fb [offset] = color;
}

void testRegisters()
{
    uint8_t misc = inb(0x3CC);
    uint8_t sequencer[5];
    uint8_t crtc[25];
    uint8_t graphics[9];
    uint8_t attribute[21];
    getSequenceRegisters(sequencer);
    getCRTCRegisters(crtc);
    getGraphicsContRegisters(graphics);
    getAtributteContRegisters(attribute);

    int i = 0;

}

void enter_13H_mode()
{
    outb(0x3C2, 0x63);
    uint8_t sequencer[5] = {0x3,0x1,0xF,0,0xE};
    uint8_t crtc[] = {0x5F,0x4F,0x50,0x82,0x24,0x80,0xBF,0x1F,0,0x41,
   0,0,0,0,0,0x31,0x9C,0x8E,0x8F,0x28,0x40,0x96,0x89,0xA3,0xFF};
    uint8_t graphics[] = {0,0,0,0,0,0x40,0x5,0xF,0xFF};
    uint8_t attribute[] = {0,1,2,3,4,5,6,7,8,9,0xA,0xB,0xC,0xD,0xE,0xF,0x41,0,0xF,0,0};

    writeSequenceRegisters(sequencer);
    writeCRTCRegisters(crtc);
    writeGraphicsContRegisters(graphics);
    writeAtributteContRegisters(attribute);
    /*outb(0x3C0, 0x10);
    outb(0x3C0, 0x41);

    outb(0x3C0, 0x11);
    outb(0x3C0, 0x00);

    outb(0x3C0, 0x12);
    outb(0x3C0, 0x0F);

    outb(0x3C0, 0x13);
    outb(0x3C0, 0x00);

    outb(0x3C0, 0x14);
    outb(0x3C0, 0x00);

    outb(0x3C2, 0x63);

    outb(0x3C4, 0x01);
    outb(0x3C4 + 1, 0x01);

    outb(0x3C4, 0x03);
    outb(0x3C4 + 1, 0x00);

    outb(0x3C4, 0x04);
    outb(0x3C4 + 1, 0x0E);

    outb(0x3CE, 0x05);
    outb(0x3CE + 1, 0x40);

    outb(0x3CE, 0x06);
    outb(0x3CE + 1, 0x05);

    outb(0x3B4, 0x00);
    outb(0x3B4 + 1, 0x5F);

    outb(0x3B4, 0x01);
    outb(0x3B4 + 1, 0x4F);

    outb(0x3B4, 0x02);
    outb(0x3B4 + 1, 0x50);

    outb(0x3B4, 0x03);
    outb(0x3B4 + 1, 0x82);

    outb(0x3B4, 0x04);
    outb(0x3B4 + 1, 0x54);

    outb(0x3B4, 0x05);
    outb(0x3B4 + 1, 0x80);

    outb(0x3B4, 0x06);
    outb(0x3B4 + 1, 0xBF);

    outb(0x3B4, 0x07);
    outb(0x3B4 + 1, 0x1F);

    outb(0x3B4, 0x08);
    outb(0x3B4 + 1, 0x00);

    outb(0x3B4, 0x09);
    outb(0x3B4 + 1, 0x41);

    outb(0x3B4, 0x10);
    outb(0x3B4 + 1, 0x9C);

    outb(0x3B4, 0x11);
    outb(0x3B4 + 1, 0x8E);

    outb(0x3B4, 0x12);
    outb(0x3B4 + 1, 0x8F);

    outb(0x3B4, 0x13);
    outb(0x3B4 + 1, 0x28);

    outb(0x3B4, 0x14);
    outb(0x3B4 + 1, 0x40);

    outb(0x3B4, 0x15);
    outb(0x3B4 + 1, 0x96);

    outb(0x3B4, 0x16);
    outb(0x3B4 + 1, 0xB9);

    outb(0x3B4, 0x17);
    outb(0x3B4 + 1, 0xA3);*/
}

void drawDupaIn13H()
{
    for(int x = 0; x<320; x++)
    {
        for(int y = 0; y<200; y++)
            pixel_256(3, x, y);
    }
}

void enter_3H_mode()
{
    outb(0x3C0, 0x10);
    outb(0x3C0, 0x0C);

    outb(0x3C0, 0x11);
    outb(0x3C0, 0x00);

    outb(0x3C0, 0x12);
    outb(0x3C0, 0x0F);

    outb(0x3C0, 0x13);
    outb(0x3C0, 0x08);

    outb(0x3C0, 0x14);
    outb(0x3C0, 0x00);

    outb(0x3C2, 0x67);

    outb(0x3C4, 0x01);
    outb(0x3C4 + 1, 0x00);

    outb(0x3C4, 0x03);
    outb(0x3C4 + 1, 0x00);

    outb(0x3C4, 0x04);
    outb(0x3C4 + 1, 0x07);

    outb(0x3CE, 0x05);
    outb(0x3CE + 1, 0x10);

    outb(0x3CE, 0x06);
    outb(0x3CE + 1, 0x0E);

    outb(0x3B4, 0x00);
    outb(0x3B4 + 1, 0x5F);

    outb(0x3B4, 0x01);
    outb(0x3B4 + 1, 0x4F);

    outb(0x3B4, 0x02);
    outb(0x3B4 + 1, 0x50);

    outb(0x3B4, 0x03);
    outb(0x3B4 + 1, 0x82);

    outb(0x3B4, 0x04);
    outb(0x3B4 + 1, 0x55);

    outb(0x3B4, 0x05);
    outb(0x3B4 + 1, 0x81);

    outb(0x3B4, 0x06);
    outb(0x3B4 + 1, 0xBF);

    outb(0x3B4, 0x07);
    outb(0x3B4 + 1, 0x1F);

    outb(0x3B4, 0x08);
    outb(0x3B4 + 1, 0x00);

    outb(0x3B4, 0x09);
    outb(0x3B4 + 1, 0x4F);

    outb(0x3B4, 0x10);
    outb(0x3B4 + 1, 0x9C);

    outb(0x3B4, 0x11);
    outb(0x3B4 + 1, 0x8E);

    outb(0x3B4, 0x12);
    outb(0x3B4 + 1, 0x8F);

    outb(0x3B4, 0x13);
    outb(0x3B4 + 1, 0x28);

    outb(0x3B4, 0x14);
    outb(0x3B4 + 1, 0x1F);

    outb(0x3B4, 0x15);
    outb(0x3B4 + 1, 0x96);

    outb(0x3B4, 0x16);
    outb(0x3B4 + 1, 0xB9);

    outb(0x3B4, 0x17);
    outb(0x3B4 + 1, 0xA3);
}