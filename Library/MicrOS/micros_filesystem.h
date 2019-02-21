#ifndef MICROS_FILESYSTEM_H
#define MICROS_FILESYSTEM_H

#include <stdbool.h>
#include <stdint.h>
#include "micros_interrupts.h"

typedef struct micros_filesystem_time
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minutes;
    uint8_t seconds;
} __attribute__((packed)) micros_filesystem_time;

typedef struct micros_filesystem_file_info
{
    char name[32];
    char path[256];
    uint32_t size;

    micros_filesystem_time create_time;
    micros_filesystem_time modify_time;
    micros_filesystem_time access_time;
} micros_filesystem_file_info;

typedef struct fmicros_s_directory_info
{
    char name[32];
    char path[256];

    micros_filesystem_time create_time;
} micros_filesystem_directory_info;

bool micros_filesystem_get_file_info(char *path, micros_filesystem_file_info *file_info);
bool micros_filesystem_get_directory_info(char *path, micros_filesystem_directory_info *directory_info);
bool micros_filesystem_read_file(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length);
uint32_t micros_filesystem_get_entries_count_in_directory(char *path);
bool micros_filesystem_get_entries_in_directory(char *path, char *entries);
bool micros_filesystem_is_file(char *path);
bool micros_filesystem_is_directory(char *path);

#endif