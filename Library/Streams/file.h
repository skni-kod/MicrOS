#ifndef FILE_H
#define FILE_H

typedef enum file_buffering_mode
{
    file_buffering_mode_none = 0,
    file_buffering_mode_line = 1,
    file_buffering_mode_full = 2
} file_buffering_mode;

typedef struct file
{
    char filename[128];
    char *buffer;
    uint32_t base;
    uint32_t pos;
    uint32_t size;
    uint32_t limit;
    uint32_t error;
    file_buffering_mode buffering_mode;

    int (*fetch)(struct file *file);
    void (*flush)(struct file *file);
} FILE;

#endif