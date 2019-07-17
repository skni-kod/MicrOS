#include <micros.h>
#include <stdlib.h>
#include <stdio.h>

void reduce_slashes(char *path);

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("RM");
    if(argc < 3)
    {
        return -1;
    }

    char *current_dir = argv[1];
    char *path = argv[2];
    
    char path_variations[2][64];
    sprintf(path_variations[0], "%s", path);
    sprintf(path_variations[1], "%s/%s", current_dir, path);
    
    for (int i = 0; i < 2; i++)
    {
        reduce_slashes(path_variations[i]);
        if(micros_filesystem_is_file(path_variations[i]))
        {
            micros_filesystem_delete_file(path_variations[i]);
            return 0;
        } 
        else if(micros_filesystem_is_directory(path_variations[i]))
        {
            if(micros_filesystem_get_entries_count_in_directory(path_variations[i]) > 0)
            {
                printf("Directory is not empty\n");
                return -1;
            }
            
            micros_filesystem_delete_directory(path_variations[i]);
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