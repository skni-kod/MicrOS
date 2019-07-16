#include <micros.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char current_dir[64];

void execute_cd(const char *str);

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("SHELL");
    current_dir[0] = '/';
    current_dir[1] = 0;
    
    while(1)
    {
        char path[64];
        
        micros_console_position cursor_position;
        micros_console_set_cursor_visibility(true);
        micros_console_get_cursor_position(&cursor_position);
        
        if(cursor_position.x == 0 && cursor_position.y == 0)
        {
            printf("MicrOS Shell v1.0\n");
            printf("Type path to execute an application\n");
        }
        
        printf(" %s> ", current_dir);
        gets(path);
        
        path[strlen(path) - 1] = 0;
        if(path[0] == 'c' && path[1] == 'd')
        {
            execute_cd(path);
            continue;
        }
        else
        {
            if(!micros_filesystem_file_exists(path))
            {
                printf("File not found\n");
                continue;
            }
        }
        
        uint32_t child_process_id = micros_process_start_process(path, "", true, true);
        micros_process_wait_for_process(child_process_id);
    }
    
    return 0;
}

void execute_cd(const char *str)
{
    if(strlen(str) < 4)
    {
        printf("Invalid parameter\n");
        return;
    }
    
    char *parameter = (char *)str + 3;
    char path_to_switch[64];
    
    if(parameter[0] == '/')
    {
        memcpy(path_to_switch, parameter, strlen(parameter) + 1);
    }
    else
    {
        sprintf(path_to_switch, "%s/%s", current_dir, parameter);
        if(path_to_switch[0] == '/' && path_to_switch[1] == '/')
        {
            memmove(path_to_switch, path_to_switch + 1, 63);
        }
    }
    
    if((path_to_switch[0] == '/' && path_to_switch[1] == 0) || micros_filesystem_directory_exists(path_to_switch))
    {
        memcpy(current_dir, path_to_switch, sizeof(path_to_switch));
    }
    else
    {
        printf("Invalid path\n");
    }
}