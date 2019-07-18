#include <micros.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char current_dir[64];

void execute_cd(const char *str);
void execute_app(const char *str);
void back_to_previous_directory();
void reduce_slashes(char *path);
void split_to_path_and_args(const char *str, char *path, char *args);
void capitalize_string(char *str);

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
        }
        else
        {
            execute_app(path);
        }
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
    else if(parameter[0] == '.' && parameter[1] == '.')
    {
        back_to_previous_directory();
        return;
    }
    else
    {
        sprintf(path_to_switch, "%s/%s", current_dir, parameter);
    }
    reduce_slashes(path_to_switch);
    
    if((path_to_switch[0] == '/' && path_to_switch[1] == 0) || micros_filesystem_directory_exists(path_to_switch))
    {
        memcpy(current_dir, path_to_switch, sizeof(path_to_switch));
    }
    else
    {
        printf("Invalid path\n");
    }
}

void execute_app(const char *str)
{
    char path[64];
    char args[64];
    
    split_to_path_and_args(str, path, args);
    int path_length = strlen(path);
    
    capitalize_string(path);
    
    char path_variations[5][64];
    memcpy(path_variations[0], path, path_length);
    sprintf(path_variations[1], "%s/%s", current_dir, path);
    sprintf(path_variations[2], "%s/%s.ELF", current_dir, path);
    sprintf(path_variations[3], "/ENV/%s", path);
    sprintf(path_variations[4], "/ENV/%s.ELF", path);
   
    for (int i = 0; i < 5; i++)
    {
        reduce_slashes(path_variations[i]);
        if(micros_filesystem_file_exists(path_variations[i]))
        {
            char args_with_current_dir[64];
            sprintf(args_with_current_dir, "%s %s", current_dir, args);
            
            if(args_with_current_dir[strlen(args_with_current_dir) - 1] == ' ')
            {
                args_with_current_dir[strlen(args_with_current_dir) - 1] = 0;
            }
            
            uint32_t child_process_id = micros_process_start_process(path_variations[i], args_with_current_dir, true, true);
            micros_process_wait_for_process(child_process_id);
            
            return;
        }
    }
    
    printf("File not found\n");
}

void back_to_previous_directory()
{
    if(current_dir[0] == '/' && current_dir[1] == 0)
    {
        return;
    }
    
    int current_dir_length = strlen(current_dir);
    for (int i = current_dir_length - 1; i >= 0; i--)
    {
        if(current_dir[i] == '/')
        {
            current_dir[i] = 0;
            break;
        }
    }
    
    if (current_dir[0] == 0)
    {
        current_dir[0] = '/';
        current_dir[1] = 0; 
    }
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

void split_to_path_and_args(const char *str, char *path, char *args)
{
    int length = strlen(str);
    
    for(int i = 0; i < length + 1; i++)
    {
        if(str[i] == ' ' || str[i] == 0)
        {
            memcpy(path, str, i);
            memcpy(args, str + i + 1, 63 - i);
            
            path[i] = 0;
            return;
        }
    }
    
    memcpy(path, str, length);
    args[0] = 0;
}

void capitalize_string(char *str)
{
    for(size_t i = 0; i < strlen(str); i++)
    {
        if(str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] -= 'a' - 'A';
        }
    }
}