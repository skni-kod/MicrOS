#ifndef FS_DIRECTORY_INFO_H
#define FS_DIRECTORY_INFO_H

typedef struct fs_directory_info
{
    char name[32];
    char path[256];

    fs_time create_time;
} fs_directory_info;

#endif