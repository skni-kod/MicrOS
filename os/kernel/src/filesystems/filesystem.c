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
        
        fat_generic_set_current_partition(floppy_partition);
        fat_init();
    }
}

partition *filesystem_get_partition(char *path)
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

void filesystem_remove_partition_from_path(char *old_path, char *new_path)
{
    int path_length = strlen(old_path);
    memcpy(new_path, old_path + 2, path_length - 2);
    new_path[path_length - 2] = 0;
}

bool filesystem_get_file_info(char *path, filesystem_file_info *file_info)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_get_file_info(path_without_partition, file_info);
}

bool filesystem_get_directory_info(char *path, filesystem_directory_info *directory_info)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_get_directory_info(path_without_partition, directory_info);
}

bool filesystem_read_file(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_read_file(path_without_partition, buffer, start_index, length);
}

uint32_t filesystem_get_entries_count_in_directory(char *path)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_get_entries_count_in_directory(path_without_partition);
}

bool filesystem_get_entries_in_directory(char *path, char **entries)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_get_entries_in_directory(path_without_partition, entries);
}

bool filesystem_is_file(char *path)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_is_file(path_without_partition);
}

bool filesystem_is_directory(char *path)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_is_directory(path_without_partition);
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
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_create_file(path_without_partition);
}

bool filesystem_create_directory(char *path)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_create_directory(path_without_partition);
}

bool filesystem_delete_file(char *path)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_delete_file(path_without_partition);
}

bool filesystem_delete_directory(char *path)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_delete_directory(path_without_partition);
}

bool filesystem_rename_file(char *path, char *new_name)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_rename_file(path_without_partition, new_name);
}

bool filesystem_rename_directory(char *path, char *new_name)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_rename_directory(path_without_partition, new_name);
}

bool filesystem_save_to_file(char *path, char *buffer, int size)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_save_to_file(path_without_partition, buffer, size);
}

bool filesystem_append_to_file(char *path, char *buffer, int size)
{
    partition *partition = filesystem_get_partition(path);
    fat_generic_set_current_partition(partition);
    
    char path_without_partition[128];
    filesystem_remove_partition_from_path(path, path_without_partition);
    
    return fat_generic_append_to_file(path_without_partition, buffer, size);
}