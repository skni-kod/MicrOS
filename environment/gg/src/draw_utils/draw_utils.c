#include "draw_utils.h"
#include "../util/screen.h"

#include <string.h>

extern byte* gpuBuffer;

void draw_rect(rect* rectangle, byte color, rect* camera)
{
    int x, y;
    if(camera != NULL)
    {
        x = rectangle->x - camera->x;
        y = rectangle->y - camera->y;
    }
    else
    {
        x = rectangle->x;
        y = rectangle->y;
    }

    int right,left,top,bottom;
    
    if(y > y + rectangle->h - 1)
    {
        top = y + rectangle->h -1;    
        bottom = y;
    }
    else
    {
        top = y;
        bottom = y + rectangle->h - 1;
    }

    if(x > x + rectangle->w - 1)
    {
        left = x + rectangle->w - 1;    
        right = x;
    }
    else
    {
        left = x;
        right = x + rectangle->w - 1;
    }

    if(left < 0 || right >= SCREEN_WIDTH) return;
    if(bottom < 0 || top >= SCREEN_HEIGHT) return;


    int top_offset = top >= 0 ? top : 0;
    int bottom_offset = bottom;

    //optimize it a little bit, maybe use SHR and SHL things to improve performance a lot.

    for(int i = left; i < right; i++)
    {
        if(top_offset >= 0) gpuBuffer[top_offset*SCREEN_WIDTH+i] = color;
        if(bottom_offset < SCREEN_HEIGHT) gpuBuffer[bottom_offset*SCREEN_WIDTH+i] = color;
    }

    for(int i = top_offset; i <= bottom_offset; i++)
    {
        
        if(left >= 0) gpuBuffer[i * SCREEN_WIDTH + left] = color;
        if(right < SCREEN_WIDTH) gpuBuffer[i * SCREEN_WIDTH + right] = color;
    }
}

void draw_fill_rect(rect* rectangle, byte color, rect* camera)
{
    int x, y;
    if(camera != NULL)
    {
        x = rectangle->x - camera->x;
        y = rectangle->y - camera->y;
    }
    else
    {
        x = rectangle->x;
        y = rectangle->y;
    }
    
    int right,left,top,bottom, temp;
    
    if(y > y + rectangle->h - 1)
    {
        top = y + rectangle->h -1;    
        bottom = y;
    }
    else
    {
        top = y;
        bottom = y + rectangle->h - 1;
    }

    if(x > x + rectangle->w - 1)
    {
        left = x + rectangle->w - 1;    
        right = x;
    }
    else
    {
        left = x;
        right = x + rectangle->w - 1;
    }

    int top_offset = top >= 0 ? top : 0;
    int bottom_offset = bottom;

    if(left < 0 || right >= SCREEN_WIDTH) return;
    if(bottom < 0 || top >= SCREEN_HEIGHT) return;

    int ox = left < 0 ? 0 : left;
    int ow = right < SCREEN_WIDTH ? right : SCREEN_WIDTH;
    int aw = (ow - ox) + 1;
    for(int i = top_offset; i <= bottom_offset; i++)
    {
        memset(&gpuBuffer[i*SCREEN_WIDTH + ox], color, aw);
    }
}
//This might be not too efficient, but should construct whole font.
bitmap_font* load_font(char* filename, int cell_width, int cell_height)
{
    bitmap_font* font = (bitmap_font*)calloc(1, sizeof(bitmap_font));

    font->font_img = loadImage(filename, true);

    int cellW = cell_width;
    int cellH = cell_height;

    int top = cellH;
    int baseA = cellH;

    uint8_t currentCharacter = 0;

    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            image_clip* curChar = &font->character[currentCharacter];

            curChar->x = cellW * j;
            curChar->y = cellH * i;

            curChar->w = cellW;
            curChar->h = cellH;

            //Find left side
            for(int pc = 0; pc < cellW; pc++)
            {
                for(int pr = 0; pr < cellH; pr++)
                {
                    int px = (cellW * j) + pc;
                    int py = (cellH * i) + pr;
                    if(font->font_img->data[py*font->font_img->width + px] != 0x03)
                    {
                        curChar->x = px;

                        //Make loops stop
                        pc = cellW;
                        pr = cellH;
                    }
                }
            }

            //Find right side
            for(int pc = cellW - 1; pc >= 0; pc--)
            {
                for(int pr = 0; pr < cellH; pr++)
                {
                    int px = (cellW * j) + pc;
                    int py = (cellH * i) + pr;
                    if(font->font_img->data[py*font->font_img->width + px] != 0x03)
                    {
                        curChar->w = (px - curChar->x) + 1;

                        //Make loops stop
                        pc = -1;
                        pr = cellH;
                    }
                }
            }
            
            //Find top side
            for(int pr = 0; pr < cellH; pr++)
            {
                for(int pc = 0; pc < cellW; pc++)
                {
                    int px = (cellW * j) + pc;
                    int py = (cellH * i) + pr;
                    if(font->font_img->data[py*font->font_img->width + px] != 0x03)
                    {
                        if(pr < top)
                        {
                            top = pr;
                        }

                        //Make loops stop
                        pc = cellW;
                        pr = cellH;
                    }
                }
            }
            
            //Find bottom of A letter, adjust if needed
            if(currentCharacter == 'A')
            {
                for(int pr = cellH - 1; pr >= 0; pr--)
                {
                    for(int pc = 0; pc < cellW; pc++)
                    {
                        int px = (cellW * j) + pc;
                        int py = (cellH * i) + pr;
                        if(font->font_img->data[py*font->font_img->width + px] != 0x03)
                        {
                            baseA = pr;

                            //Make loops stop
                            pc = cellW;
                            pr = -1;
                        }
                    }
                }
            }
            currentCharacter++;
        }
    }

    font->space = cellW / 2;
    font->nl_spacing = baseA;

    for(int i = 0; i < 256; i++)
    {
        font->character[i].y += top;
        font->character[i].h -= top;
    }

    return font;
}

void draw_text(char* str, int x, int y, bitmap_font* font, uint8_t color)
{
    int cx = x, cy = y;

    int len = strlen(str);

    for(int i = 0; i < len; i++)
    {
        if(str[i] == ' ')
        {
            cx += font->space;
        }
        else if(str[i] == '\n')
        {
            cy += font->nl_spacing;
            cx = x;
        }
        else
        {
            drawClippedTransparentC(font->font_img, cx, cy, &font->character[(uint8_t)str[i]], 0x03, color, NULL);
            cx += font->character[(uint8_t)str[i]].w + 1;
        }
    }
}
