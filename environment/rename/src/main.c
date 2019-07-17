#include <micros.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("RENAME");
    if(argc < 4)
    {
        return -1;
    }

    char *current_dir = argv[1];
    char *path = argv[2];
    char *new_name = argv[3];
    
    char path_variations[2][64];
    sprintf(path_variations[0], "%s", path);
    sprintf(path_variations[1], "%s/%s", current_dir, path);
    
    for (int i = 0; i < 2; i++)
    {
        if(micros_filesystem_is_file(path_variations[i]))
        {
            micros_filesystem_rename_file(path_variations[i], new_name);
            return 0;
        }
        else if(micros_filesystem_is_directory(path_variations[i]))
        {
            micros_filesystem_rename_directory(path_variations[i], new_name);
            return 0;
        }
    }
    
    printf("Invalid path\n");
    return -1;
}