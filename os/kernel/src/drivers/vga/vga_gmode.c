/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * THIS FILE IS MARKED TO BE REFACTORED IN FUTURE. TRY NOT TO ADD NEW THINGS TO PARTS OF OS IN THIS FILE *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "vga_gmode.h"
#include "assembly/io.h"
#include <string.h>
//REMOVE
#include <stdio.h>
#include <math.h>
#include "vga.h"
#include "genericvga.h"
#include "filesystems/filesystem.h"
#include "modes/mode_13h/mode_13h.h"
#include "../vbe/vbe.h"

#define peekb(S, O) *(unsigned char *)(16uL * (S) + (O))
#define pokeb(S, O, V) *(unsigned char *)(16uL * (S) + (O)) = (V)
#define pokew(S, O, V) *(unsigned short *)(16uL * (S) + (O)) = (V)
#define _vmemwr(DS, DO, S, N) memcpy((char *)((DS)*16 + (DO)), S, N)

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(p,m,v) (v ? bit_set(p,m) : bit_clear(p,m))

static char mode = 3;


typedef struct bmpFileHeader
{
	uint16_t header;
	uint32_t fileSize;
	uint32_t reserved;
	uint32_t dataOffset;
} __attribute__((packed)) bmpFileHeader;

typedef struct bmpInfoHeader
{
	uint32_t headerSize;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bitPerPixel;
	uint32_t compression;
	uint32_t imageSize;
	uint32_t xPixelsPerM;
	uint32_t yPixelsPerM;
	uint32_t colorsUsed;
	uint32_t importantColors;
} __attribute__((packed)) bmpInfoHeader;

typedef struct _Os2BmpFileHeader
{
	uint16_t   FileType;      /* File type identifier */
	uint32_t  FileSize;      /* Size of the file in bytes */
	uint16_t   XHotSpot;      /* X coordinate of hotspot */
	uint16_t   YHotSpot;      /* Y coordinate of hotspot */
	uint32_t  BitmapOffset;  /* Starting position of image data in bytes */
} __attribute__((packed)) OS2BMPFILEHEADER;

typedef struct _Os21xBitmapHeader
{
	uint32_t Size;            /* Size of this header in bytes */
	uint32_t  Width;           /* Image width in pixels */
	uint32_t  Height;          /* Image height in pixels */
	uint16_t  NumPlanes;       /* Number of color planes */
	uint16_t  BitsPerPixel;    /* Number of bits per pixel */
} __attribute__((packed)) OS21XBITMAPHEADER;

typedef struct _Os21xPaletteElement
{
	uint8_t Blue;      /* Blue component */
	uint8_t Green;     /* Green component */
	uint8_t Red;       /* Red component */
} __attribute__((packed)) OS21XPALETTEELEMENT;

void drawMicrOSLogoIn13H()
{
	filesystem_file_info info;
	filesystem_get_file_info("A:/LOGO.BMP", &info);

	uint8_t *buffer = heap_kernel_alloc(info.size, 0);
	filesystem_read_file("A:/LOGO.BMP", buffer, 0, info.size);

	OS2BMPFILEHEADER fh;
	memcpy(&fh, buffer, sizeof(OS2BMPFILEHEADER));

	OS21XBITMAPHEADER bh;
	memcpy(&bh, buffer + sizeof(OS2BMPFILEHEADER), sizeof(OS21XBITMAPHEADER));

	for(int x = 0; x < bh.Width; x++)
		for(int y = 0; y < bh.Height; y++)
			mode13h_draw_pixel(buffer[fh.BitmapOffset + (bh.Height * bh.Width - bh.Width - y*bh.Width) + x],x,y);

	heap_kernel_dealloc(buffer);
}


void drawLenaIn13H()
{
	filesystem_file_info info;
	filesystem_get_file_info("A:/LENA.BMP", &info);

	uint8_t *buffer = heap_kernel_alloc(info.size, 0);
	filesystem_read_file("A:/LENA.BMP", buffer, 0, info.size);

	OS2BMPFILEHEADER fh;
	memcpy(&fh, buffer, sizeof(OS2BMPFILEHEADER));

	OS21XBITMAPHEADER bh;
	memcpy(&bh, buffer + sizeof(OS2BMPFILEHEADER), sizeof(OS21XBITMAPHEADER));

	for(int x = 0; x < bh.Width; x++)
		for(int y = 0; y < bh.Height; y++)
			mode13h_draw_pixel(buffer[fh.BitmapOffset + (bh.Height * bh.Width - bh.Width - y*bh.Width) + x],x,y);

	heap_kernel_dealloc(buffer);
}

uint32_t lz77_decompress(uint8_t* compressed_text, uint8_t* uncompressed_text)
{
    uint8_t pointer_length_width;
    uint16_t input_pointer, pointer_length, pointer_pos, pointer_length_mask;
    uint32_t compressed_pointer, coding_pos, pointer_offset, uncompressed_size;

    uncompressed_size = *((uint32_t*)compressed_text);
    pointer_length_width = *(compressed_text + 4);
    compressed_pointer = 5;

    pointer_length_mask = pow(2, pointer_length_width) - 1;

    for (coding_pos = 0; coding_pos < uncompressed_size; ++coding_pos)
    {
        input_pointer = *((uint16_t*)(compressed_text + compressed_pointer));
        compressed_pointer += 2;
        pointer_pos = input_pointer >> pointer_length_width;
        pointer_length = pointer_pos ? ((input_pointer & pointer_length_mask) + 1) : 0;
        if (pointer_pos)
            for (pointer_offset = coding_pos - pointer_pos; pointer_length > 0; --pointer_length)
                uncompressed_text[coding_pos++] = uncompressed_text[pointer_offset++];
        *(uncompressed_text + coding_pos) = *(compressed_text + compressed_pointer++);
    }

    return coding_pos;
}

uint32_t decompress(uint8_t* src, uint32_t src_len, uint8_t** dst)
{
    uint32_t compressed_size, uncompressed_size;
    uint8_t * uncompressed_text;

    compressed_size = src_len;

    uncompressed_size = *((uint32_t*)src);
    *dst = heap_kernel_alloc(uncompressed_size, 0);
    if (lz77_decompress(src, *dst) != uncompressed_size)
        return 0;

    return uncompressed_size;
}

long fsize(FILE* in)
{
    long pos, length;
    pos = ftell(in);
    fseek(in, 0L, SEEK_END);
    length = ftell(in);
    fseek(in, pos, SEEK_SET);
    return length;
}

void drawLenaIn10fH_linear()
{
	filesystem_file_info info;
	filesystem_get_file_info("A:/IMG.LZ", &info);

	uint8_t *buffer = heap_kernel_alloc(info.size, 0);
	filesystem_read_file("A:/IMG.LZ", buffer, 0, info.size);

	uint8_t *dst = NULL;

	int filesize = decompress(buffer, info.size, &dst);

	if(filesize == 0)
	{
		heap_kernel_dealloc(buffer);
		heap_kernel_dealloc(dst);
	}
	else
	{
		heap_kernel_dealloc(buffer);
		buffer = dst;

		int s = sizeof(bmpFileHeader);

		bmpFileHeader fh;
		memcpy(&fh, buffer, sizeof(bmpFileHeader));

		bmpInfoHeader bh;
		memcpy(&bh, buffer + sizeof(bmpFileHeader), sizeof(bmpInfoHeader));
		for(int y = bh.height - 1, j = 0; y >= 0; y--, j++)
			for(int x = 0; x < bh.width; x++)
				{
					//int i = 3*y * bh.width + 3*x;

					// uint8_t b = buffer[fh.dataOffset + (x * 3) + (bh.height - y - 1) * 3 * bh.width];
					// uint8_t g = buffer[fh.dataOffset + (x * 3) + (bh.height - y - 1) * 3 * bh.width + 1];
					// uint8_t r = buffer[fh.dataOffset + (x * 3) + (bh.height - y - 1) * 3 * bh.width + 2];

					uint8_t b = buffer[fh.dataOffset + ((x*3) + y * 3 * bh.width)];
					uint8_t g = buffer[fh.dataOffset + ((x*3) + y * 3 * bh.width) + 1];
					uint8_t r = buffer[fh.dataOffset + ((x*3) + y * 3 * bh.width) + 2];

					VBE_draw_pixel_8_8_8_8_linear(1024, 768, x, j, r, g, b, 255);
				}
		heap_kernel_dealloc(buffer);
	}
}

void drawLenaIn10fH()
{
	filesystem_file_info info;
	filesystem_get_file_info("A:/LENAD.BMP", &info);

	uint8_t *buffer = heap_kernel_alloc(info.size, 0);
	filesystem_read_file("A:/LENAD.BMP", buffer, 0, info.size);

	OS2BMPFILEHEADER fh;
	memcpy(&fh, buffer, sizeof(OS2BMPFILEHEADER));

	OS21XBITMAPHEADER bh;
	memcpy(&bh, buffer + sizeof(OS2BMPFILEHEADER), sizeof(OS21XBITMAPHEADER));

	for(int x = 0; x < bh.Width; x++)
		for(int y = 0; y < bh.Height; y++)
			//mode13h_draw_pixel(buffer[fh.BitmapOffset + (bh.Height * bh.Width - bh.Width - y*bh.Width) + x],x,y);
			VBE_draw_pixel_8_8_8(320, 240, 64, 64, x, y, buffer[fh.BitmapOffset + (x * 3) + (bh.Height - y - 1) * 3 * bh.Width + 2], buffer[fh.BitmapOffset + (x * 3) + (bh.Height - y - 1) * 3 * bh.Width + 1], buffer[fh.BitmapOffset + (x * 3) + (bh.Height - y - 1) * 3 * bh.Width]);
	heap_kernel_dealloc(buffer);
}

char vga_gmode_get_mode()
{
	return mode;
}