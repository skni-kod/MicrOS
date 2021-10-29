#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "../stdafx.h"
#include <stddef.h>

/**
 * Load file from specified path into dst
 * @param filename path to a file on filesystem
 * @param dst destination location of data
 * @param compressed set to TRUE if file is compressed
 * @return amount of data loaded
 */
uint32_t loadFile(char* filename, byte** dst, bool compressed);
uint32_t saveFile(char* filename, byte* src, size_t src_size, bool useCompression);



#endif