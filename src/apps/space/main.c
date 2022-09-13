#include <micros.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    micros_process_set_current_process_name("SPACE");
    
    int partitions_count = micros_partitions_get_count();
    char *partition_symbols = malloc(sizeof(char) * partitions_count);
    micros_partitions_get_symbols(partition_symbols);
    
    for (int i = 0; i < partitions_count; i++)
    {
        int free_space = micros_filesystem_get_free_space(partition_symbols[i]) / 1000;
        int total_space = micros_filesystem_get_total_space(partition_symbols[i]) / 1000;
        int percent = 100 - free_space * 100 / total_space;
        
        printf("Partition %c:\n", partition_symbols[i]);
        printf("Free space: %d KB, Total space: %d KB\n", free_space, total_space);
        printf("Usage: %d %%\n\n", percent);
    }
    
    free(partition_symbols);
    return 0;
}