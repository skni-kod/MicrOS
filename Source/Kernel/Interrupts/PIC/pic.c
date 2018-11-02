#include "pic.h"

void pic_init()
{
	// Move all interrupts from 0..7 to 32..39
	// First parameter means that IRQ0 data will have 0x20 (32) offset in IDT
	// Second parameter means that IRQ8 data will have 0x28 (40) offset in IDT
    pic_remap(0x20, 0x28);
}

void pic_remap(int master_offset, int slave_offset)
{
	// Send initialization sequence to the master PIC and slave PIC
	outb(MASTER_PIC_COMMAND, 0x11);
	io_wait();
	outb(SLAVE_PIC_COMMAND, 0x11);
	io_wait();

	// Set master PIC offset
	outb(MASTER_PIC_DATA, master_offset);
	io_wait();

	// Set slave PIC offset
	outb(SLAVE_PIC_DATA, slave_offset);
	io_wait();

	// Tell master PIC that there is a slave PIC on the IRQ2 (4 = 0100)
	outb(MASTER_PIC_DATA, 4); 
	io_wait();

	// Tell slave PIC about his role
	outb(SLAVE_PIC_DATA, 2); 
	io_wait();
 
	// We are in 8086 mode
	outb(MASTER_PIC_DATA, 0x01);
	io_wait();
	outb(SLAVE_PIC_DATA, 0x01);
	io_wait();
 
	// Reset masks to the default values
	outb(MASTER_PIC_DATA, 0xFF);
	outb(SLAVE_PIC_DATA, 0xFF);

	// Enable interrupts
	enable();
}

void pic_enable_irq(uint8_t interrupt_number)
{
    uint16_t current_mask = (inb(SLAVE_PIC_DATA) << 8) | inb(MASTER_PIC_DATA);
	current_mask &= ~(1 << interrupt_number);

	uint8_t test1 = (uint8_t)current_mask & 0xFFFF;
	uint8_t test2 = (current_mask >> 8) & 0xFFFF;


	outb(MASTER_PIC_DATA, (uint8_t)current_mask & 0xFFFF);
	outb(SLAVE_PIC_DATA, (current_mask >> 8) & 0xFFFF);
}

void pic_disable_irq(uint8_t interrupt_number)
{
	uint16_t current_mask = (inb(SLAVE_PIC_DATA) << 8) | inb(MASTER_PIC_DATA);
	current_mask |= 1 << interrupt_number;

	outb(MASTER_PIC_DATA, (uint8_t)current_mask);
	outb(SLAVE_PIC_DATA, current_mask >> 8);
}