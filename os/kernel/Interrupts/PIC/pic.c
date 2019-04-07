#include "pic.h"

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
	io_wait();
	io_out_byte(SLAVE_PIC_COMMAND, 0x11);
	io_wait();

	// Set master PIC offset
	io_out_byte(MASTER_PIC_DATA, master_offset);
	io_wait();

	// Set slave PIC offset
	io_out_byte(SLAVE_PIC_DATA, slave_offset);
	io_wait();

	// Tell master PIC that there is a slave PIC on the IRQ2 (4 = 0100)
	io_out_byte(MASTER_PIC_DATA, 4);
	io_wait();

	// Tell slave PIC about his role
	io_out_byte(SLAVE_PIC_DATA, 2);
	io_wait();

	// We are in 8086 mode
	io_out_byte(MASTER_PIC_DATA, 0x01);
	io_wait();
	io_out_byte(SLAVE_PIC_DATA, 0x01);
	io_wait();

	// Reset masks to the default values
	io_out_byte(MASTER_PIC_DATA, 0xFF);
	io_out_byte(SLAVE_PIC_DATA, 0xFF);

	// Enable interrupts
	io_enable_interrupts();
}

void pic_enable_irq(uint8_t interrupt_number)
{
	uint16_t current_mask = (io_in_byte(SLAVE_PIC_DATA) << 8) | io_in_byte(MASTER_PIC_DATA);
	current_mask &= ~(1 << interrupt_number);

	io_out_byte(MASTER_PIC_DATA, (uint8_t)current_mask & 0xFFFF);
	io_out_byte(SLAVE_PIC_DATA, (current_mask >> 8) & 0xFFFF);
}

void pic_disable_irq(uint8_t interrupt_number)
{
	uint16_t current_mask = (io_in_byte(SLAVE_PIC_DATA) << 8) | io_in_byte(MASTER_PIC_DATA);
	current_mask |= 1 << interrupt_number;

	io_out_byte(MASTER_PIC_DATA, (uint8_t)current_mask);
	io_out_byte(SLAVE_PIC_DATA, current_mask >> 8);
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