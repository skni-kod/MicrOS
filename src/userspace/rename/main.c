#include <micros.h>
#include <stdlib.h>
#include <stdio.h>

void reduce_slashes(char *path);

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("RENAME");
    if(argc <= 4)
    {
        return -1;
    }

    char *current_dir = argv[1];
    char *path = argv[2];
    char *new_name = argv[3];
    char current_partition_symbol = current_dir[0];
    
    if(strlen(new_name) > 12)
    {
        printf("Name too long\n");
        return -1;
    }
    
    char path_variations[3][64];
    sprintf(path_variations[0], "%s", path); // A:/ENV
    sprintf(path_variations[1], "%c:/%s", current_partition_symbol, path); // /ENV -> A:/ENV
    sprintf(path_variations[2], "%s/%s", current_dir, path); // DATA -> A:/ENV/DATA
    
    for (int i = 0; i < 3; i++)
    {
        reduce_slashes(path_variations[i]);
        if(micros_filesystem_file_exists(path_variations[i]))
        {
            micros_filesystem_rename_file(path_variations[i], new_name);
            return 0;
        }
        else if(micros_filesystem_directory_exists(path_variations[i]))
        {
            micros_filesystem_rename_directory(path_variations[i], new_name);
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