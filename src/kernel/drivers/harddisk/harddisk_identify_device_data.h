#ifndef HARDDISK_IDENTIFY_DEVICE_DATA_H
#define HARDDISK_IDENTIFY_DEVICE_DATA_H

#include <stdint.h>

//! Length of hard disk serial number.
#define HARDDISK_SERIAL_NUMBER_LENGTH 20

//! Length of hard disk firmware version.
#define HARDDISK_FIRMWARE_VERSION_LENGTH 8

//! Length of hard disk model number.
#define HARDDISK_MODEL_NUMBER_LENGTH 40

//! General configuration bit.
typedef struct general_configuration_fields
{      
    // Reserved
    uint8_t                                                  : 1;
    // Retried
    uint8_t                                                  : 1;
    //! Response incomplete.
    uint8_t response_incomplete                              : 1;
    // Retried
    uint8_t                                                  : 3;
    // Obsolete
    uint8_t                                                  : 1;
    //! 1 = removable media device.
    uint8_t removable_media_device                           : 1;
    // Retried
    uint8_t                                                  : 7;
    //! 0 = ATA device. 
    uint8_t ata_device                                       : 1;
} __attribute__((packed)) general_configuration_fields;

//! Trusted Computing.
typedef struct trusted_computing_fields
{
    //! Trusted Computing feature set is supported
    uint8_t trusted_computing_feature_set_supported          : 1;
    //! Reserved for Trusted Computing Group
    uint16_t trusted_computing_reserved                      : 13;
    //! Trusted Computing will be set to one
    uint8_t trusted_computing_set_to_one                     : 1;
    //! Trusted Computing will be set to zero
    uint8_t trusted_computing_set_to_zero                    : 1;
} __attribute__((packed)) trusted_computing_fields;

//! Capabilities.
typedef struct capabilities_fields
{
    // Retired
    uint8_t                                                  : 8;
    //! DMA supported
    uint8_t dma_supported                                    : 1;
    //! LBA supported
    uint8_t lba_supported                                    : 1;
    //! IORDY may be disabled = 1 or may be supported = 0
    uint8_t iordy_may_be_disabled_or_supported               : 1;
    //! IORDY supported
    uint8_t iordy_supported                                  : 1;
    // Reserved for IDENTIFY PACKET DEVICE command
    uint8_t                                                  : 1;
    //! 1 = standby timer values as specified in this standard are supported, 0 = standby timer values will be managed by device
    uint8_t standby_timer_type                               : 1;
    // Reserved for IDENTIFY PACKET DEVICE command
    uint8_t                                                  : 2;

    //! Will be set to one to indicate a device specific Standby timer value minimum
    uint8_t standby_timer_value_min                          : 1;
    // Obsolete
    uint8_t                                                  : 1;
    // Reserved
    uint16_t                                                 : 12;
    //! Will be set to one
    uint8_t capabilities_set_to_one                          : 1;
    //! Will be set to zero
    uint8_t capabilities_set_to_zero                         : 1;
} __attribute__((packed)) capabilities_fields;

//! Additional supported.
typedef struct additional_supported_fields
{  
    // Reserved
    uint8_t                                                  : 2;
    //! All write cache is non-volatile.
    uint8_t all_write_cache_is_non_volatile                  : 1;
    //! Extended Number of User Addressable Sectors is supported
    uint8_t extended_number_of_user_addressable_sectors_is_supported : 1;
    //! Device Encrypts All User Data on the device.
    uint8_t device_encrypts_all_user_data_on_the_device      : 1;
    //! Trimmed LBA range(s) returning zeroed data is supported
    uint8_t trimmed_lba_range_returning_zeroed_data_is_supported : 1;
    //! O = optional ATA device 28-bit commands supported.
    uint8_t optional_ata_device_28_bit_command_support       : 1;
    //! Reserved for IEEE 1667.
    uint8_t                                                  : 1;
    //! DOWNLOAD MICROCODE DMA is supported.
    uint8_t download_microcode_dma_is_supported              : 1;
    //! Obsolete
    uint8_t                                                  : 1;
    //! WRITE BUFFER DMA is supported.
    uint8_t write_buffer_dma_is_supported                    : 1;
    //! READ BUFFER DMA is supported.
    uint8_t read_buffer_dma_is_supported                     : 1;
    //! Obsolete
    uint8_t                                                  : 1;
    //! Long Physical Sector Alignment Error Reporting Control is supported.
    uint8_t long_physical_sector_alignment_error_reporting_control_is_supported : 1;
    //! Deterministic data is trimmed LBA range(s) is supported.
    uint8_t determinic_read_after_trim_supported             : 1;
    // Reserved for CFA.
    uint8_t                                                  : 1;
} __attribute__((packed)) additional_supported_fields;

//! Serial ATA capabilities.
typedef struct serial_ata_capabilities_fields
{
    
    // Will be cleared to zero.
    uint8_t                                                  : 1;
    //! Supports SATA Gen1 Signaling Speed.
    uint8_t sata_gen1                                        : 1;
    //! Supports SATA Gen2 Signaling Speed.
    uint8_t sata_gen2                                        : 1;
    //! Supports SATA Gen3 Signaling Speed.
    uint8_t sata_gen3                                        : 1;
    //! Reserved for SATA.
    uint8_t                                                  : 4;
    //! Supports NCQ feature set.
    uint8_t ncq                                              : 1;
    //! Supports receipt of host initiated power management requests.
    uint8_t hipm                                             : 1;
    //! Supports the SATA Phy Event Counters log.
    uint8_t phy_event_counters                               : 1;
    //! Supports Unload while NCQ commands are outstanding.
    uint8_t unload_while_ncq                                 : 1;
    //! Supports NCQ priority information.
    uint8_t ncq_priority                                     : 1;
    //! Supports Host Automatic Partial to Slumber transitions.
    uint8_t host_auto_ps_transactions                        : 1;
    //! Supports Device Automatic Partial to Slumber transitions.
    uint8_t device_auto_ps_transactions                      : 1;
    //! Supports READ LOG DMA EXT as equivalent to READ LOG EXT.
    uint8_t read_log_dma_ext                                 : 1;

    // Will be cleared to zero.
    uint8_t                                                  : 1;
    //! Coded value indicating current negotiated Serial ATA signal speed.
    uint8_t coded_value_for_current_sata_speed               : 3;
    //! Supports NCQ streaming.
    uint8_t ncq_streaming                                    : 1;
    //! Supports NCQ Queue Management Command.
    uint8_t mcq_queue_management_command                     : 1;
    //! Supports RECEIVE FPDMA QUEUED and SEND FPDMA QUEUED commands.
    uint8_t receive_send_fpdma_queued                        : 1;
    // Reserved for Serial ATA.
    uint16_t                                                 : 9;
} __attribute__((packed)) serial_ata_capabilities_fields;

//! Serial ATA features supported.
typedef struct serial_ata_features_supported_fields
{  
    // Will be cleared to zero.
    uint8_t                                                  : 1;
    //! Device supports non-zero buffer offset.
    uint8_t non_zero_buffer_offset                           : 1;
    //! Device supports DMA setup auto-activation.
    uint8_t dma_setup_auto_activation                        : 1;
    //! Device supports initiating power management.
    uint8_t initiating_power_management                      : 1;
    //! Device supports in-order data delivery.
    uint8_t in_order_data_delivery                           : 1;
    //! Device supports Hardware Feature Control.
    uint8_t hardware_feature_control                         : 1;
    //! Device supports Hardware Settings Preservation.
    uint8_t hardware_setting_preservation                    : 1;
    //! Device supports NCQ Autosense.
    uint8_t ncq_autosense                                    : 1;
    // Reserved for Serial ATA.
    uint8_t                                                  : 8;
} __attribute__((packed)) serial_ata_features_supported_fields;

//! Serial ATA features enabled.
typedef struct serial_ata_features_enabled_fields
{
    
    // Will be cleared to zero.
    uint8_t                                                  : 1;
    //! Non-zero buffer offset enabled.
    uint8_t non_zero_buffer_offset                           : 1;
    //! DMA setup auto-activation enabled.
    uint8_t dma_setup_auto_activation                        : 1;
    //! Initiating power management enabled.
    uint8_t initiating_power_management                      : 1;
    //! In-order data delivery enabled.
    uint8_t in_order_data_delivery                           : 1;
    //! Hardware Feature Control enabled.
    uint8_t hardware_feature_control                         : 1;
    //! Hardware Settings Preservation enabled.
    uint8_t hardware_setting_preservation                    : 1;
    //! Automatic Partial to Slumber transitions enabled.
    uint8_t auto_ps_transictions                             : 1;
    // Reserved for Serial ATA.
    uint8_t                                                  : 8;
} __attribute__((packed)) serial_ata_features_enabled_fields;

//! Major revision.
typedef struct major_revision_fields
{
    // Reserved
    uint8_t                                                  : 1;
    // Obsolete
    uint8_t                                                  : 4;
    //! Supports ATA/ATAPI-5.
    uint8_t supports_ata_atapi_5                             : 1;
    //! Supports ATA/ATAPI-6.
    uint8_t supports_ata_atapi_6                             : 1;
    //! Supports ATA/ATAPI-7.
    uint8_t supports_ata_atapi_7                             : 1;
    //! Supports ATA8-ACS.
    uint8_t supports_ata8_acs                                : 1;
    //! Supports ACS-2.
    uint8_t supports_acs_2                                   : 1;
    //! Supports ACA-3
    uint8_t supports_acs_3                                   : 1;
    // Reserved
    uint8_t                                                  : 5;
} __attribute__((packed)) major_revision_fields;

//! Command Set Support.
typedef struct command_set_support_fields
{
    //! SMART feature set supported.
    uint8_t smart                                            : 1;
    //! Security Mode feature set supported.
    uint8_t security_mode                                    : 1;
    //! Removable Media feature set supported.
    uint8_t removable_media                                  : 1;
    //! Mandatory Power Management feature set supported.
    uint8_t mandatory_power_management                       : 1;
    //! PACKET Command feature set not supported if cleared to zero.
    uint8_t packet_command                                   : 1;
    //! Write cache supported.
    uint8_t write_cache                                      : 1;
    //! Look-ahead supported.
    uint8_t look_ahead                                       : 1;
    //! Release interrupt supported.
    uint8_t release_interrupt                                : 1;
    //! SERVICE interrupt supported.
    uint8_t service_interrupt                                : 1;
    //! DEVICE RESET command supported.
    uint8_t device_reset_command                             : 1;
    //! Host Protected Area feature set supported.
    uint8_t host_protected_area                              : 1;
    //! Obsolete
    uint8_t                                                  : 1;
    //! WRITE BUFFER command supported.
    uint8_t write_buffer_command                             : 1;
    //! READ BUFFER command supported.
    uint8_t read_buffer_command                              : 1;
    //! NOP command supported.
    uint8_t nop_command                                      : 1;
    //! Obsolete
    uint8_t                                                  : 1;

    //! DOWNLOAD MICROCODE command supported.
    uint8_t download_microcode                               : 1;
    //! READ/WRITE DMA QUEUED supported.
    uint8_t read_write_dma_queued                            : 1;
    //! CFA feature set supported.
    uint8_t cfa                                              : 1;
    //! Advanced Power Management feature set supported.
    uint8_t advanced_power_management                        : 1;
    //! Removable Media Status Notification feature set supported.
    uint8_t removable_media_status_notification              : 1;
    //! Power-up In Standby feature set supported.
    uint8_t powerup_in_standby                               : 1;
    //! SET FEATURES subcommand required to spinup after power-up.
    uint8_t manual_powerup                                   : 1;
    //! See Address Offset Reserved Area Boot.
    uint8_t                                                  : 1;
    //! SET MAX security extension supported.
    uint8_t set_max                                          : 1;
    //! Automatic Acoustic Management feature set supported.
    uint8_t automatic_acoustic_management                    : 1;
    //! 48-bit Address feature set supported.
    uint8_t address_48_bit                                   : 1;
    //! Device Configuration Overlay feature set supported.
    uint8_t device_configuration_overlay                     : 1;
    //! Mandatory FLUSH CACHE command supported.
    uint8_t mandatory_flush_cache                            : 1;
    //! FLUSH CACHE EXT command supported.
    uint8_t flush_cache_ext                                  : 1;
    // Will be cleared to one.
    uint8_t                                                  : 1;
    // Will be cleared to zero.
    uint8_t                                                  : 1;

    //! SMART error logging supported.
    uint8_t smart_error_logging                              : 1;
    //! SMART seft-test supported.
    uint8_t smart_self_test                                  : 1;
    //! Media serial number supported.
    uint8_t media_serial_number                              : 1;
    //! Media Card Pass Through Command feature set supported.
    uint8_t media__card_pass_through                         : 1;
    //! Streaming feature set supported.
    uint8_t streaming                                        : 1;
    //! General Purpose Logging feature set supported.
    uint8_t general_purpose_logging                          : 1;
    //! WRITE DMA FUA EXT and WRITE MULTIPLE FUA EXT commands supported.
    uint8_t write_dma_fua_ext                                : 1;
    //! WRITE DMA QUEUED FUA EXT command supported.
    uint8_t write_dma_queued_fua_ext                         : 1;
    //! 64-bit Worldwide name supported.
    uint8_t worldwide_name_64_bit                            : 1;
    //! URG bit supported for READ STREAM DMA EXT and READ STREAM EXT.
    uint8_t urg_for_read_stream                              : 1;
    //! URG bit supported for WRITE STREAM DMA EXT and WRITE STREAM EXT.
    uint8_t urg_for_write_stream                             : 1;
    //! Reserved for technical report.
    uint8_t reserved_for_tech_report_1                       : 1;
    //! Reserved for technical report.
    uint8_t reserved_for_tech_report_2                       : 1;
    //! 1 = IDLE IMMEDIATE with UNLOAD FEATURE supported.
    uint8_t idle_with_unload_feature                         : 1;
    // Will be cleared to one.
    uint8_t                                                  : 1;
    // Will be cleared to zero.
    uint8_t                                                  : 1;
} __attribute__((packed)) command_set_support_fields;

//! Command Set Active.
typedef struct command_set_active_fields
{
    //! SMART feature set enabled.
    uint8_t smart                                            : 1;
    //! Security Mode feature set enabled.
    uint8_t security_mode                                    : 1;
    //! Removable Media feature set enabled.
    uint8_t removable_media                                  : 1;
    //! Mandatory Power Management feature set enabled.
    uint8_t mandatory_power_management                       : 1;
    //! PACKET Command feature set not supported if cleared to zero.
    uint8_t packet_command                                   : 1;
    //! Write cache enabled.
    uint8_t write_cache                                      : 1;
    //! Look-ahead enabled.
    uint8_t look_ahead                                       : 1;
    //! Release interrupt enabled.
    uint8_t release_interrupt                                : 1;
    //! SERVICE interrupt enabled.
    uint8_t service_interrupt                                : 1;
    //! DEVICE RESET command enabled.
    uint8_t device_reset_command                             : 1;
    //! Host Protected Area feature set enabled.
    uint8_t host_protected_area                              : 1;
    //! Obsolete
    uint8_t                                                  : 1;
    //! WRITE BUFFER command enabled.
    uint8_t write_buffer_command                             : 1;
    //! READ BUFFER command enabled.
    uint8_t read_buffer_command                              : 1;
    //! NOP command enabled.
    uint8_t nop_command                                      : 1;
    //! Obsolete
    uint8_t                                                  : 1;

    //! DOWNLOAD MICROCODE command supported.
    uint8_t download_microcode                               : 1;
    //! READ/WRITE DMA QUEUED supported.
    uint8_t read_write_dma_queued                            : 1;
    //! CFA feature set enabled.
    uint8_t cfa                                              : 1;
    //! Advanced Power Management feature set enabled.
    uint8_t advanced_power_management                        : 1;
    //! Removable Media Status Notification feature set enabled.
    uint8_t removable_media_status_notification              : 1;
    //! Power-up In Standby feature set enabled.
    uint8_t powerup_in_standby                               : 1;
    //! SET FEATURES subcommand required to spinup after power-up.
    uint8_t manual_powerup                                   : 1;
    //! See Address Offset Reserved Area Boot.
    uint8_t                                                  : 1;
    //! SET MAX security extension enabled by SET MAX SET PASSWORD.
    uint8_t set_max                                          : 1;
    //! Automatic Acoustic Management feature set enabled.
    uint8_t automatic_acoustic_management                    : 1;
    //! 48-bit Address feature set supported.
    uint8_t address_48_bit                                   : 1;
    //! Device Configuration Overlay feature set supported.
    uint8_t device_configuration_overlay                     : 1;
    //! Mandatory FLUSH CACHE command supported.
    uint8_t mandatory_flush_cache                            : 1;
    //! FLUSH CACHE EXT command supported.
    uint8_t flush_cache_ext                                  : 1;
    // Reserved.
    uint8_t                                                  : 2;

    //! SMART error logging supported.
    uint8_t smart_error_logging                              : 1;
    //! SMART seft-test supported.
    uint8_t smart_self_test                                  : 1;
    //! Media serial number is valid.
    uint8_t media_serial_number_valid                        : 1;
    //! Media Card Pass Through Command feature set enabled.
    uint8_t media__card_pass_through                         : 1;
    //! Valid CONFIGURE STREAM command has been executed.
    uint8_t configure_stream_executed                        : 1;
    //! General Purpose Logging feature set supported.
    uint8_t general_purpose_logging                          : 1;
    //! WRITE DMA FUA EXT and WRITE MULTIPLE FUA EXT commands supported.
    uint8_t write_dma_fua_ext                                : 1;
    //! WRITE DMA QUEUED FUA EXT command supported.
    uint8_t write_dma_queued_fua_ext                         : 1;
    //! 64-bit Worldwide name supported.
    uint8_t worldwide_name_64_bit                            : 1;
    //! URG bit supported for READ STREAM DMA EXT and READ STREAM EXT.
    uint8_t urg_for_read_stream                              : 1;
    //! URG bit supported for WRITE STREAM DMA EXT and WRITE STREAM EXT.
    uint8_t urg_for_write_stream                             : 1;
    // Reserved for technical report.
    uint8_t                                                  : 1;
    // Reserved for technical report.
    uint8_t                                                  : 1;
    //! 1 = IDLE IMMEDIATE with UNLOAD FEATURE supported.
    uint8_t idle_with_unload_feature                         : 1;
    // Will be cleared to one.
    uint8_t                                                  : 1;
    // Will be cleared to zero.
    uint8_t                                                  : 1;
} __attribute__((packed)) command_set_active_fields;

//! Ultra DMA modes
typedef struct ultra_dma_modes_fields
{
    //! Ultra DMA mode 0 is supported
    uint8_t ultra_dma_mode_0_supported                       : 1;
    //! Ultra DMA mode 1 and below are supported
    uint8_t ultra_dma_mode_1_and_below_supported             : 1;
    //! Ultra DMA mode 2 and below are supported
    uint8_t ultra_dma_mode_2_and_below_supported             : 1;
    //! Ultra DMA mode 3 and below are supported
    uint8_t ultra_dma_mode_3_and_below_supported             : 1;
    //! Ultra DMA mode 4 and below are supported
    uint8_t ultra_dma_mode_4_and_below_supported             : 1;
    //! Ultra DMA mode 5 and below are supported
    uint8_t ultra_dma_mode_5_and_below_supported             : 1;
    //! Ultra DMA mode 6 and below are supported
    uint8_t ultra_dma_mode_6_and_below_supported             : 1;
    // Reserved
    uint8_t                                                  : 1;
    //! Ultra DMA mode 0 is selected
    uint8_t ultra_dma_mode_0_selected                        : 1;
    //! Ultra DMA mode 1 is selected
    uint8_t ultra_dma_mode_1_selected                        : 1;
    //! Ultra DMA mode 2 is selected
    uint8_t ultra_dma_mode_2_selected                        : 1;
    //! Ultra DMA mode 3 is selected
    uint8_t ultra_dma_mode_3_selected                        : 1;
    //! Ultra DMA mode 4 is selected
    uint8_t ultra_dma_mode_4_selected                        : 1;
    //! Ultra DMA mode 5 is selected
    uint8_t ultra_dma_mode_5_selected                        : 1;
    //! Ultra DMA mode 6 is selected
    uint8_t ultra_dma_mode_6_selected                        : 1;
    // Reserved
    uint8_t                                                  : 1;
} __attribute__((packed)) ultra_dma_modes_fields;

//! Harware reset result
typedef struct hardware_reset_result_fields
{  
    //! Device 0 hardware reset result.
    /*! Device 1 will clear these bits to zero. Device 0 will set these bits as follows: */
    struct
    {
        // Will be cleared to one.
        uint8_t                                                  : 1;
        //! These bits indicate how device 0 determined the device number.
        /*! 00 = reserved<br>
        01 = A jumper was used<br>
        10 = The CSEL signal was used<br>
        11 = Some other method was used or method is unknown*/
        uint8_t how_determined_device_0_number                   : 2;
        //! Device 0 passed diagnostics.
        uint8_t passed_diagnostics                               : 1;
        //! Device 0 detected the assertion of PDIAG.
        uint8_t assertion_of_pdiag                               : 1;
        //! Device 0 detected the assertion of DASP.
        uint8_t assertion_of_dasp                                : 1;
        //! Device 0 respond when Device 1 is selected.
        uint8_t respond_when_device_1                            : 1;
        // Reserved
        uint8_t                                                  : 1;
    } hardware_device_0_reset_result_fields;
    //! Device 1 hardware reset result.
    /*! Device 0 will clear these bits to zero. Device 1 will set these bits as follows: */
    struct
    {
        // Will be cleared to one.
        uint8_t                                                  : 1;
        //! These bits indicate how device 1 determined the device number.
        /*! 00 = reserved<br>
        01 = A jumper was used<br>
        10 = The CSEL signal was used<br>
        11 = Some other method was used or method is unknown*/
        uint8_t how_determined_device_1_number                   : 2;
        //! Device 1 asserted PDIAG.
        uint8_t asserted_pdiag                                   : 1;
        // Reserved
        uint8_t                                                  : 1;
    } hardware_device_1_reset_result_fields;
    //! 1 = device detected CBLID - above V_iH, 0 = device detected CBLID - below V_iL.
    uint8_t cblid_detection                                  : 1;
    // Will be cleared to one.
    uint8_t                                                  : 1;
    // Will be cleared to zero.
    uint8_t                                                  : 1;
} __attribute__((packed)) hardware_reset_result_fields;

//! Physical/Logical Sector Size.
typedef struct physical_logical_sector_size_fields
{
    
    //! 2^x logical sectors per physical sector.
    uint8_t logical_per_physical_sectors                     : 4;
    // Reserved
    uint8_t logical_sector_per_physical_sectors              : 8;
    //! Device Logical Sector Longer than 256 Words.
    uint8_t logical_sector_longer_than_256_words             : 4;
    //! Device has multiple Logical Sector per physical sector.
    uint8_t multiple_logical_sector_per_physical_sector      : 4;
    // Will be cleared to one.
    uint8_t                                                  : 1;
    // Will be cleared to zero.
    uint8_t                                                  : 1;
} __attribute__((packed)) physical_logical_sector_size_fields;

//! Worldwide name.
typedef struct worldwide_name_fields
{
    //! IEEE UI 12-23
    uint16_t ieee_oui_12_23                                  : 12;
    //! NAA 0-3
    uint16_t naa_0_3                                         : 4;

    //! Unique ID 32-35
    uint16_t unique_id_32_35                                 : 4;
    //! IEEE UI 0_11
    uint16_t ieee_oui_0_11                                   : 12;

    //! Unique ID 16-31
    uint16_t unique_id_16_31                                 : 16;

    //! Unique ID 0-15
    uint16_t unique_id_0_15                                  : 16;
} __attribute__((packed)) worldwide_name_fields;

//! Security status.
typedef struct security_status_fields
{    
    //! Security supported.
    uint8_t security_supported                               : 1;
    //! Security enabled.
    uint8_t security_enabled                                 : 1;
    //! Security locked.
    uint8_t security_locked                                  : 1;
    //! Security frozen.
    uint8_t security_frozen                                  : 1;
    //! Security count expired.
    uint8_t security_count_expired                           : 1;
    //! Enhanced security erase supported.
    uint8_t enhanced_security_erase_supported                : 1;
    // Reserved
    uint8_t                                                  : 2;
    //! Security level.
    /*! 0 = high<br>
    1 = maximum*/
    uint8_t security_level                                   : 1;
    // Reserved
    uint8_t                                                  : 7;
} __attribute__((packed)) security_status_fields;

//! CFA power mode 1
typedef struct cfa_power_mode_1_fields
{ 
    //! Maximum current in ma.
    uint16_t maximum_current_in_ma                           : 12;
    //! CFA power mode 1 is disabled.
    uint8_t cfa_power_mode_1_disabled                        : 1;
    //! CFA power mode 1 is required for one or more commands implemented by the device.
    uint8_t cfa_powe_mode_1_required                         : 1;
    // Reserved
    uint8_t                                                  : 1;
    //! Word 160 supported.
    uint8_t word_160_supported                               : 1;
} __attribute__((packed)) cfa_power_mode_1_fields;

//! Hard disk Identify Device Data.
/*! Struct contains all data about hard disk.*/
typedef struct harddisk_identify_device_data_fields
{
    // 0 word
    //! General configuration bit.
    general_configuration_fields general_configuration;

    // 1 word
    // Obsolete
    uint16_t                                                 : 16;

    // 2 word
    //! Specific configuration
    uint16_t specific_configuration                          : 16;

    // 3 word
    // Obsolete
    uint16_t                                                 : 16;

    // 4 word
    // Retried
    uint16_t                                                 : 16;

    // 5 word
    // Retried
    uint16_t                                                 : 16;

    // 6 word
    // Obsolete
    uint16_t                                                 : 16;

    // 7-8 word
    // Reserved for assignment by the CompactFlash Association
    uint32_t                                                 : 32;

    // 9 word
    // Retried
    uint16_t                                                 : 16;

    // 10-19 word
    //! Serial number (20 ASCII characters)
    char serial_number[HARDDISK_SERIAL_NUMBER_LENGTH];

    // 20 word
    // Retried
    uint16_t                                                 : 16;

    // 21 word
    // Retried
    uint16_t                                                 : 16;

    // 22 word
    // Obsolete
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
    //! Trusted Computing.
    trusted_computing_fields trusted_computing;

    // 49-50 word
    //! Capabilities.
    capabilities_fields capabilities;

    // 51 word
    // Obsolete
    uint16_t                                                 : 16;

    // 52 word
    // Obsolete
    uint16_t                                                 : 16;

    // 53 word
    // Obsolete
    uint8_t                                                  : 1;
    //! Fields reported in words 64-70 are valid
    uint8_t fields_64_70_are_valid                           : 1;
    //! Fields reported in words 88 are valid
    uint8_t fields_88_are_valid                              : 1;
    // Reserved
    uint8_t                                                  : 5;
    //! Free-fall Control Sensitivity
    uint8_t freefall_control_sensitivity                     : 8;

    // 54 word
    // Obsolete
    uint16_t                                                 : 16;
    
    // 55 word
    // Obsolete
    uint16_t                                                 : 16;
    
    // 56 word
    // Obsolete
    uint16_t                                                 : 16;
    
    // 57 word
    // Obsolete
    uint16_t                                                 : 16;
    
    // 58 word
    // Obsolete
    uint16_t                                                 : 16;

    // 59 word
    //! Current setting for number of logical sectors that will be trensfered per DRQ data block.
    uint8_t number_of_logical_sectors_per_drq                : 8;
    //! Multiple logical sector setting is valid
    uint8_t multiple_logical_sector_setting_is_valid         : 1;
    // Reserved
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
    // Obsolete
    uint16_t                                                 : 16;

    // 63 word
    //! 1 = Multiword DMA mode 0 is supported
    uint8_t multiword_dma_mode_0_is_supported                : 1;
    //! 1 = Multiword DMA mode 1 and below are supported
    uint8_t multiword_dma_mode_1_and_below_are_supported     : 1;
    //! 1 = Multiword DMA mode 2 and below are supported
    uint8_t multiword_dma_mode_2_and_below_are_supported     : 1;
    // Reserved
    uint8_t                                                  : 5;
    //! 1 = Multiword DMA mode 0 is selected, 0 = Multiword DMA mode 0 is not selected
    uint8_t multiword_dma_mode_0_is_selected                 : 1;
    //! 1 = Multiword DMA mode 0 is selected, 0 = Multiword DMA mode 0 is not selected
    uint8_t multiword_dma_mode_1__is_selected                : 1;
    //! 1 = Multiword DMA mode 0 is selected, 0 = Multiword DMA mode 0 is not selected
    uint8_t multiword_dma_mode_2_is_selected                 : 1;
    // Reserved
    uint8_t                                                  : 5;

    // 64 word
    //! PIO modes supported
    uint8_t pio_modes_supported                              : 8;
    // Reserved
    uint8_t                                                  : 8;

    // 65 word
    //! Minimum multiword DMA transfer cycle time per word
    uint16_t min_multiword_DMA_transfer_cycle_time_per_word : 16;

    // 66 word
    //! Manufacturers recommended multiword DMA transfer cycle time
    uint16_t manufaturers_recommended_multiword_DMA_transfer_cycle_time : 16;

    // 67 word
    //! Minimum PIO transfer cycle time without flow control
    uint16_t min_pio_transfer_cycle_time_without_flow_control : 16;

    // 68 word
    //! Minimum PIO transfer cycle time with IORDT flow control
    uint16_t min_pio_transfer_cycle_time_with_iordy_flow_control : 16;

    // 69 word
    //! Additional supported.
    additional_supported_fields additional_supported;

    // 70 word
    // Reserved
    uint16_t                                                 : 16;

    // 71 word
    // Reserved for the IDENTITY PACKET DEVICE command.
    uint16_t                                                 : 16;

    // 72 word
    // Reserved for the IDENTITY PACKET DEVICE command.
    uint16_t                                                 : 16;

    // 73 word
    // Reserved for the IDENTITY PACKET DEVICE command.
    uint16_t                                                 : 16;

    // 74 word
    // Reserved for the IDENTITY PACKET DEVICE command.
    uint16_t                                                 : 16;

    // 75 word
    //! Maximum queue depth - 1
    uint8_t queue_depth                                      : 5;
    // Reserved
    uint16_t                                                 : 11;

    // 76-77 word
    //! Serial ATA capabilities.
    serial_ata_capabilities_fields serial_ata_capabilities;

    // 78 word
    //! Serial ATA features supported.
    serial_ata_features_supported_fields serial_ata_features_supported;

    // 79 word
    //! Serial ATA features enabled.
    serial_ata_features_enabled_fields serial_ata_features_enabled;

    // 80 word
    //! Major revision.
    major_revision_fields major_revision;

    // 81 word
    //! Minor revision.
    uint16_t minor_revision                                  : 16;

    // 82-84 word
    //! Command Set Support.
    command_set_support_fields command_set_support;

    // 85-87 word
    //! Command Set Active.
    command_set_active_fields command_set_active;

    // 88 word
    //! Ultra DMA modes
    ultra_dma_modes_fields ultra_dma_modes;

    // 89 word
    //! Time required for security erase unit completion.
    uint16_t time_required_for_security_erase                : 16;

    // 90 word
    //! Time required for Enhanced security erase unit completion.
    uint16_t time_required_for_enhanced_security_erase       : 16;

    // 91 word
    //! Current advanced power management value
    uint16_t current_advanced_power_management_value         : 16;

    // 92 word
    //! Master Password Revision Code
    uint16_t master_password_revision_code                   : 16;

    // 93 word
    //! Harware reset result
    hardware_reset_result_fields hardware_reset_result;

    // 94 word
    //! Current automatic acoustic management value.
    uint8_t current_acoustic_value                           : 8;
    //! Vendor's recommended acoustic management value.
    uint8_t recommended_acoustic_value                       : 8;

    // 95 word
    //! Stream Minimum Request Size.
    uint16_t stream_min_request_size                         : 16;

    // 96 word
    //! Streaming Transfer Time - DMA.
    uint16_t steraming_transfer_time_dma                     : 16;

    // 97 word
    //! Streaming Access Lantecy - DMA and PIO.
    uint16_t steraming_access_latency_dma_pio                : 16;

    // 98-99 word
    //! Streaming Performance Granularity.
    uint32_t steraming_performance_granularity               : 32;

    // 100-103 word
    //! Maximum user LBA for 48-bit Address feature set.
    uint64_t maximum_user_lba_for_48_bit                     : 64;

    // 104 word
    //! Streaming Transfer Time - PIO.
    uint16_t steraming_transfer_time_pio                     : 16;

    // 105 word
    // Reserved
    uint16_t                                                 : 16;

    // 106 word
    //! Physical/Logical Sector Size.
    physical_logical_sector_size_fields physical_logical_sector_size;

    // 107 word
    //! Inter-seek delay for ISO-7770 acoustic testing in microseconds.
    uint16_t inter_seek_delay                                : 16;

    // 108-111 word
    //! Worldwide name.
    worldwide_name_fields worldwide_name;

    // 112-115 word
    // Reserved for worldwide name extension to 128 bits.
    uint64_t                                                 : 64;

    // 116 word
    // Reserved for technical report.
    uint16_t                                                 : 16;

    // 117-118 word
    //! Words per Logical Sector.
    uint32_t words_per_logical_sector                        : 32;

    // 119 word
    // Reserved
    uint16_t                                                 : 16;

    // 120 word
    // Reserved
    uint16_t                                                 : 16;

    // 121 word
    // Reserved
    uint16_t                                                 : 16;

    // 122 word
    // Reserved
    uint16_t                                                 : 16;

    // 123 word
    // Reserved
    uint16_t                                                 : 16;

    // 124 word
    // Reserved
    uint16_t                                                 : 16;

    // 125 word
    // Reserved
    uint16_t                                                 : 16;

    // 126 word
    // Reserved
    uint16_t                                                 : 16;

    // 127 word
    //! Removable Media Status Notification feature set support
    /*! 00 = not supported<br>
    01 = supported<br>
    10 = reserved<br>
    11 = reserved<br>*/
    uint8_t removable_media_status_notification_support      : 2;
    // Reserved
    uint16_t                                                 : 14;

    // 128 word
    //! Security status.
    security_status_fields security_status;

    // 129-159 word
    //! Vendor specific
    uint16_t vendor_specific[31];

    // 160 word
    //! CFA power mode 1
    cfa_power_mode_1_fields cfa_power_mode_1;

    // 161-175 word
    // Reserved for assignment by the CompactFlash Association.
    uint16_t reserved_161_175[15];

    // 176-205 word
    //! Current media serial number.
    uint16_t current_media_serial_number[30];

    // 206-254 word
    // Reserved
    uint16_t reserved_206_254[49];

    // 255 word
    //! Signature
    uint8_t signature                                        : 8;
    //! Checksum
    uint8_t checksum                                         : 8;

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