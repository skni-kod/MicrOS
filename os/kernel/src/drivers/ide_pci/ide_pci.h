#ifndef PCI_IDE_H
#define PCI_IDE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../pci/pci.h"
#include "../../assembly/io.h"
#include "../../logger/logger.h"
#include <ktime.h>

#define IDE_PCI_DEVICE_CLASS 1
#define IDE_PCI_DEVICE_SUBCLASS 1

//The following constants are derived from wiki.osdev.org, and should be treated with care and distrust

//Status
#define ATA_SR_BSY     0x80    // Busy
#define ATA_SR_DRDY    0x40    // Drive ready
#define ATA_SR_DF      0x20    // Drive write fault
#define ATA_SR_DSC     0x10    // Drive seek complete
#define ATA_SR_DRQ     0x08    // Data request ready
#define ATA_SR_CORR    0x04    // Corrected data
#define ATA_SR_IDX     0x02    // Index
#define ATA_SR_ERR     0x01    // Error

//Errors
#define ATA_ER_BBK      0x80    // Bad block
#define ATA_ER_UNC      0x40    // Uncorrectable data
#define ATA_ER_MC       0x20    // Media changed
#define ATA_ER_IDNF     0x10    // ID mark not found
#define ATA_ER_MCR      0x08    // Media change request
#define ATA_ER_ABRT     0x04    // Command aborted
#define ATA_ER_TK0NF    0x02    // Track 0 not found
#define ATA_ER_AMNF     0x01    // No address mark

//Commands
#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC

#define ATAPI_CMD_READ       0xA8
#define ATAPI_CMD_EJECT      0x1B

//Identification
#define ATA_IDENT_DEVICETYPE   0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID   106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMANDSETS  164
#define ATA_IDENT_MAX_LBA_EXT  200

//Selectors
#define IDE_ATA        0x00
#define IDE_ATAPI      0x01
 
#define ATA_MASTER     0x00
#define ATA_SLAVE      0x01

//Register offsets
#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

// Channels:
#define ATA_PRIMARY      0x00
#define ATA_SECONDARY    0x01
 
// Directions:
#define ATA_READ      0x00
#define ATA_WRITE     0x01

typedef struct _device{
	uint8_t presence;	// 0 if device is not present, or 1 if it is connected
	uint8_t channel;	// ATA_PRIMARY or ATA_SECONDARY
	uint8_t mode;		// 0 - master, 1 - slave
	uint16_t type;		// IDE_ATA or IDE_ATAPI
	uint16_t signature;	// Signature of a drive
	uint16_t features; 	// Information about drive's features
	uint32_t commands;	// Supported command sets
	uint32_t size;		// Sector count
	uint8_t model[41];	// Model name (char array)
}ide_device;

typedef struct _regs{
	uint16_t base;		// I/O Base
	uint16_t control;	// Control Base
	uint16_t master;	// Bus Master IDE
	uint8_t ni; 		// No interrupt
} channel_regs;



//! Initialization function
/*!
	Chcecks for IDE controller presence and configures it's structure
	@return true if initialization has passed or false if no controller was found or other error occured
*/
bool ide_pci_init();

//Following functions from osdev. To be adjusted
void ide_write(channel_regs* channel, uint8_t reg, uint8_t data);
uint8_t ide_read(channel_regs* channel, uint8_t reg);

#endif