#ifndef FILESYSTEM_FILE_INFO_H
#define FILESYSTEM_FILE_INFO_H

#include <stdint.h>
#include "filesystem_time.h"

typedef struct filesystem_file_info
{
    char name[32];
    char path[256];
    uint32_t size;

    filesystem_time create_time;
    filesystem_time modify_time;
    filesystem_time access_time;
} filesystem_file_info;

#endif