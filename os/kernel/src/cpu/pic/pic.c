#include "pic.h"
#include "../../drivers/vga/vga.h"

void pic_wait_100_us()
{
	unsigned ticks = 0;

	unsigned int last_pit;

	io_out_byte(0x43, 0x0);
 
	last_pit = io_in_byte(0x40);		// Low byte
	last_pit |= io_in_byte(0x40)<<8;		// High byte

	while(ticks < 1200)
	{
		unsigned int current_pit;
		io_out_byte(0x43, 0x0);
		current_pit = io_in_byte(0x40);		// Low byte
		current_pit |= io_in_byte(0x40)<<8;		// High byte
		if(current_pit != last_pit)
		{
			last_pit = current_pit;
			ticks++;
		}
	}
}

void pic_init()
{
	// Move all interrupts from 0..7 to 32..39
	// First parameter means that IRQ0 data will have 0x20 (32) offset in IDT
	// Second parameter means that IRQ8 data will have 0x28 (40) offset in IDT
	pic_remap(0x20, 0x28);
}

void pic_remap(uint32_t master_offset, uint32_t slave_offset)
{
	// Send initialization sequence to the master PIC and slave PIC
	io_out_byte(MASTER_PIC_COMMAND, 0x11);
	pic_wait_100_us();

	io_out_byte(SLAVE_PIC_COMMAND, 0x11);
	pic_wait_100_us();

	// Set master PIC offset
	io_out_byte(MASTER_PIC_DATA, master_offset);
	pic_wait_100_us();

	// Set slave PIC offset
	io_out_byte(SLAVE_PIC_DATA, slave_offset);
	pic_wait_100_us();

	// Tell master PIC that there is a slave PIC on the IRQ2 (4 = 0100)
	io_out_byte(MASTER_PIC_DATA, 4);
	pic_wait_100_us();
	// Tell slave PIC about his role
	io_out_byte(SLAVE_PIC_DATA, 2);
	pic_wait_100_us();

	// We are in 8086 mode
	//TODO: Consider if should be like this:
	//io_out_byte(MASTER_PIC_DATA, 0x05);
	io_out_byte(MASTER_PIC_DATA, 0x01);
	pic_wait_100_us();
	io_out_byte(SLAVE_PIC_DATA, 0x01);
	pic_wait_100_us();

	vga_printstring("PIC WAIT START\n");
	pic_wait_100_us();
	vga_printstring("PIC WAIT STOP\n");

	// Reset masks to the default values
	io_out_byte(MASTER_PIC_DATA, 0x0);
	io_out_byte(SLAVE_PIC_DATA, 0x0);
	// Enable interrupts
	io_enable_interrupts();
	while(1);
}

void pic_enable_irq(uint8_t interrupt_number)
{
	uint16_t current_mask = io_in_byte(SLAVE_PIC_DATA) << 8 |
							io_in_byte(MASTER_PIC_DATA);
	current_mask &= ~(1 << interrupt_number);

	io_out_byte(MASTER_PIC_DATA, (uint8_t)current_mask);
	io_out_byte(SLAVE_PIC_DATA, (uint8_t)(current_mask >> 8));
}

void pic_disable_irq(uint8_t interrupt_number)
{
	uint16_t current_mask = io_in_byte(SLAVE_PIC_DATA) << 8 |
							io_in_byte(MASTER_PIC_DATA);
	current_mask |= 1 << interrupt_number;

	io_out_byte(MASTER_PIC_DATA, (uint8_t)current_mask);
	io_out_byte(SLAVE_PIC_DATA, (uint8_t)(current_mask >> 8));
}

void pic_confirm_master()
{
	io_out_byte(MASTER_PIC_COMMAND, 0x20);
}

void pic_confirm_master_and_slave()
{
	io_out_byte(MASTER_PIC_COMMAND, 0x20);
	io_out_byte(SLAVE_PIC_COMMAND, 0x20);
}