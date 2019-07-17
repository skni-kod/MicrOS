#include <micros.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("LS");
    char *path = argc >= 4 ? argv[2] : argv[1];
    
    if((path[0] == '/' && path[1] == 0) || micros_filesystem_directory_exists(path))
    {
        int count = micros_filesystem_get_entries_count_in_directory(path);
        char **entries = malloc(sizeof(char *) * count);
        
        micros_filesystem_get_entries_in_directory(path, entries);
        for(int i=0; i<count; i++)
        {
            printf("%s\n", entries[i]);
        }
    }
    else
    {
        printf("Directory not found!\n");
        return -1;
    }
    
    return 0;
}