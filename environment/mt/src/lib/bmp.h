#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include "../renderer/ssurface.h"

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

ssurface* load_image(char* filename);

#endif