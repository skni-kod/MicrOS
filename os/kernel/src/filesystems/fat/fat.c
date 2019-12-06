#include "fat.h"

volatile partition *current_partition;

void fat_init()
{
    switch (current_partition->type)
    {
        case filesystem_fat12:
        {
            current_partition->last_valid_sector_mark = 0xFF0;
            current_partition->end_sector_mark = 0xFFF;
            break;
        }
        
        case filesystem_fat16:
        {
            current_partition->last_valid_sector_mark = 0xFFF0;
            current_partition->end_sector_mark = 0xFFFF;
            break;
        }
    }
    
    current_partition->fat_length = current_partition->header->bytes_per_sector * current_partition->header->sectors_per_fat;
    current_partition->fat = heap_kernel_alloc(current_partition->fat_length, 0);
    fat_load_fat();

    current_partition->directory_length = current_partition->header->directory_entries * 32;
    current_partition->root = heap_kernel_alloc(current_partition->directory_length, 0);
    fat_load_root();
}

void fat_load_fat()
{
    for (int i = current_partition->header->reserved_sectors; i < current_partition->header->sectors_per_fat; i++)
    {
        uint16_t sector_number = i + current_partition->first_sector;
        uint32_t fat_offset = (uint32_t)current_partition->fat + (i - current_partition->header->reserved_sectors) * current_partition->header->bytes_per_sector;
        uint8_t *buffer = current_partition->read_from_device(current_partition->device_number, sector_number);
        
        memcpy((void *)fat_offset, buffer, current_partition->header->bytes_per_sector);
    }
}

void fat_save_fat()
{
    for (int i = current_partition->header->reserved_sectors; i < current_partition->header->sectors_per_fat; i++)
    {
        uint16_t sector_number = i + current_partition->first_sector;
        uint32_t fat_offset = ((uint32_t)current_partition->fat + (i - current_partition->header->reserved_sectors) * current_partition->header->bytes_per_sector);
        
        current_partition->write_on_device(current_partition->device_number, sector_number, (char *)fat_offset);
    }
}

void fat_load_root()
{
    int root_first_sector = current_partition->header->reserved_sectors + current_partition->header->sectors_per_fat * current_partition->header->fat_count;
    int root_sectors_count = current_partition->header->directory_entries * 32 / current_partition->header->bytes_per_sector;

    for (int i = root_first_sector; i < root_first_sector + root_sectors_count; i++)
    {
        uint16_t sector_number = i + current_partition->first_sector;
        uint32_t root_offset = (uint32_t)current_partition->root + (i - root_first_sector) * current_partition->header->bytes_per_sector;
        
        uint8_t *buffer = current_partition->read_from_device(current_partition->device_number, sector_number);
        memcpy((void *)root_offset, buffer, current_partition->header->bytes_per_sector);
    }
}

void fat_save_root()
{
    int root_first_sector = current_partition->header->reserved_sectors + current_partition->header->sectors_per_fat * current_partition->header->fat_count;
    int root_sectors_count = current_partition->header->directory_entries * 32 / current_partition->header->bytes_per_sector;

    for (int i = root_first_sector; i < root_first_sector + root_sectors_count; i++)
    {
        uint16_t sector_number = i + current_partition->first_sector;
        uint32_t root_offset = (uint32_t)current_partition->root + (i - root_first_sector) * current_partition->header->bytes_per_sector;
        
        current_partition->write_on_device(current_partition->device_number, sector_number, (uint8_t *)root_offset);
    }
}

uint16_t fat_read_sector_value(uint32_t sector_number)
{
    uint8_t high_byte;
    uint8_t low_byte;

    switch (current_partition->type)
    {
        case filesystem_fat12:
        {
            if (sector_number % 2 == 0)
            {
                high_byte = *(current_partition->fat + (uint32_t)(sector_number * 1.5f + 1)) & 0x0F;
                low_byte = *(current_partition->fat + (uint32_t)(sector_number * 1.5f));

                return high_byte << 8 | low_byte;
            }
            else
            {
                high_byte = *(current_partition->fat + (uint32_t)(((sector_number - 1) * 1.5f) + 2));
                low_byte = *(current_partition->fat + (uint32_t)((sector_number - 1) * 1.5f) + 1) & 0xF0;

                return high_byte << 4 | low_byte >> 4;
            }
        }
        
        case filesystem_fat16:
        {
            high_byte = *(current_partition->fat + (uint32_t)(sector_number * 2 + 1));
            low_byte = *(current_partition->fat + (uint32_t)(sector_number * 2));
            
            return high_byte << 8 | low_byte;
        }
    }
}

void fat_save_sector_value(uint32_t sector_number, uint16_t value)
{
    uint8_t high_byte;
    uint8_t low_byte;
    
    switch (current_partition->type)
    {
        case filesystem_fat12:
        {
            if (sector_number % 2 == 0)
            {
                high_byte = *(current_partition->fat + (uint32_t)(sector_number * 1.5f + 1)) & 0xF0;
                high_byte |= (value >> 8) & 0x0F;
                low_byte = value & 0xFF;
                
                *(current_partition->fat + (uint32_t)(sector_number * 1.5f + 1)) = high_byte;
                *(current_partition->fat + (uint32_t)(sector_number * 1.5f)) = low_byte;
            }
            else
            {
                high_byte = (value >> 4) & 0xFF;
                low_byte = *(current_partition->fat + (uint32_t)((sector_number - 1) * 1.5f) + 1) & 0x0F;
                low_byte |= (value << 4) & 0xF0;
                
                *(current_partition->fat + (uint32_t)(((sector_number - 1) * 1.5f) + 2)) = high_byte;
                *(current_partition->fat + (uint32_t)((sector_number - 1) * 1.5f) + 1) = low_byte;
            }
            
            break;
        }
        
        case filesystem_fat16:
        {
            high_byte = value >> 8;
            low_byte = value;
            
            *(current_partition->fat + (uint32_t)(sector_number * 2 + 1)) = high_byte;
            *(current_partition->fat + (uint32_t)(sector_number * 2)) = low_byte;
            
            break;
        }
    }
}

kvector *fat_parse_path(char *path)
{
    kvector *chunks = heap_kernel_alloc(sizeof(kvector), 0);
    kvector_init(chunks);
    
    if(path[0] != '/')
    {
        return chunks;
    }

    uint8_t index = 0;
    while (*path != 0)
    {
        if (*path == '/')
        {
            char *string = heap_kernel_alloc(12, 0);
            memset(string, ' ', 12);
            kvector_add(chunks, string);

            index = 0;
        }
        else
        {
            char *string = chunks->data[chunks->count - 1];
            string[index] = *path;
            index++;
        }

        path++;
    }

    for (uint32_t i = 0; i < chunks->count; i++)
    {
        fat_normalise_filename(chunks->data[i], true);
    }

    if (index == 0)
    {
        heap_kernel_dealloc(chunks->data[chunks->count - 1]);
        kvector_remove(chunks, chunks->count - 1);
    }

    return chunks;
}

void fat_normalise_filename(char *filename, bool with_dot)
{
    char *ptr = filename;
    for (int i = 0; i < 12; i++)
    {
        if (*ptr == '.' && i < 9)
        {
            with_dot ? memmove(filename + 8, ptr, 4) : memmove(filename + 8, ptr + 1, 3);
            memset(filename + i, ' ', 11 - i - 3);
            break;
        }

        ptr++;
    }
}

void fat_denormalise_filename(char *filename)
{
    char *ptr = filename;
    int spaces_count = 0;

    for (int i = 0; i < 12; i++)
    {
        if (*ptr == ' ')
        {
            spaces_count++;
        }
        else if (*ptr == '.')
        {
            memmove(ptr - spaces_count, ptr, 4);
            if (spaces_count > 0)
            {
                *(ptr + 4 - spaces_count) = 0;
            }

            return;
        }
        else if (*ptr == '/')
        {
            *(ptr - spaces_count) = '/';
            *(ptr - spaces_count + 1) = 0;
            return;
        }

        ptr++;
    }

    if (spaces_count != 0)
    {
        filename[12 - spaces_count] = 0;
    }
}

uint16_t fat_save_file_to_sector(uint16_t initial_sector, uint16_t sectors_count, char* buffer)
{
    uint16_t sector = initial_sector == 0 ? fat_get_free_sector_index() : initial_sector;
    uint16_t sector_to_return = sector;
    
    for(int i=0; i<sectors_count; i++)
    {
        uint16_t sector_number = sector + 31 + current_partition->first_sector;
        uint32_t sector_offset = (uint8_t *)(buffer + (i * current_partition->header->bytes_per_sector));
        
        current_partition->write_on_device(current_partition->device_number, sector_number, sector_offset);
        
        uint16_t next_sector = fat_read_sector_value(sector);
        if(next_sector == 0 || next_sector >= current_partition->last_valid_sector_mark)
        {
            if(i == sectors_count - 1)
            {
                fat_save_sector_value(sector, current_partition->end_sector_mark);
                break;
            }
            else
            {
                next_sector = fat_get_free_sector_index();
                fat_save_sector_value(next_sector, current_partition->end_sector_mark);
            }
        }
        else
        {
            if(i == sectors_count - 1)
            {
                fat_save_sector_value(sector, current_partition->end_sector_mark);
                fat_clear_file_sectors(next_sector);
                break;
            }
        }
        
        fat_save_sector_value(sector, next_sector);
        sector = next_sector;
    }
    
    return sector_to_return;
}

bool fat_read_file_from_path(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length)
{
    fat_directory_entry *file_info = fat_get_info_from_path(path, false);

    if (file_info == NULL)
    {
        return false;
    }

    uint16_t initial_sector = start_index / current_partition->header->bytes_per_sector;
    uint16_t last_sector = (start_index + length) / current_partition->header->bytes_per_sector;
    uint16_t sectors_count = length == 0 ? file_info->size / current_partition->header->bytes_per_sector + 1 : (uint16_t)(last_sector - initial_sector + 1);
    uint32_t read_sectors = 0;

    uint8_t *result = fat_read_file_from_sector(file_info->first_sector, initial_sector, sectors_count, &read_sectors);
    memcpy(buffer, result + (start_index % current_partition->header->bytes_per_sector), length);

    heap_kernel_dealloc(result);
    if(file_info != 0) heap_kernel_dealloc(file_info);

    return true;
}

uint8_t *fat_read_file_from_sector(uint16_t initial_sector, uint16_t sector_offset, uint16_t sectors_count, uint32_t *read_sectors)
{
    uint16_t sector = initial_sector;
    for (int i = 0; i < sector_offset; i++)
    {
        sector = fat_read_sector_value(sector);
        if (sector == current_partition->end_sector_mark)
        {
            return NULL;
        }
    }

    uint8_t *buffer = heap_kernel_alloc(current_partition->header->bytes_per_sector, 0);
    *read_sectors = 0;

    while (*read_sectors < sectors_count * current_partition->header->sectors_per_cluster && sector < current_partition->last_valid_sector_mark)
    {
        int sector_to_read = ((sector - 2) * current_partition->header->sectors_per_cluster) +
                             current_partition->header->fat_count * current_partition->header->sectors_per_fat + 
                             current_partition->header->directory_entries * 32 / current_partition->header->bytes_per_sector +
                             current_partition->header->reserved_sectors +
                             current_partition->first_sector;
        
        for (int i = 0; i < current_partition->header->sectors_per_cluster && sector < current_partition->last_valid_sector_mark; i++)
        {
            uint8_t *read_data = current_partition->read_from_device(current_partition->device_number, sector_to_read + i);

            buffer = heap_kernel_realloc(buffer, current_partition->header->bytes_per_sector * (*read_sectors + 1), 0);
            memcpy(buffer + (*read_sectors * current_partition->header->bytes_per_sector), read_data, current_partition->header->bytes_per_sector);
        
            (*read_sectors)++;
        }
        
        sector = fat_read_sector_value(sector); 
    }

    return buffer;
}

bool fat_delete_file_from_path(char* path, bool is_directory)
{
    uint32_t read_sectors = 0;
    bool root_dir = false;
    
    kvector *chunks = fat_parse_path(path);
    fat_directory_entry *file_info = fat_get_info_from_chunks(chunks, is_directory);
    
    chunks->count--;
    fat_directory_entry *dir_info = fat_get_info_from_chunks(chunks, true);
    fat_directory_entry *directory = fat_get_directory_from_chunks(chunks, &read_sectors, &root_dir);
    chunks->count++;
    
    fat_directory_entry *current_file_ptr = directory;
    uint32_t items_count = file_info != 0 && directory != 0 ? read_sectors * 16 : 0;
    bool deleted = false;
    
    for (uint32_t i = 0; i < items_count; i++)
    {
        char full_filename[12];
        fat_merge_filename_and_extension(current_file_ptr, full_filename);

        if (fat_is_entry_valid(current_file_ptr) && memcmp(full_filename, chunks->data[chunks->count - 1], 12) == 0 &&
            current_file_ptr->file_attributes.subdirectory == is_directory)
        {
            current_file_ptr->filename[0] = 0xE5;
            fat_clear_file_sectors(current_file_ptr->first_sector);
            deleted = true;
            break;
        }
        
        current_file_ptr++;
    }
    
    if(deleted)
    { 
        if(!root_dir)
        {
            fat_save_file_to_sector(dir_info->first_sector, read_sectors, (char *)directory);
        }
        else
        {
            memcpy((void *)current_partition->root, directory, current_partition->directory_length);
            fat_save_root();
        }
        
        fat_save_fat();
    }
    
    kvector_clear(chunks);
    heap_kernel_dealloc(chunks);
    
    if(file_info != 0) { heap_kernel_dealloc(file_info); }
    if(dir_info != 0) { heap_kernel_dealloc(dir_info); }
    if(directory != 0) heap_kernel_dealloc(directory);
    
    return deleted;
}

bool fat_rename_file_from_path(char* path, char* new_name, bool is_directory)
{
    uint32_t read_sectors = 0;
    bool root_dir = false;
    
    if(strlen(new_name) > 11)
    {
        return false;
    }
    
    kvector *chunks = fat_parse_path(path);
    fat_directory_entry *file_info = fat_get_info_from_chunks(chunks, is_directory);
    
    chunks->count--;
    fat_directory_entry *dir_info = fat_get_info_from_chunks(chunks, true);
    fat_directory_entry *directory = fat_get_directory_from_chunks(chunks, &read_sectors, &root_dir);
    chunks->count++;
    
    fat_directory_entry *current_file_ptr = directory;
    uint32_t items_count = file_info != 0 && directory != 0 ? read_sectors * 16 : 0;
    bool changed = false;
    
    for (uint32_t i = 0; i < items_count; i++)
    {
        char full_filename[12];
        fat_merge_filename_and_extension(current_file_ptr, full_filename);

        if (fat_is_entry_valid(current_file_ptr) && memcmp(full_filename, chunks->data[chunks->count - 1], 12) == 0)
        {
            char name_without_dot[11];
            memset(name_without_dot, ' ', 11);
            memcpy(name_without_dot, new_name, strlen(new_name));
            fat_normalise_filename(name_without_dot, false);
        
            memcpy(current_file_ptr->filename, name_without_dot, 11);
            changed = true;
            
            break;
        }
        
        current_file_ptr++;
    }
    
    if(changed)
    {
        if(!root_dir)
        {
            fat_save_file_to_sector(dir_info->first_sector, read_sectors, (char *)directory);
        }
        else
        {
            memcpy((void *)current_partition->root, directory, current_partition->directory_length);
            fat_save_root();
        }  
    }
    
    fat_save_fat();
    
    kvector_clear(chunks);
    heap_kernel_dealloc(chunks);
    
    if(file_info != 0) { heap_kernel_dealloc(file_info); }
    if(dir_info != 0) { heap_kernel_dealloc(dir_info); }
    if(directory != 0) heap_kernel_dealloc(directory);
    
    return changed;
}

bool fat_save_file_from_path(char* path, char* buffer, uint32_t size)
{
    uint32_t read_sectors = 0;
    bool root_dir = false;
    
    kvector *chunks = fat_parse_path(path);
    fat_directory_entry *file_info = fat_get_info_from_chunks(chunks, false);
    
    chunks->count--;
    fat_directory_entry *dir_info = fat_get_info_from_chunks(chunks, true);
    fat_directory_entry *directory = fat_get_directory_from_chunks(chunks, &read_sectors, &root_dir);
    chunks->count++;
    
    fat_directory_entry *current_file_ptr = directory;
    uint32_t items_count = file_info != 0 && directory != 0 ? read_sectors * 16 : 0;
    bool changed = false;
    
    for (uint32_t i = 0; i < items_count; i++)
    {
        char full_filename[12];
        fat_merge_filename_and_extension(current_file_ptr, full_filename);

        if (fat_is_entry_valid(current_file_ptr) && memcmp(full_filename, chunks->data[chunks->count - 1], 12) == 0)
        {
            char* erased_buffer = heap_kernel_alloc(size, 0);
            memcpy(erased_buffer, buffer, size);
            
            fat_save_file_to_sector(file_info->first_sector, (size / current_partition->header->bytes_per_sector) + 1, erased_buffer);
            current_file_ptr->size = size;
            changed = true;
            
            heap_kernel_dealloc(erased_buffer);
            break;
        }
        
        current_file_ptr++;
    }
    
    if(changed)
    {
        rtc_time now;
        rtc_read(&now);
        
        fat_update_date(&current_file_ptr->modify_date, now.year, now.month, now.day);
        fat_update_time(&current_file_ptr->modify_time, now.hour, now.minute, now.second);
        
        if(!root_dir)
        {
            fat_save_file_to_sector(dir_info->first_sector, read_sectors, (char *)directory);
        }
        else
        {
            memcpy((void *)current_partition->root, directory, current_partition->directory_length);
            fat_save_root();
        }  
    }
    
    fat_save_fat();
    
    kvector_clear(chunks);
    heap_kernel_dealloc(chunks);
    
    if(file_info != 0) { heap_kernel_dealloc(file_info); }
    if(dir_info != 0) { heap_kernel_dealloc(dir_info); }
    if(directory != 0) heap_kernel_dealloc(directory);
    
    return changed;
}

bool fat_append_file_from_path(char* path, char* buffer, uint32_t size)
{
    uint32_t read_sectors = 0;
    bool root_dir = false;
    
    kvector *chunks = fat_parse_path(path);
    fat_directory_entry *file_info = fat_get_info_from_chunks(chunks, false);
    
    chunks->count--;
    fat_directory_entry *dir_info = fat_get_info_from_chunks(chunks, true);
    fat_directory_entry *directory = fat_get_directory_from_chunks(chunks, &read_sectors, &root_dir);
    chunks->count++;
    
    fat_directory_entry *current_file_ptr = directory;
    uint32_t items_count = file_info != 0 && directory != 0 ? read_sectors * 16 : 0;
    bool changed = false;
    
    for (uint32_t i = 0; i < items_count; i++)
    {
        char full_filename[12];
        fat_merge_filename_and_extension(current_file_ptr, full_filename);

        if (fat_is_entry_valid(current_file_ptr) && memcmp(full_filename, chunks->data[chunks->count - 1], 12) == 0)
        {
            uint32_t last_sector_ack;
            
            uint16_t last_file_sector = fat_get_last_file_sector(current_file_ptr->first_sector);
            uint8_t* last_sector_buffer = fat_read_file_from_sector(last_file_sector, 0, 1, &last_sector_ack);
            uint16_t last_sector_true_size = current_file_ptr->size % current_partition->header->bytes_per_sector;
            
            uint16_t new_sectors_count = (last_sector_true_size + size) / current_partition->header->bytes_per_sector;
            last_sector_buffer = heap_kernel_realloc(last_sector_buffer, (new_sectors_count + 1) * current_partition->header->bytes_per_sector, 0);
            memcpy(last_sector_buffer + last_sector_true_size, buffer, size);
            
            fat_save_file_to_sector(last_file_sector, new_sectors_count + 1, (char *)last_sector_buffer);
            current_file_ptr->size += size;
            changed = true;
            
            break;
        }
        
        current_file_ptr++;
    }
    
    if(changed)
    {
        rtc_time now;
        rtc_read(&now);
        
        fat_update_date(&current_file_ptr->modify_date, now.year, now.month, now.day);
        fat_update_time(&current_file_ptr->modify_time, now.hour, now.minute, now.second);
        
        if(!root_dir)
        {
            fat_save_file_to_sector(dir_info->first_sector, read_sectors, (char *)directory);
        }
        else
        {
            memcpy((void *)current_partition->root, directory, current_partition->directory_length);
            fat_save_root();
        }  
    }
    
    fat_save_fat();
    
    kvector_clear(chunks);
    heap_kernel_dealloc(chunks);
    
    if(file_info != 0) { heap_kernel_dealloc(file_info); }
    if(dir_info != 0) { heap_kernel_dealloc(dir_info); }
    if(directory != 0) heap_kernel_dealloc(directory);
    
    return changed;
}

bool fat_create_file_from_path(char* path, bool is_directory)
{
    uint32_t read_sectors = 0;
    bool root_dir = false;
    
    kvector *chunks = fat_parse_path(path);
    
    chunks->count--;
    fat_directory_entry *dir_info = fat_get_info_from_chunks(chunks, true);
    fat_directory_entry *directory = fat_get_directory_from_chunks(chunks, &read_sectors, &root_dir);
    chunks->count++;
    
    fat_directory_entry *current_file_ptr = directory;
    uint32_t items_count = directory != 0 ? read_sectors * 16 : 0;
    bool found = false;
    
    for (uint32_t i = 0; i < items_count; i++)
    {
        char full_filename[12];
        fat_merge_filename_and_extension(current_file_ptr, full_filename);

        if (!fat_is_entry_valid(current_file_ptr))
        {
            found = true;
            break;
        }
        
        current_file_ptr++;
    }
    
    // TODO: Change size of directory when there is no enough space to add new file
    // if(!found && !root_dir)
    // {
    //     directory = heap_kernel_realloc(directory, (read_sectors + 1) * 512, 0);
    //     current_file_ptr = directory + items_count;
    //     found = true;
    // }
    
    if(found)
    {
        char name_without_dot[12];
        memcpy(name_without_dot, chunks->data[chunks->count - 1], 12);
        fat_normalise_filename(name_without_dot, false);
        
        memset(current_file_ptr, 0, sizeof(fat_directory_entry));
        memcpy(current_file_ptr->filename, name_without_dot, 11);
        current_file_ptr->first_character = ' ';
        current_file_ptr->file_attributes.subdirectory = is_directory;
        current_file_ptr->file_attributes.archive = !is_directory;
        
        rtc_time now;
        rtc_read(&now);
        
        fat_update_date(&current_file_ptr->create_date, now.year, now.month, now.day);
        fat_update_date(&current_file_ptr->last_access_date, now.year, now.month, now.day);
        fat_update_date(&current_file_ptr->modify_date, now.year, now.month, now.day);
        
        fat_update_time(&current_file_ptr->create_time, now.hour, now.minute, now.second);
        fat_update_time(&current_file_ptr->modify_time, now.hour, now.minute, now.second);
        
        current_file_ptr->first_sector = fat_get_free_sector_index();
        
        char *empty_sector = heap_kernel_alloc(current_partition->header->bytes_per_sector * current_partition->header->reserved_sectors, 0);
        memset(empty_sector, 0, current_partition->header->bytes_per_sector * current_partition->header->reserved_sectors);
        
        fat_save_file_to_sector(current_file_ptr->first_sector, 1, empty_sector);
        heap_kernel_dealloc(empty_sector);
        
        if(!root_dir)
        {
            fat_save_file_to_sector(dir_info->first_sector, read_sectors, (char *)directory);
        }
        else
        {
            memcpy((void *)current_partition->root, directory, current_partition->directory_length);
            fat_save_root();
        }
        
        fat_save_fat();
    }
    
    kvector_clear(chunks);
    heap_kernel_dealloc(chunks);
    
    if(dir_info != 0) { heap_kernel_dealloc(dir_info); }
    if(directory != 0) heap_kernel_dealloc(directory);
    
    return found;
}

fat_directory_entry *fat_get_directory_from_path(char *path, uint32_t *read_sectors)
{
    kvector *chunks = fat_parse_path(path);
    bool root_dir = false;
    
    fat_directory_entry *directory = fat_get_directory_from_chunks(chunks, read_sectors, &root_dir);

    kvector_clear(chunks);
    heap_kernel_dealloc(chunks);

    return directory;
}

fat_directory_entry *fat_get_directory_from_chunks(kvector *chunks, uint32_t *read_sectors, bool *root_dir)
{
    fat_directory_entry *current_directory = heap_kernel_alloc(current_partition->directory_length, 0);
    memcpy(current_directory, (void *)current_partition->root, current_partition->directory_length);

    fat_directory_entry *result = NULL;

    if (chunks->count == 0)
    {
        *read_sectors = (current_partition->header->directory_entries * 32) / current_partition->header->bytes_per_sector;
        *root_dir = true;
        
        return current_directory;
    }
    *root_dir = false;

    fat_directory_entry *current_file_ptr = current_directory;
    uint32_t current_chunk_index = 0;
    uint32_t items_count = current_partition->header->directory_entries;

    for (uint32_t i = 0; i < items_count; i++)
    {
        char full_filename[12];
        fat_merge_filename_and_extension(current_file_ptr, full_filename);

        if (fat_is_entry_valid(current_file_ptr) && current_file_ptr->file_attributes.subdirectory &&
            memcmp(full_filename, chunks->data[current_chunk_index], 12) == 0)
        {
            uint8_t *directory = fat_read_file_from_sector(current_file_ptr->first_sector, 0, INT16_MAX, read_sectors);

            heap_kernel_dealloc(current_directory);

            current_directory = (fat_directory_entry *)directory;
            current_file_ptr = current_directory;

            if (current_chunk_index == chunks->count - 1)
            {
                result = current_directory;
                break;
            }
            else
            {
                i = 0;
                items_count = *read_sectors * 16;
                current_file_ptr = current_directory;
                current_chunk_index++;
            }
        }
        else
        {
            if (i + 1 == items_count)
            {
                result = NULL;
            }

            current_file_ptr++;
        }
    }

    return result;
}

fat_directory_entry *fat_get_info_from_path(char *path, bool is_directory)
{
    kvector *chunks = fat_parse_path(path);
    fat_directory_entry *info = fat_get_info_from_chunks(chunks, is_directory);

    kvector_clear(chunks);
    heap_kernel_dealloc(chunks);

    return info;
}

fat_directory_entry *fat_get_info_from_chunks(kvector *chunks, bool is_directory)
{
    if(chunks->count == 0)
    {
        return NULL;
    }
    
    char *target_filename = chunks->data[chunks->count - 1];
    uint32_t read_sectors = 0;
    bool root_dir = false;

    chunks->count--;

    fat_directory_entry *directory = fat_get_directory_from_chunks(chunks, &read_sectors, &root_dir);
    fat_directory_entry *current_file_ptr = directory;
    fat_directory_entry *result = NULL;
    
    chunks->count++;

    for (uint32_t i = 0; i < read_sectors * 16; i++)
    {
        char full_filename[12];
        fat_merge_filename_and_extension(current_file_ptr, full_filename);

        if ((fat_is_entry_valid(current_file_ptr) && current_file_ptr->file_attributes.subdirectory == is_directory))
        {
            if (memcmp(full_filename, target_filename, 12) == 0)
            {
                result = current_file_ptr;
                break;
            }
        }

        if (i + 1 == read_sectors * 16)
        {
            result = NULL;
            break;
        }

        current_file_ptr++;
    }

    fat_directory_entry *result_without_junk = NULL;
    if (result != NULL)
    {
        result_without_junk = heap_kernel_alloc(sizeof(fat_directory_entry), 0);
        memcpy(result_without_junk, result, sizeof(fat_directory_entry));
    }

    if(directory != 0) heap_kernel_dealloc(directory);

    return result_without_junk;
}

void fat_clear_file_sectors(uint32_t initial_sector)
{
    uint16_t current_sector_index = initial_sector;
    while(current_sector_index < current_partition->last_valid_sector_mark)
    {
        uint16_t value = fat_read_sector_value(current_sector_index);
        fat_save_sector_value(current_sector_index, 0);
        
        current_sector_index = value;
    }
}

uint32_t fat_get_last_file_sector(uint32_t initial_sector)
{
    uint16_t current_sector_index = initial_sector;
    while(true)
    {
        uint16_t new_sector_index = fat_read_sector_value(current_sector_index);
        if(new_sector_index >= current_partition->last_valid_sector_mark)
        {
            return current_sector_index;
        }
        
        current_sector_index = new_sector_index;
    }
}

uint32_t fat_get_entries_count_in_directory(char *path)
{
    kvector *chunks = fat_parse_path(path);
    uint32_t read_sectors = 0;
    bool root_dir = false;

    fat_directory_entry *directory = fat_get_directory_from_chunks(chunks, &read_sectors, &root_dir);
    fat_directory_entry *current_file_ptr = directory;
    uint32_t entries_count = 0;

    if (directory == NULL)
    {
        return 0;
    }

    for (uint32_t i = 0; i < read_sectors * 16; i++)
    {
        if (fat_is_entry_valid(current_file_ptr))
        {
            entries_count++;
        }

        if (i + 1 == read_sectors * 16)
        {
            break;
        }

        current_file_ptr++;
    }

    kvector_clear(chunks);
    heap_kernel_dealloc(chunks);
    if(directory != 0) heap_kernel_dealloc(directory);

    return entries_count;
}

uint32_t fat_get_entries_in_directory(char *path, char **entries)
{
    kvector *chunks = fat_parse_path(path);
    uint32_t path_length = strlen(path);
    uint32_t read_sectors = 0;
    bool root_dir = false;

    fat_directory_entry *directory = fat_get_directory_from_chunks(chunks, &read_sectors, &root_dir);
    fat_directory_entry *current_file_ptr = directory;
    uint32_t current_entry_index = 0;

    if (directory == NULL)
    {
        return false;
    }

    for (uint32_t i = 0; i < read_sectors * 16; i++)
    {
        if (fat_is_entry_valid(current_file_ptr))
        {
            char full_filename[12];
            fat_merge_filename_and_extension(current_file_ptr, full_filename);

            if (current_file_ptr->file_attributes.subdirectory)
            {
                full_filename[11] = '/';
            }

            fat_denormalise_filename(full_filename);

            entries[current_entry_index] = heap_user_alloc(path_length + 14, 0);
            memset(entries[current_entry_index], 0, path_length + 14);
            memcpy(entries[current_entry_index], path, path_length);
            
            bool shift = false;
            if(entries[current_entry_index][path_length - 1] != '/')
            {
                entries[current_entry_index][path_length] = '/';
                shift = true;
            }
            
            memcpy(entries[current_entry_index] + path_length + (shift ? 1 : 0), full_filename, 12);

            current_entry_index++;
        }

        if (i + 1 == read_sectors * 16)
        {
            break;
        }

        current_file_ptr++;
    }

    kvector_clear(chunks);
    heap_kernel_dealloc(chunks);
    if(directory != 0) heap_kernel_dealloc(directory);

    return true;
}

void fat_update_date(fat_directory_entry_date *fat_date, int year, int month, int day)
{
    fat_date->year = year - 1980;
    fat_date->month = month;
    fat_date->day = day;
}

void fat_update_time(fat_directory_entry_time *fat_time, int hours, int minutes, int seconds)
{
    fat_time->hours = hours;
    fat_time->minutes = minutes;
    fat_time->seconds = seconds / 2;
}

bool fat_is_entry_valid(fat_directory_entry *entry)
{
    return entry->filename[0] >= 32 && entry->filename[0] <= 126 && entry->filename[0] != 0x2E;
}

void fat_merge_filename_and_extension(fat_directory_entry *entry, char *buffer)
{
    memset(buffer, ' ', 12);
    memcpy(buffer, entry->filename, 8);

    if (!entry->file_attributes.subdirectory)
    {
        buffer[8] = '.';
        memcpy(buffer + 9, entry->extension, 3);
    }
}

// Generic filesystem functions
void fat_generic_set_current_partition(partition *partition)
{
    current_partition = partition;
}

bool fat_generic_get_file_info(char *path, filesystem_file_info *generic_file_info)
{
    fat_directory_entry *fat_file_info = fat_get_info_from_path(path, false);
    if (fat_file_info == NULL)
    {
        return false;
    }

    memcpy(generic_file_info->path, path, strlen(path) + 1);
    generic_file_info->size = fat_file_info->size;

    uint8_t filename_length = fat_generic_copy_filename_to_generic(fat_file_info->filename, generic_file_info->name);
    generic_file_info->name[filename_length] = '.';
    memcpy(generic_file_info->name + filename_length + 1, fat_file_info->extension, 3);

    fat_generic_convert_date_fat_to_generic(&fat_file_info->create_date, &fat_file_info->create_time, &generic_file_info->create_time);
    fat_generic_convert_date_fat_to_generic(&fat_file_info->modify_date, &fat_file_info->modify_time, &generic_file_info->modify_time);
    fat_generic_convert_date_fat_to_generic(&fat_file_info->last_access_date, NULL, &generic_file_info->access_time);

    heap_kernel_dealloc(fat_file_info);
    return true;
}

uint32_t fat_get_free_sector_index()
{
    int total_sectors = current_partition->header->total_sectors != 0 ?
        current_partition->header->total_sectors :
        current_partition->header->large_sectors;
        
    for(int i = 0; i < total_sectors; i++)
    {
        if(fat_read_sector_value(i) == 0)
        {
            return i;
        }
    }
    
    return 0;
}

bool fat_generic_get_directory_info(char *path, filesystem_directory_info *generic_directory_info)
{
    fat_directory_entry *fat_directory_info = fat_get_info_from_path(path, true);
    if (fat_directory_info == NULL)
    {
        return false;
    }

    fat_generic_copy_filename_to_generic(fat_directory_info->filename, generic_directory_info->name);
    memcpy(generic_directory_info->path, path, strlen(path) + 1);

    fat_generic_convert_date_fat_to_generic(&fat_directory_info->create_date, &fat_directory_info->create_time, &generic_directory_info->create_time);

    heap_kernel_dealloc(fat_directory_info);
    return true;
}

bool fat_generic_read_file(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length)
{
    return fat_read_file_from_path(path, buffer, start_index, length);
}

uint32_t fat_generic_get_entries_count_in_directory(char *path)
{
    return fat_get_entries_count_in_directory(path);
}

bool fat_generic_get_entries_in_directory(char *path, char **entries)
{
    return fat_get_entries_in_directory(path, entries);
}

bool fat_generic_is_file(char *path)
{
    fat_directory_entry *entry = fat_get_info_from_path(path, false);

    if (entry != NULL)
    {
        heap_kernel_dealloc(entry);
        return true;
    }

    return false;
}

bool fat_generic_is_directory(char *path)
{
    fat_directory_entry *entry = fat_get_info_from_path(path, true);

    if (entry != NULL)
    {
        heap_kernel_dealloc(entry);
        return true;
    }

    return false;
}

uint32_t fat_generic_get_free_space()
{
    uint32_t free_sectors = 0;
    for(int i = 0; i < current_partition->header->total_sectors; i++)
    {
        uint32_t sector = fat_read_sector_value(i);
        if(sector == 0)
        {
            free_sectors++;
        }
    }
    
    return free_sectors * current_partition->header->bytes_per_sector;
}

uint32_t fat_generic_get_total_space()
{
    return current_partition->header->total_sectors * current_partition->header->bytes_per_sector;
}

bool fat_generic_create_file(char *path)
{
    return fat_create_file_from_path(path, false);
}

bool fat_generic_create_directory(char *path)
{
    return fat_create_file_from_path(path, true);
}

bool fat_generic_delete_file(char *path)
{
    return fat_delete_file_from_path(path, false);
}

bool fat_generic_delete_directory(char *path)
{
    return fat_delete_file_from_path(path, true);
}

bool fat_generic_rename_file(char *path, char *new_name)
{
    return fat_rename_file_from_path(path, new_name, false);
}

bool fat_generic_rename_directory(char *path, char *new_name)
{
    return fat_rename_file_from_path(path, new_name, true);
}

bool fat_generic_save_to_file(char *path, char *buffer, int size)
{
    return fat_save_file_from_path(path, buffer, size);
}

bool fat_generic_append_to_file(char *path, char *buffer, int size)
{
    return fat_append_file_from_path(path, buffer, size);
}

uint8_t fat_generic_copy_filename_to_generic(char *fat_filename, char *generic_filename)
{
    uint8_t filename_length = 0;
    for (filename_length = 0; filename_length < 8; filename_length++)
    {
        char c = fat_filename[filename_length];
        if (c == ' ')
        {
            break;
        }

        generic_filename[filename_length] = c;
    }

    return filename_length;
}

void fat_generic_convert_date_fat_to_generic(fat_directory_entry_date *fat_date, fat_directory_entry_time *fat_time, filesystem_time *generic_time)
{
    if (fat_date != NULL)
    {
        generic_time->year = fat_date->year + 1980;
        generic_time->month = fat_date->month;
        generic_time->day = fat_date->day;
    }

    if (fat_time != NULL)
    {
        generic_time->hour = fat_time->hours;
        generic_time->minutes = fat_time->minutes;
        generic_time->seconds = fat_time->seconds * 2;
    }
}