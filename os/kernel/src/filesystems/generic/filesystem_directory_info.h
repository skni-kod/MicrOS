#ifndef FILESYSTEM_DIRECTORY_INFO_H
#define FILESYSTEM_DIRECTORY_INFO_H

#include "filesystem_time.h"

typedef struct filesystem_directory_info
{
    char name[32];
    char path[256];

    filesystem_time create_time;
} filesystem_directory_info;

#endif