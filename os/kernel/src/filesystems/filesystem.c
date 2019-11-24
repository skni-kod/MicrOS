#include "filesystem.h"

kvector partitions;

bool filesystem_init()
{
    if(fdc_is_present() && floppy_is_inserted())
    {
        partition *floppy_partition = (partition*)heap_kernel_alloc(sizeof(partition), 0);
        floppy_partition->type = filesystem_fat12;
        floppy_partition->symbol = 'A';
        floppy_partition->header = heap_kernel_alloc(512, 0);
        
        memcpy(floppy_partition->header, floppy_read_sector(0), 512);
        kvector_add(&partitions, floppy_partition);
    }
}

partition *filesystem_get_partition_and_valid_path(char *path)
{
    char partition_symbol = path[0];
    for (int i = 0; i < partitions.count; i++)
    {
        partition *partition_to_check = partitions.data[i];
        if(partition_to_check->symbol == partition_symbol)
        {
            int path_length = strlen(path);
            memcpy(path, path + 2, path_length - 2);
            path[path_length] = 0;
            
            return partition_to_check;
        }
    }
    
    return 0;
}

bool filesystem_get_file_info(char *path, filesystem_file_info *file_info)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_get_file_info(path, file_info);
}

bool filesystem_get_directory_info(char *path, filesystem_directory_info *directory_info)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_get_directory_info(path, directory_info);
}

bool filesystem_read_file(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_read_file(path, buffer, start_index, length);
}

uint32_t filesystem_get_entries_count_in_directory(char *path)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_get_entries_count_in_directory(path);
}

bool filesystem_get_entries_in_directory(char *path, char **entries)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_get_entries_in_directory(path, entries);
}

bool filesystem_is_file(char *path)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_is_file(path);
}

bool filesystem_is_directory(char *path)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_is_directory(path);
}

uint32_t filesystem_get_free_space()
{
    return fat_generic_get_free_space();
}

uint32_t filesystem_get_total_space()
{
    return fat_generic_get_total_space();
}

bool filesystem_create_file(char *path)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_create_file(path);
}

bool filesystem_create_directory(char *path)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_create_directory(path);
}

bool filesystem_delete_file(char *path)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_delete_file(path);
}

bool filesystem_delete_directory(char *path)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_delete_directory(path);
}

bool filesystem_rename_file(char *path, char *new_name)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_rename_file(path, new_name);
}

bool filesystem_rename_directory(char *path, char *new_name)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_rename_directory(path, new_name);
}

bool filesystem_save_to_file(char *path, char *buffer, int size)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_save_to_file(path, buffer, size);
}

bool filesystem_append_to_file(char *path, char *buffer, int size)
{
    partition *partition = filesystem_get_partition_and_valid_path(path);
    fat_generic_set_current_partition(partition);
    
    return fat_generic_append_to_file(path, buffer, size);
}