#include "loader.h"

static const void *kernel_addr = KERNEL_ADDRESS;
static const void (*kernel_entry)() = KERNEL_ADDRESS;

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t)uc | (uint16_t)color << 8;
}

size_t strlen(const char *str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t *)0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
	if (c == '\n')
	{
		++terminal_row;
		if (terminal_row == VGA_HEIGHT)
			terminal_row = 0;
		terminal_column = 0;
		return;
	}

	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH)
	{
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void terminal_write(const char *data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char *data)
{
	terminal_write(data, strlen(data));
}

char *itoa(int value, char *result, int base)
{
	// check that the base if valid
	if (base < 2 || base > 36)
	{
		*result = '\0';
		return result;
	}

	char *ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do
	{
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
	} while (value);

	// Apply negative sign
	if (tmp_value < 0)
		*ptr++ = '-';
	*ptr-- = '\0';
	while (ptr1 < ptr)
	{
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return ptr1;
}

void print_ip(ipv4_addr_t *addr, char *buf)
{
	char *ptr = itoa(addr->oct_a, buf, 10);
	*(++ptr) = '.';
	ptr = itoa(addr->oct_b, ++ptr, 10);
	*(++ptr) = '.';
	ptr = itoa(addr->oct_c, ++ptr, 10);
	*(++ptr) = '.';
	ptr = itoa(addr->oct_d, ++ptr, 10);
	*(++ptr) = '\n';
	*(++ptr) = '\0';
}

void *memcpy(void *destination, const void *source, size_t size)
{
	uint64_t *destination_64ptr = (uint64_t *)destination;
	uint64_t *source_64ptr = (uint64_t *)source;
	while (size >= 8)
	{
		*destination_64ptr++ = *source_64ptr++;
		size -= 8;
	}

	uint32_t *destination_32ptr = (uint32_t *)destination_64ptr;
	uint32_t *source_32ptr = (uint32_t *)source_64ptr;
	while (size >= 4)
	{
		*destination_32ptr++ = *source_32ptr++;
		size -= 4;
	}

	uint8_t *destination_ptr = (uint8_t *)destination_32ptr;
	uint8_t *source_ptr = (uint8_t *)source_32ptr;
	while (size >= 1)
	{
		*destination_ptr++ = *source_ptr++;
		size--;
	}

	return destination;
}

uint8_t chksum8(const unsigned char *buff, size_t len)
{
	unsigned int sum; // nothing gained in using smaller types!
	for (sum = 0; len != 0; len--)
		sum += *(buff++); // parenthesis not required!
	return (uint8_t)sum;
}

/*
	Scan for PXENV+ structure
*/
uint8_t *scan_old_pxe(uint8_t *mem_begin, uint8_t *mem_end)
{
	for (uint8_t *i = mem_begin; i < mem_end; i++)
	{
		if (*i != 'P')
			continue;
		if (*(i + 1) != 'X')
			continue;
		if (*(i + 2) != 'E')
			continue;
		if (*(i + 3) != 'N')
			continue;
		if (*(i + 4) != 'V')
			continue;
		if (*(i + 5) != '+')
			continue;
		return i;
	}
	return 0;
}

/*
	Scan for !PXE structure
*/
uint8_t *scan_new_pxe(uint8_t *mem_begin, uint8_t *mem_end)
{
	for (uint8_t *i = mem_begin; i < mem_end; i++)
	{
		if (*i != '!')
			continue;
		if (*(i + 1) != 'P')
			continue;
		if (*(i + 2) != 'X')
			continue;
		if (*(i + 3) != 'E')
			continue;
		return i;
	}
	return 0;
}

extern char LOADER_END;

void loader_main(seg_off_t pxe)
{
	terminal_initialize();
	old_pxe_t *old_pxe_struct = (pxe.segment << 4) + pxe.offset;
	pxe_t *pxe_struct = scan_new_pxe(&LOADER_END,0xA0000);

	seg_off_t pxe_entrypoint;

	if (old_pxe_struct->version < 0x0201)
	{
		// use old
		terminal_writestring("Booting using PXENV+\n");
		pxe_entrypoint = old_pxe_struct->rm_entry;
	}
	else
	{
		terminal_writestring("Booting using !PXE\n");
		pxe_entrypoint.segment = pxe_struct->EntryPointSP.segment;
		pxe_entrypoint.offset = pxe_struct->EntryPointSP.offset;

		if (pxe_struct->length != 0x58 || chksum8(pxe_struct, 0x58) != 0)
		{
			terminal_writestring("This is bad!\n");
			while (1)
				;
		}
	}

	get_cached_info_t info = (get_cached_info_t){.packet_type = PXENV_PACKET_TYPE_DHCP_ACK};

	// get tftp server addr
	uint16_t rv;

	rv = pxecall(pxe_entrypoint.segment,
				 pxe_entrypoint.offset,
				 PXE_OPCODE_GET_CACHED_INFO,
				 0,
				 &info);

	dhcp_message_t *dhcp_ack = (info.buffer_seg << 4) + info.buffer_off;
	ipv4_addr_t *addr = &dhcp_ack->siaddr;

	// get file size
	get_file_size_t file = (get_file_size_t){
		.status = 0,
		.server_addr = *addr,
		.gateway_addr = 0,
		.file_size = 0,
		.file = KERNEL_FILENAME};

	rv = pxecall(pxe_entrypoint.segment,
				 pxe_entrypoint.offset,
				 PXE_OPCODE_TFTP_GET_FILE_SIZE,
				 0,
				 &file);

	// open connection
	tftp_open_t open = (tftp_open_t){
		.status = 0,
		.server_addr = *addr,
		.gateway_addr = 0,
		.packet_size = BUFFER_SIZE,
		.tftp_port = HTONS(69),
		.file = KERNEL_FILENAME};

	rv = pxecall(pxe_entrypoint.segment,
				 pxe_entrypoint.offset,
				 0x20,
				 0,
				 &open);

	// read file
	uint8_t buf[BUFFER_SIZE];

	tftp_read_t read = (tftp_read_t){
		.status = 0,
		.packet_number = 0,
		.bytes_read = 0,
		.buffer.offset = &buf,
		.buffer.segment = 0,
	};

	uint32_t offset = 0;
	while (1)
	{

		rv = pxecall(pxe_entrypoint.segment,
					 pxe_entrypoint.offset,
					 0x22,
					 0,
					 &read);

		memcpy(kernel_addr + offset, &buf, BUFFER_SIZE);

		if (read.status)
		{
			terminal_writestring("Please restart, unable to download kernel\n");
			while (1)
				;
		}

		if (read.bytes_read < BUFFER_SIZE)
			break;
		offset += BUFFER_SIZE;
	}

	// close file
	tftp_close_t close = (tftp_close_t){.status = 0};

	rv = pxecall(pxe_entrypoint.segment,
				 pxe_entrypoint.offset,
				 0x21,
				 0,
				 &close);

	// jump into kernel!
	enter_kernel();
}