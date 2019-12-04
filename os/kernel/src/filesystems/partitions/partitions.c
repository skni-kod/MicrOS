#include "partitions.h"

kvector partitions;

void partitions_init()
{
    partitions_init_floppy();
    
    partitions_init_harddisks(HARDDISK_ATA_MASTER, HARDDISK_ATA_PRIMARY_BUS);
    partitions_init_harddisks(HARDDISK_ATA_SLAVE, HARDDISK_ATA_PRIMARY_BUS);
    partitions_init_harddisks(HARDDISK_ATA_MASTER, HARDDISK_ATA_SECONDARY_BUS);
    partitions_init_harddisks(HARDDISK_ATA_SLAVE, HARDDISK_ATA_SECONDARY_BUS);
}

void partitions_init_floppy()
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

void partitions_init_harddisks(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus)
{
    HARDDISK_STATE state = harddisk_get_state(type, bus);
    if(state == HARDDISK_ATA_PRESENT)
    {
        mbr mbr_data;
        harddisk_read_sector(type, bus, 0, 0, (uint8_t *)&mbr_data);
        
        for(int i = 0; i < 4; i++)
        {
            if (mbr_data.partitions[i].status != 0)
            {
                int fat_header_sector = mbr_data.partitions[i].first_sector_lba;
                
                partition *hdd_partition = (partition*)heap_kernel_alloc(sizeof(partition), 0);
                hdd_partition->header = heap_kernel_alloc(512, 0);
                harddisk_read_sector(type, bus, fat_header_sector >> 32, fat_header_sector, (uint8_t *)&hdd_partition->header);
                
                while(1);
            }
        }
        
        while(1);
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