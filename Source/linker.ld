STARTUP(Build\Kernel\startup.o)
OUTPUT_FORMAT(binary)

SECTIONS
{
	. = 0x10000;
 
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}
 
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}
 
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}
 
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(.bss)
	}
}