OUTPUT_FORMAT(binary)

SECTIONS
{
	. = 0x7C00;

	.boot :
	{
		/mnt/ramdisk/build/obj/bootloader/pxe/boot.o
	}

	. = 0x8F00;
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
}
