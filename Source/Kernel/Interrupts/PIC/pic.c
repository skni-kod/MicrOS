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
	// Save current masks because they will be destroyed during reinit
	uint8_t master_mask = inb(MASTER_PIC_DATA);
	uint8_t slave_mask = inb(SLAVE_PIC_DATA);
 
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
 
	// Restore masks for master PIC and slave PIC
	outb(MASTER_PIC_DATA, master_mask);
	outb(SLAVE_PIC_DATA, slave_mask);
}