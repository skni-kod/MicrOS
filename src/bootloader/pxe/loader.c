#include "loader.h"

extern uint16_t pxe_addr_seg;
extern uint16_t pxe_addr_off;

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

void *memset(void *buffer, int value, size_t size)
{
	uint8_t *ptr = (uint8_t *)buffer;
	for (size_t i = 0; i < size; i++)
	{
		*ptr = value;
		ptr++;
	}

	return buffer;
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
	char *rv = ptr;
	while (ptr1 < ptr)
	{
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return rv;
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

int memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *c1 = s1, *c2 = s2;
	int d = 0;

	while (n--)
	{
		d = (int)*c1++ - (int)*c2++;
		if (d)
			break;
	}

	return d;
}

static int is_pxe(const void *buf)
{
	const pxe_t *pxe = buf;
	const uint8_t *p = buf;
	int i = pxe->length;
	uint8_t sum = 0;

	if (i < sizeof(pxe_t) ||
		memcmp(pxe->signature, "!PXE", 4))
		return 0;

	while (i--)
		sum += *p++;

	return sum == 0;
}

/*
 * Just like is_pxe, it checks PXENV+ structure
 *
 */
static int is_pxenv(const void *buf)
{
	const pxenv_t *pxenv = buf;
	const uint8_t *p = buf;
	int i = pxenv->length;
	uint8_t sum = 0;

	/* The pxeptr field isn't present in old versions */
	if (i < offsetof(pxenv_t, pxe_ptr) ||
		memcmp(pxenv->signature, "PXENV+", 6))
		return 0;

	while (i--)
		sum += *p++;

	return sum == 0;
}

static const void *memory_scan(uintptr_t start, int (*func)(const void *))
{
	const char *ptr;

	/* Scan each 16 bytes of conventional memory before the VGA region */
	for (ptr = (const char *)start; ptr < (const char *)HIGH_BASE_MEM; ptr += 16)
	{
		if (func(ptr))
			return ptr; /* found it! */
		ptr += 16;
	}
	return NULL;
}

static inline void *get_ptr(const seg_off_t ptr)
{
	return (void *)((ptr.segment << 4) + ptr.offset);
}

static const struct pxe_t *memory_scan_for_pxe_struct(void)
{
	return memory_scan((int)(&LOADER_END), is_pxe);
}

static const struct pxenv_t *memory_scan_for_pxenv_struct(void)
{
	return memory_scan(0x10000, is_pxenv);
}

void dhcp_read_option(dhcp_message_t *msg, uint8_t code, uint8_t *buf, uint16_t len)
{
	uint32_t i = 0;
	while (1)
	{
		uint8_t type = msg->options[i];

		if (0xFF == type)
			return;

		if (type == code)
		{
			memcpy(buf, (*msg).options + i + 2, len);
			return;
		}
		int skip = msg->options[i + 1] + 2;
		i += skip;
	}
}

int get_ip_config(seg_off_t entry, ipv4_addr_t *server, ipv4_addr_t *gateway)
{
	get_cached_info_t info = (get_cached_info_t){.packet_type = PXENV_PACKET_TYPE_DHCP_ACK};
	char tmp[64] = "";

	server->value = gateway->value = 0;

	pxecall(entry.segment,
			entry.offset,
			PXE_OPCODE_GET_CACHED_INFO,
			0,
			&info);

	dhcp_message_t *dhcp_ack = (info.buffer_seg << 4) + info.buffer_off;
	ipv4_addr_t *addr = &dhcp_ack->siaddr;

	if (dhcp_ack->siaddr.value)
		*server = dhcp_ack->siaddr;

	if (dhcp_ack->giaddr.value)
		*gateway = dhcp_ack->giaddr;

	memcpy(tmp, "Your IP: ", 10);
	print_ip(&dhcp_ack->yiaddr, tmp + 9);
	terminal_writestring(tmp);
	memset(tmp, 0, 64);

	dhcp_read_option(dhcp_ack, 3, gateway, sizeof(ipv4_addr_t));

	memcpy(tmp, "Gateway: ", 10);
	print_ip(gateway, tmp + 9);
	terminal_writestring(tmp);
	memset(tmp, 0, 64);

	info.packet_type = 3;
	info.status = 0;
	pxecall(entry.segment,
			entry.offset,
			PXE_OPCODE_GET_CACHED_INFO,
			0,
			&info);

	dhcp_ack = (info.buffer_seg << 4) + info.buffer_off;

	dhcp_read_option(dhcp_ack, 54, server, sizeof(ipv4_addr_t));
	memcpy(tmp, "Server: ", 9);
	print_ip(server, tmp + 8);
	terminal_writestring(tmp);
	memset(tmp, 0, 64);

	if (server->value != 0)
		return 0;

	return -1;
}

void loader_main(seg_off_t in_addr)
{
	seg_off_t pxe_entrypoint;
	pxenv_t *pxenv;
	pxe_t *pxe;

	ipv4_addr_t server;
	ipv4_addr_t gateway;

	terminal_initialize();

	pxenv = get_ptr(in_addr);
	if (is_pxenv(pxenv))
		goto have_pxenv;

	if ((pxe = memory_scan_for_pxe_struct()))
		goto have_pxe;

	if ((pxenv = memory_scan_for_pxenv_struct()))
		goto have_pxenv;

	goto dupa;

have_pxenv:
	/* if the API version number is 0x0201 or higher, use the !PXE structure */
	if (pxenv->version >= 0x201)
	{
		if (pxenv->length >= sizeof(pxenv_t))
		{
			pxe = get_ptr(pxenv->pxe_ptr);
			if (is_pxe(pxe))
				goto have_pxe;
			/*
			 * Nope, !PXE structure missing despite API 2.1+, or at least
			 * the pointer is missing. Do a last-ditch attempt to find it
			 */
			if ((pxe = memory_scan_for_pxe_struct()))
				goto have_pxe;
		}
	}
	pxe_entrypoint = pxenv->rm_entry;
	terminal_writestring("Loading via PXENV+\n");
	goto load;
have_pxe:
	pxe_entrypoint = pxe->EntryPointSP;
	terminal_writestring("Loading via !PXE\n");
load:
	terminal_writestring("Hello!\n");

	uint16_t rv = (uint16_t)get_ip_config(pxe_entrypoint, &server, &gateway);

	if (rv)
		goto dupa;

	// get file size
	{
		get_file_size_t file = (get_file_size_t){
			.status = 0,
			.server_addr = server,
			.gateway_addr = gateway,
			.file_size = 0,
			.file = KERNEL_FILENAME};

		rv = pxecall(pxe_entrypoint.segment,
					 pxe_entrypoint.offset,
					 PXE_OPCODE_TFTP_GET_FILE_SIZE,
					 0,
					 &file);
	}

	if (rv)
		goto file_load_fail;

	// open connection
	{
		tftp_open_t open = (tftp_open_t){
			.status = 0,
			.server_addr = server,
			.gateway_addr = gateway,
			.packet_size = BUFFER_SIZE,
			.tftp_port = HTONS(69),
			.file = KERNEL_FILENAME};

		rv = pxecall(pxe_entrypoint.segment,
					 pxe_entrypoint.offset,
					 (uint16_t)0x20,
					 0,
					 &open);
	}

	if (rv)
		goto file_load_fail;

	// read file
	{
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
						 (uint16_t)0x22,
						 0,
						 &read);

			if (rv)
				goto file_load_fail;

			memcpy(kernel_addr + offset, &buf, BUFFER_SIZE);

			if (read.bytes_read < BUFFER_SIZE)
				break;
			offset += BUFFER_SIZE;
		}
	}

	// close file
	{
		tftp_close_t close = (tftp_close_t){.status = 0};

		rv = pxecall(pxe_entrypoint.segment,
					 pxe_entrypoint.offset,
					 (uint16_t)0x21,
					 0,
					 &close);
	}

	// Unload PXE
	{
		PXENV_UNDI_SHUTDOWN_t shut = (PXENV_UNDI_SHUTDOWN_t){.status = 0};

		rv = pxecall(pxe_entrypoint.segment,
					 pxe_entrypoint.offset,
					 (uint16_t)0x5,
					 0,
					 &shut);
	}

	if (rv)
		goto dupa;

	{
		PXENV_UNLOAD_STACK_t shut = (PXENV_UNLOAD_STACK_t){.status = 0};

		rv = pxecall(pxe_entrypoint.segment,
					 pxe_entrypoint.offset,
					 (uint16_t)0x70,
					 0,
					 &shut);
	}

	if (rv)
		goto dupa;

	{
		PXENV_STOP_UNDI_t shut = (PXENV_STOP_UNDI_t){.status = 0};

		rv = pxecall(pxe_entrypoint.segment,
					 pxe_entrypoint.offset,
					 (uint16_t)0x02,
					 0,
					 &shut);
	}

	if (rv)
		goto dupa;

	// jump into kernel!
	terminal_writestring("Jumping into kernel...");
	enter_kernel();

file_load_fail:
	terminal_writestring("Unable to get file:");
	terminal_writestring(KERNEL_FILENAME);
	terminal_writestring(" using TFTP\n");

dupa:
	terminal_writestring("No i dupa\n");
	while (1)
		;
}