#ifndef LOADER_H
#define LOADER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PXE_OPCODE_GET_CACHED_INFO ((uint16_t)0x71u)
#define PXENV_PACKET_TYPE_DHCP_ACK ((uint16_t)0x2u)
#define PXE_OPCODE_TFTP_GET_FILE_SIZE ((uint16_t)0x25u)

#define HTONS(n) ((((n)&0xFF) << 8) | (((n)&0xFF00) >> 8))

#define KERNEL_FILE "/output/KERNEL.BIN"

typedef struct cpu_regs
{
	// eax
	union
	{
		uint32_t eax;
		uint32_t ax : 16, : 16;
		uint32_t al : 8, ah : 8, : 16;
	};
	// ebx
	union
	{
		uint32_t ebx;
		uint32_t bx : 16, : 16;
		uint32_t bl : 8, bh : 8, : 16;
	};
	// ecx
	union
	{
		uint32_t ecx;
		uint32_t cx : 16, : 16;
		uint32_t cl : 8, ch : 8, : 16;
	};
	// edx
	union
	{
		uint32_t edx;
		uint32_t dx : 16, : 16;
		uint32_t dl : 8, dh : 8, : 16;
	};
	// esp
	union
	{
		uint32_t esp;
		uint32_t sp : 16, : 16;
	};
	// ebp
	union
	{
		uint32_t ebp;
		uint32_t bp : 16, : 16;
	};
	// esi
	union
	{
		uint32_t esi;
		uint32_t si : 16, : 16;
	};
	// edi
	union
	{
		uint32_t edi;
		uint32_t di : 16, : 16;
	};
	uint32_t efl;
	uint16_t es;
	uint16_t ds;
	uint16_t fs;
	uint16_t gs;
	uint16_t ss;
} cpu_regs_t;

typedef struct seg_desc
{
	uint16_t segment_address;
	uint16_t Physical_address;
	uint16_t Seg_Size;
} seg_desc_t;

typedef struct seg_off
{
	uint16_t offset;
	uint16_t segment;
} seg_off_t;

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
	uint16_t Status;
} __attribute__((packed)) tftp_close_t;

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

#endif