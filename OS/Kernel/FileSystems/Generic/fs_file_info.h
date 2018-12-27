#ifndef FS_FILE_INFO_H
#define FS_FILE_INFO_H

#include <stdint.h>
#include "fs_time.h"

typedef struct file_info
{
    char name[32];
    char path[256];
    uint32_t size;

    fs_time create_time;
    fs_time modify_time;
    fs_time access_time;
} file_info;

#endif