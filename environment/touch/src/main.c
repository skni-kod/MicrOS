#include <micros.h>
#include <stdlib.h>
#include <stdio.h>

void reduce_slashes(char *path);

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("TOUCH");
    if(argc <= 3)
    {
        printf("Not enough parameters\n");
        return -1;
    }

    char *current_dir = argv[1];
    char *name = argv[2];
    
    if(strlen(name) > 12)
    {
        printf("Name too long\n");
        return -1;
    }
    
    char final_path[64];
    sprintf(final_path, "%s/%s", current_dir, name);
    
    reduce_slashes(final_path);
    if(micros_filesystem_file_exists(final_path) || micros_filesystem_directory_exists(final_path))
    {
        printf("File or directory with the specified name already exists\n");
        return -1;
    }
    
    micros_filesystem_create_file(final_path);
    
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