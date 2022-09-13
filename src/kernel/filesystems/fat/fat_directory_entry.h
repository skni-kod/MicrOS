#ifndef FAT_DIRECTORY_ENTRY_H
#define FAT_DIRECTORY_ENTRY_H

#include <stdint.h>

typedef struct fat_directory_entry_file_attributes
{
    uint8_t readonly : 1;
    uint8_t hidden : 1;
    uint8_t system : 1;
    uint8_t volume_label : 1;
    uint8_t subdirectory : 1;
    uint8_t archive : 1;
    uint8_t device : 1;
    uint8_t reserved : 1;
} __attribute__((packed)) fat_directory_entry_file_attributes;

typedef struct fat_directory_entry_user_attribtes
{
    uint8_t read_requires_password : 1;
    uint8_t write_requires_password : 1;
    uint8_t delete_requires_password : 1;
    uint8_t reserved : 1;
    uint8_t disable_checksums : 1;
    uint8_t ignore_checksum_error : 1;
    uint8_t partial_close_default : 1;
    uint8_t modify_default_open_rules : 1;
} __attribute__((packed)) fat_directory_entry_user_attribtes;

typedef struct fat_directory_entry_time
{
    uint8_t seconds : 5;
    uint8_t minutes : 6;
    uint8_t hours : 5;
} __attribute__((packed)) fat_directory_entry_time;

typedef struct fat_directory_entry_date
{
    uint8_t day : 5;
    uint8_t month : 4;
    uint8_t year : 7;
} __attribute__((packed)) fat_directory_entry_date;

typedef struct fat_directory_entry_access_rights
{
    uint8_t owner_delete_rename_attribute_change : 1;
    uint8_t owner_execute : 1;
    uint8_t owner_write_modify : 1;
    uint8_t owner_read_copy : 1;
    uint8_t group_delete_rename_attribute_change : 1;
    uint8_t group_execute : 1;
    uint8_t group_write_modify : 1;
    uint8_t group_read_copy : 1;
    uint8_t world_delete_rename_attribute_change : 1;
    uint8_t world_execute : 1;
    uint8_t world_write_modify : 1;
    uint8_t world_read_copy : 1;
    uint8_t reserved : 4;
} __attribute__((packed)) fat_directory_entry_access_rights;

typedef struct directory_entry
{
    char filename[8];
    char extension[3];

    fat_directory_entry_file_attributes file_attributes;
    fat_directory_entry_user_attribtes user_attributes;

    uint8_t first_character;

    fat_directory_entry_time create_time;
    fat_directory_entry_date create_date;

    fat_directory_entry_date last_access_date;

    fat_directory_entry_access_rights access_rights;

    fat_directory_entry_time modify_time;
    fat_directory_entry_date modify_date;

    uint16_t first_sector;
    uint32_t size;
} __attribute__((packed)) fat_directory_entry;

#endif