#ifndef LOADER_H
#define LOADER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PXE_OPCODE_GET_CACHED_INFO (0x71u)
#define PXENV_PACKET_TYPE_DHCP_ACK (0x2u)
#define PXE_OPCODE_TFTP_GET_FILE_SIZE (0x25u)

#define HTONS(n) ((((n)&0xFF) << 8) | (((n)&0xFF00) >> 8))


#define BASE_MEMORY_HIGH 0xA0000
#define BASE_MEMORY_LOW 0x10000

#define KERNEL_FILENAME "/output/KERNEL.BIN"
#define KERNEL_ADDRESS 0x100000

#define BUFFER_SIZE 1024


enum vga_color
{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

typedef union ipv4_addr
{
	struct
	{
		uint8_t oct_a;
		uint8_t oct_b;
		uint8_t oct_c;
		uint8_t oct_d;
	};
	uint32_t value;
} ipv4_addr_t;

typedef struct dhcp_message
{
	uint8_t opcode;
	uint8_t htype;
	uint8_t hlen;
	uint8_t hops;
	uint32_t xid;
	uint16_t secs;
	uint16_t : 7, broadcast : 1, : 8;
	ipv4_addr_t ciaddr;
	ipv4_addr_t yiaddr;
	ipv4_addr_t siaddr;
	ipv4_addr_t giaddr;
	uint32_t chaddr[4];
	uint8_t sname[64];
	uint8_t file[128];
	uint32_t magic_cookie;
	uint8_t options[];
} __attribute__((packed)) dhcp_message_t;

typedef struct seg_desc
{
	uint16_t segment_address;
	uint16_t physical_address;
	uint16_t segment_Size;
} seg_desc_t;

typedef struct seg_off
{
	uint16_t offset;
	uint16_t segment;
} seg_off_t;

typedef struct old_pxe
{
	uint8_t signature[6];
	uint16_t version;
	uint8_t length;
	uint8_t checksum;
	uint8_t;
	seg_off_t rm_entry;
	uint32_t pm_offset;
	uint16_t pm_selector;
	uint16_t stack_seg;
	uint16_t stack_size;
	uint16_t bc_codeseg;
	uint16_t bc_codesize;
	uint16_t bc_dataseg;
	uint16_t bc_datasize;
	uint16_t undi_data_seg;
	uint16_t undi_data_size;
	uint16_t undi_code_seg;
	uint16_t undi_code_size;
	seg_off_t pxe_ptr;
} old_pxe_t;

typedef struct pxe
{
	uint8_t signature[4];
	uint8_t length;
	uint8_t checksum;
	uint8_t revision;
	uint8_t;
	seg_off_t UNDIROMID;
	seg_off_t BaseROMID;
	seg_off_t EntryPointSP;
	seg_off_t EntryPointESP;
	seg_off_t StatusCallout;
	uint8_t;
	uint8_t SegDescCnt;
	uint16_t FirstSelector;
	seg_desc_t stack;
	seg_desc_t UNDIData;
	seg_desc_t UNDICode;
	seg_desc_t UNDICodeWrite;
	seg_desc_t BC_Data;
	seg_desc_t BC_Code;
	seg_desc_t BC_CodeWrite;
} pxe_t;

typedef struct GetCachedInfo
{
	uint16_t status;
	uint16_t packet_type;
	uint16_t buffer_size;
	uint16_t buffer_off;
	uint16_t buffer_seg;
	uint16_t buffer_limit;
} __attribute__((packed)) get_cached_info_t;

typedef struct get_file_size
{
	uint16_t status;
	ipv4_addr_t server_addr;
	ipv4_addr_t gateway_addr;
	uint8_t file[128];
	uint32_t file_size;
} __attribute__((packed)) get_file_size_t;

typedef struct tftp_open
{
	uint16_t status;
	ipv4_addr_t server_addr;
	ipv4_addr_t gateway_addr;
	uint8_t file[128];
	uint16_t tftp_port;
	uint16_t packet_size;
} __attribute__((packed)) tftp_open_t;

typedef struct tftp_read
{
	uint16_t status;
	uint16_t packet_number;
	uint16_t bytes_read;
	seg_off_t buffer;
} __attribute__((packed)) tftp_read_t;

typedef struct tftp_close
{
	uint16_t status;
} __attribute__((packed)) tftp_close_t;


extern uint16_t pxecall(uint16_t segment,
						uint16_t offset,
						uint16_t opcode,
						uint16_t param_segment,
						uint16_t param_offset);

extern void enter_kernel();

#endif