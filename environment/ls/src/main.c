#include <micros.h>
#include <stdlib.h>
#include <stdio.h>

void reduce_slashes(char *path);

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("LS");
    char *path = argc >= 4 ? argv[2] : argv[1];
    char *current_dir = argv[1];
    
    char path_variations[2][64];
    sprintf(path_variations[0], "%s", path);
    sprintf(path_variations[1], "%s/%s", current_dir, path);
    
    for (int i = 0; i < 2; i++)
    {
        reduce_slashes(path_variations[i]);
        if((path_variations[i][0] == '/' && path_variations[i][1] == 0) || micros_filesystem_directory_exists(path_variations[i]))
        {
            int count = micros_filesystem_get_entries_count_in_directory(path_variations[i]);
            char **entries = malloc(sizeof(char *) * count);
            
            micros_filesystem_get_entries_in_directory(path_variations[i], entries);
            for(int i=0; i<count; i++)
            {
                printf("%s\n", entries[i]);
            }
            
            return;
        }
    }
    
    printf("Directory not found!\n");
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