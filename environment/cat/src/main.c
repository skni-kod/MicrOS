#include <micros.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void reduce_slashes(char *path);

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("CAT");
    if(argc <= 3)
    {
        return -1;
    }

    char *current_dir = argv[1];
    char *path = argv[2];
    char current_partition_symbol = current_dir[0];
    
    char path_variations[3][64];
    sprintf(path_variations[0], "%s", path); // A:/ENV
    sprintf(path_variations[1], "%c:/%s", current_partition_symbol, path); // /ENV -> A:/ENV
    sprintf(path_variations[2], "%s/%s", current_dir, path); // DATA -> A:/ENV/DATA
    
    for (int i = 0; i < 3; i++)
    {
        reduce_slashes(path_variations[i]);
        if(micros_filesystem_file_exists(path_variations[i]))
        {
            micros_filesystem_file_info info;
            micros_filesystem_get_file_info(path_variations[i], &info);
            
            uint8_t buffer[1600];
            int length = info.size > 1600 ? 1600 : info.size;
            
            micros_filesystem_read_file(path_variations[i], buffer, 0, 1599);
            
            if(length == 1600)
            {
                printf("First 1600 bytes of the file:\n");
            }
            
            for(int i = 0; i < length; i++)
            {
                micros_console_print_char(buffer[i]);
            }
            
            return 0;
        }
    }
    
    printf("Invalid path\n");
    return -1;
}

void reduce_slashes(char *path)
{
    int length = strlen(path);
    int i = 0;
    
    while (i < length - 1)
    {
        if(path[i] == '/' && path[i + 1] == '/')
        {
            memmove(path + i, path + i + 1, 63 - i);
        }
        else
        {
            i++;
        }
    }
}