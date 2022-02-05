#include "partitions.h"
#include <stdlib.h>

kvector partitions;

void partitions_init()
{
    partitions_init_floppy();
    
    //partitions_init_harddisks(HARDDISK_ATA_MASTER, HARDDISK_ATA_PRIMARY_BUS);
    ///partitions_init_harddisks(HARDDISK_ATA_SLAVE, HARDDISK_ATA_PRIMARY_BUS);
    //partitions_init_harddisks(HARDDISK_ATA_MASTER, HARDDISK_ATA_SECONDARY_BUS);
    //partitions_init_harddisks(HARDDISK_ATA_SLAVE, HARDDISK_ATA_SECONDARY_BUS);
}

void partitions_init_floppy()
{
    // NOTE: it doesn't work well, so assume for now that floppy controller is always present
    // if(fdc_is_present())
    {
        floppy_init(18);
        
        // NOTE: it doesn't work well, so assume for now that floppy is always inserted
        // if(floppy_is_inserted())
        {
            partition *floppy_partition = (partition*)heap_kernel_alloc(sizeof(partition), 0);
            floppy_partition->type = filesystem_fat12;
            floppy_partition->symbol = 'A' + partitions.count;
            floppy_partition->header = heap_kernel_alloc(512, 0);
            floppy_partition->device_type = device_type_floppy;
            floppy_partition->device_number = 0;
            floppy_partition->write_on_device = floppy_write_sector;
            floppy_partition->read_from_device = floppy_read_continous;
            floppy_partition->first_sector = 0;

            
            memcpy(floppy_partition->header, floppy_do_operation_on_sector(0, 0, 1, true), 512);
            ;

            //while(1);
            fat_generic_set_current_partition(floppy_partition);
            
            if (fat_init())
            {
                kvector_add(&partitions, floppy_partition);
            }
            else
            {
                heap_kernel_dealloc(floppy_partition->header);
                heap_kernel_dealloc(floppy_partition);
            }
        }
    }
}

void partitions_init_harddisks(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus)
{
    HARDDISK_STATE state = harddisk_get_state(type, bus);
    if(state == HARDDISK_ATA_PRESENT)
    {
        mbr mbr_data;
        harddisk_read_sector(type, bus, 0, 0, (uint16_t *)&mbr_data);
        
        for(int i = 0; i < 4; i++)
        {
            if (mbr_data.partitions[i].type != 0)
            {
                char buffer[512];
                uint32_t fat_header_sector = mbr_data.partitions[i].first_sector_lba;
                
                harddisk_read_sector(type, bus, 0, fat_header_sector, (uint16_t *)buffer);
                
                partition *hdd_partition = (partition*)heap_kernel_alloc(sizeof(partition), 0);
                hdd_partition->header = heap_kernel_alloc(512, 0);
                memcpy(hdd_partition->header, buffer, 512);
                
                hdd_partition->type = partitions_get_filesystem_type(hdd_partition->header->system_identifier);
                hdd_partition->symbol = 'A' + partitions.count;
                hdd_partition->device_type = device_type_harddisk;
                hdd_partition->device_number = hdd_wrapper_get_device_number(type, bus);
                hdd_partition->write_on_device = hdd_wrapper_write_sector;
                hdd_partition->read_from_device = hdd_wrapper_read_sector;
                hdd_partition->first_sector = fat_header_sector;
                
                fat_generic_set_current_partition(hdd_partition);
                if (fat_init())
                {
                    kvector_add(&partitions, hdd_partition);
                }
                else
                {
                    heap_kernel_dealloc(hdd_partition->header);
                    heap_kernel_dealloc(hdd_partition);
                }
            }
        }
    }
}

partition *partitions_get_by_path(char *path)
{
    char partition_symbol = path[0];
    for (uint32_t i = 0; i < partitions.count; i++)
    {
        partition *partition_to_check = partitions.data[i];
        if(partition_to_check->symbol == partition_symbol)
        {
            return partition_to_check;
        }
    }
    
    return 0;
}

partition *partitions_get_by_symbol(char symbol)
{
    for (uint32_t i = 0; i < partitions.count; i++)
    {
        partition *partition_to_check = partitions.data[i];
        if(partition_to_check->symbol == symbol)
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
    for (uint32_t i = 0; i < partitions.count; i++)
    {
        partition *partition_to_check = partitions.data[i];
        symbol_array[i] = partition_to_check->symbol;
    }
}

filesystem_type partitions_get_filesystem_type(char *name)
{
    if (strncmp("FAT12", name, 4) == 0)
    {
        return filesystem_fat12;
    }
    else if(strncmp("FAT16", name, 4) == 0)
    {
        return filesystem_fat16;
    }
    
    return 0;
}

void partitions_get_info(char symbol, partition_info *info)
{
    partition *partition = partitions_get_by_symbol(symbol);
    if (partition != 0)
    {
        switch (partition->device_type)
        {
            case device_type_floppy:
            {
                info->device_type = 0;
                
                char floppy_name[] = "SHUKA FLOPPY";
                memcpy(info->device_name, floppy_name, sizeof(floppy_name));
                
                break;
            }
            
            case device_type_harddisk:
            {
                int type = hdd_wrapper_get_type_by_device_number(partition->device_number);
                int bus = hdd_wrapper_get_bus_by_device_number(partition->device_number);
                
                info->device_type = 1 + partition->device_number;
                harddisk_get_disk_model_number_terminated(type, bus, info->device_name);
                
                break;
            }
            
            case device_type_unknown:
            {
                // We have unsupported disk attached
                break;
            }
        }
    }
}