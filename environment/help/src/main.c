#include <micros.h>
#include <stdio.h>

char help_path_with_partition[64];
const char help_path[] = "/DATA/HELP.TXT";

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("HELP");
    
    sprintf(help_path_with_partition, "%c:%s", argv[1][0], help_path);
    if(!micros_filesystem_file_exists((char *)help_path_with_partition))
    {
        printf("No HELP.TXT file\n");
        return -1;
    }
    
    micros_filesystem_file_info info;
    micros_filesystem_get_file_info((char *)help_path_with_partition, &info);
    
    uint8_t *buffer = malloc(sizeof(char) * info.size);
    micros_filesystem_read_file((char *)help_path_with_partition, buffer, 0, info.size);
     
    for(uint32_t i = 0; i < info.size; i++)
    {
        if(buffer[i] == '\r')
        {
            continue;
        }
        
        micros_console_print_char((char)buffer[i]);
    }
    micros_console_print_char('\n');
    
    free(buffer);
    return 0;
}