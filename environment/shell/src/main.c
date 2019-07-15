#include <micros.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("SHELL");
    printf("MicrOS Shell v1.0\n");
    printf("Type path to execute an application\n");
    
    while(1)
    {
        char buffer[64];
        
        printf("> ");
        gets(buffer);
        
        uint32_t child_process_id = micros_process_start_process(buffer, "", true, true);
        micros_process_wait_for_process(child_process_id);
    }
    
    return 0;
}