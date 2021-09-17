#ifndef LZ77_H
#define LZ77

#include <stdint.h>
#include <stddef.h>

typedef uint8_t byte;

uint32_t lz77_compress(uint8_t* uncompressed_text, uint32_t uncompressed_size, uint8_t* compressed_text, uint8_t pointer_length_width);
uint32_t lz77_decompress(uint8_t* compressed_text, uint8_t* uncompressed_text);
uint32_t compress_file(char* filename_in, char* filename_out, size_t malloc_size, uint8_t pointer_length_width);
uint32_t decompress_file(char* filename_in, char* filename_out);

uint32_t compress(byte* src, byte* dst, size_t malloc_size, uint8_t pointer_length_width);
uint32_t decompress(byte* src, size_t src_len, byte** dst);
#endif