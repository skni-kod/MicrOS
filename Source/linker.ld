OUTPUT_FORMAT(elf32-i386)

KERNEL_PHYSICAL_ADDRESS = 0x00010000;
KERNEL_VIRTUAL_ADDRESS  = 0xC0000000;

SECTIONS
{
	. = KERNEL_PHYSICAL_ADDRESS;
 
	.boot :
	{
		Build\Kernel\startup.o
	}

	. += KERNEL_VIRTUAL_ADDRESS;

	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VIRTUAL_ADDRESS)
	{
		*(.text)
	}
 
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VIRTUAL_ADDRESS)
	{
		*(.rodata)
	}
 
	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VIRTUAL_ADDRESS)
	{
		*(.data)
	}
 
	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VIRTUAL_ADDRESS)
	{
		*(.bss)
	}
}