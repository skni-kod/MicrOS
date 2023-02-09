STARTUP_ADDRESS = 0x7C00;
STACK_ADDRESS = 0x7BFF;

SECTIONS
{
	
	. = STARTUP_ADDRESS;
	.boot :
	{
		*(.boot)
	}

	.text ALIGN(4K) : AT(ADDR(.text))
	{
		*(.text)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata))
	{
		*(.rodata)
	}

	.data ALIGN(4K) : AT(ADDR(.data))
	{
		*(.data)
	}

	BSS_SECTION_START = .;

	.bss ALIGN(4K) : AT(ADDR(.bss))
	{
		*(.bss)
	}

	BSS_SECTION_END = .;
	LOADER_END = .;
}
