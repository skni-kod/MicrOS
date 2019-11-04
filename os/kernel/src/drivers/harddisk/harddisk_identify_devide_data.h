#ifndef HARDDISK_IDENTIFY_DEVICE_DATA_H
#define HARDDISK_IDENTIFY_DEVICE_DATA_H

#include <stdint.h>

//! Length of hard disk serial number.
#define HARDDISK_SERIAL_NUMBER_LENGTH 20

//! Length of hard disk firmware version.
#define HARDDISK_FIRMWARE_VERSION_LENGTH 8

//! Length of hard disk model number.
#define HARDDISK_MODEL_NUMBER_LENGTH 40

//! Hard disk Identify Device Data.
/*! Struct contains all data about hard disk.*/
typedef struct harddisk_identify_device_data_fields
{
    // 0 word - General configuration bit - significant information
    uint8_t                                                  : 1;
    uint8_t                                                  : 1;
    //! Response incomplete.
    uint8_t response_incomplete                              : 1;
    uint8_t                                                  : 3;
    uint8_t                                                  : 1;
    //! 1 = removable media device.
    uint8_t removable_media_device                           : 1;
    uint8_t                                                  : 7;
    //! 0 = ATA device. 
    uint8_t ata_device                                       : 1;

    // 1 word
    uint16_t                                                 : 16;

    // 2 word
    //! Specific configuration
    uint16_t specific_configuration                          : 16;

    // 3 word
    uint16_t                                                 : 16;

    // 4 word
    uint16_t                                                 : 16;

    // 5 word
    uint16_t                                                 : 16;

    // 6 word
    uint16_t                                                 : 16;

    // 7-8 word
    //! Reserved for assignment by the CompactFlash Association
    uint32_t compactflash_association_reserved               : 32;

    // 9 word
    uint16_t                                                 : 16;

    // 10-19 word
    //! Serial number (20 ASCII characters)
    char serial_number[HARDDISK_SERIAL_NUMBER_LENGTH];

    // 20 word
    uint16_t                                                 : 16;

    // 21 word
    uint16_t                                                 : 16;

    // 22 word
    uint16_t                                                 : 16;

    // 23-26 word
    //! Firmware version (8 ASCII characters)
    char firmware_version[HARDDISK_FIRMWARE_VERSION_LENGTH];

    // 27-46 word
    //! Model number (40 ASCII characters)
    char model_number[HARDDISK_MODEL_NUMBER_LENGTH];

    // 47 word
    //! Value 80h
    uint8_t value_80h                                        : 8;
    //! 00h = Reserved. 01h-FFh - Maximum number of sectors that will be transfered per interrupt on READ/WRITE MULTIPLE commands.
    uint8_t max_number_of_sectors_per_interrupt              : 8;

    // 48 word
    //! Trusted Computing feature set is supported
    uint8_t trusted_computing_feature_set_supported          : 1;
    //! Reserved for Trusted Computing Group
    uint16_t trusted_computing_reserved                      : 13;
    //! Trusted Computing will be set to one
    uint8_t trusted_computing_set_to_one                     : 1;
    //! Trusted Computing will be set to zero
    uint8_t trusted_computing_set_to_zero                    : 1;

    // 49 word - Capabilities
    uint8_t                                                  : 8;
    //! DMA supported
    uint8_t dma_supported                                    : 1;
    //! LBA supported
    uint8_t lba_supported                                    : 1;
    //! IORDY may be disabled = 1 or may be supported = 0
    uint8_t iordy_may_be_disabled_or_supported               : 1;
    //! IORDY supported
    uint8_t iordy_supported                                  : 1;
    //! Reserved for IDENTIFY PACKET DEVICE command
    uint8_t                                                  : 1;
    //! 1 = standby timer values as specified in this standard are supported, 0 = standby timer values will be managed by device
    uint8_t standby_timer_type                               : 1;
    //! Reserved for IDENTIFY PACKET DEVICE command
    uint8_t                                                  : 2;

    // 50 word - Capabilities
    //! Will be set to one to indicate a device specific Standby timer value minimum
    uint8_t standby_timer_value_minimum                      : 1;
    uint8_t                                                  : 1;
    uint16_t                                                 : 12;
    //! Will be set to one
    uint8_t capabilities_set_to_one                          : 1;
    //! Will be set to zero
    uint8_t capabilities_set_to_zero                         : 1;

    // 51-52 word
    uint32_t                                                 : 32;

    // 53 word
    uint8_t                                                  : 1;
    //! Fields reported in words 64-70 are valid
    uint8_t fields_64_70_are_valid                           : 1;
    //! Fields reported in words 88 are valid
    uint8_t fields_88_are_valid                              : 1;
    uint8_t                                                  : 5;
    //! Free-fall Control Sensitivity
    uint8_t freefall_control_sensitivity                     : 8;

    // 54 word
    uint16_t                                                 : 16;
    
    // 55 word
    uint16_t                                                 : 16;
    
    // 56 word
    uint16_t                                                 : 16;
    
    // 57 word
    uint16_t                                                 : 16;
    
    // 58 word
    uint16_t                                                 : 16;

    // 59 word
    //! Current setting for number of logical sectors that will be trensfered per DRQ data block.
    uint8_t number_of_logical_sectors_per_drq                : 8;
    //! Multiple logical sector setting is valid
    uint8_t multiple_logical_sector_setting_is_valid         : 1;
    uint8_t                                                  : 3;
    //! The sanitize feature set is supported
    uint8_t the_sanitize_feature_set_is_supported            : 1;
    //! The CRYPTO SCRAMBLE TEXT command is supported
    uint8_t the_crypto_scramble_text_command_is_supported    : 1;
    //! The OVERWRITE TEXT command is supported
    uint8_t the_overwrite_text_command_is_supported          : 1;
    //! The BLOCK ERASE TEXT command is supported
    uint8_t the_block_erase_text_command_is_supported        : 1;

    // 60-61 word
    //! Total number of user addressable sectors
    uint32_t total_number_of_user_addressable_sectors        : 32;

    // 62 word
    uint16_t                                                 : 16;

    // 63 word
    //! 1 = Multiword DMA mode 0 is supported
    uint8_t multiword_dma_mode_0_is_supported                : 1;
    //! 1 = Multiword DMA mode 1 and below are supported
    uint8_t multiword_dma_mode_1_and_below_are_supported     : 1;
    //! 1 = Multiword DMA mode 2 and below are supported
    uint8_t multiword_dma_mode_2_and_below_are_supported     : 1;
    uint8_t                                                  : 5;
    //! 1 = Multiword DMA mode 0 is selected, 0 = Multiword DMA mode 0 is not selected
    uint8_t multiword_dma_mode_0_is_selected                 : 1;
    //! 1 = Multiword DMA mode 0 is selected, 0 = Multiword DMA mode 0 is not selected
    uint8_t multiword_dma_mode_1__is_selected                : 1;
    //! 1 = Multiword DMA mode 0 is selected, 0 = Multiword DMA mode 0 is not selected
    uint8_t multiword_dma_mode_2_is_selected                 : 1;
    uint8_t                                                  : 5;

} __attribute__((packed)) harddisk_identify_device_data_fields;

//! Hard disk Identify Device Data.
/*! Struct contains all data about hard disk.*/
typedef union harddisk_identify_device_data
{
    //! Fields of this union.
    harddisk_identify_device_data_fields fields;
    //! Values of this union.
    uint16_t values[256];
} harddisk_identify_device_data;

#endif