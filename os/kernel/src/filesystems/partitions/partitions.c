#include "partitions.h"

kvector partitions;

void partitions_init()
{
    if(fdc_is_present())
    {
        floppy_init(18);
        if(floppy_is_inserted())
        {
            partition *floppy_partition = (partition*)heap_kernel_alloc(sizeof(partition), 0);
            floppy_partition->type = filesystem_fat12;
            floppy_partition->symbol = 'A';
            floppy_partition->header = heap_kernel_alloc(512, 0);
            floppy_partition->device_type = device_type_floppy;
            floppy_partition->device_number = 0;
            floppy_partition->write_on_device = floppy_write_sector;
            floppy_partition->read_from_device = floppy_read_sector;
            
            memcpy(floppy_partition->header, floppy_do_operation_on_sector(0, 0, 1, true), 512);
            kvector_add(&partitions, floppy_partition);
            
            fat_generic_set_current_partition(floppy_partition);
            fat_init();
        }
    }
}

partition *partitions_get(char *path)
{
    char partition_symbol = path[0];
    for (int i = 0; i < partitions.count; i++)
    {
        partition *partition_to_check = partitions.data[i];
        if(partition_to_check->symbol == partition_symbol)
        {
            return partition_to_check;
        }
    }
    
    return 0;
}

int partitions_get_count()
{
    return partitions.count;
}

void partitions_get_symbols(char *symbol_array)
{
    for (int i = 0; i < partitions.count; i++)
    {
        partition *partition_to_check = partitions.data[i];
        symbol_array[i] = partition_to_check->symbol;
    }
}