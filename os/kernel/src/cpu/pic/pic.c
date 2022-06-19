#include "pic.h"
#include "../../drivers/vga/vga.h"

// Count spurious interrupts
// TODO: Make interface to show user this number:
uint16_t spurious_count = 0;

void pic_init()
{
	// Move all interrupts from 0..7 to 32..39
	// First parameter means that IRQ0 data will have 0x20 (32) offset in IDT
	// Second parameter means that IRQ8 data will have 0x28 (40) offset in IDT
	pic_remap(MASTER_OFFSET, SLAVE_OFFSET);
}

void pic_remap(uint32_t master_offset, uint32_t slave_offset)
{
	// Read current masks
	uint8_t master_mask = io_in_byte(MASTER_PIC_DATA);
	uint8_t slave_mask = io_in_byte(SLAVE_PIC_DATA);

	// ICW1 MASTER with ICW4_NEEDED and CASCADE MODE
	pic_out_byte(MASTER_PIC_COMMAND, ICW1_FLAG_INIT | ICW1_FLAG_ICW4_NEEDED | ICW1_FLAG_MODE_CASCADE);

	// ICW2 MASTER - Interrupt Vector Address
	pic_out_byte(MASTER_PIC_DATA, master_offset);

	// ICW3 MASTER -
	pic_out_byte(MASTER_PIC_DATA, PIC_MASTER_CASCADE_LINE);

	// ICW4 MASTER
	pic_out_byte(MASTER_PIC_DATA, ICW3_FLAG_MODE_x86);

	// ICW1 SLAVE with ICW4_NEEDED and CASCADE MODE
	pic_out_byte(SLAVE_PIC_COMMAND, ICW1_FLAG_INIT | ICW1_FLAG_ICW4_NEEDED);

	// ICW2 SLAVE - Interrupt Vector Address
	pic_out_byte(SLAVE_PIC_DATA, slave_offset);

	// ICW3 SLAVE -
	pic_out_byte(SLAVE_PIC_DATA, PIC_SLAVE_CASCADE_LINE);

	// ICW4 SLAVE
	pic_out_byte(SLAVE_PIC_DATA, ICW3_FLAG_MODE_x86);

	// Restore masks
	pic_out_byte(MASTER_PIC_DATA, master_mask);
	pic_out_byte(SLAVE_PIC_DATA, slave_mask);
}

void pic_enable_irq(uint8_t interrupt_number)
{
	// If interrupt number correpsonds to master
	if (interrupt_number < 8)
	{
		uint8_t mask = io_in_byte(MASTER_PIC_DATA);
		mask &= ~(1 << interrupt_number);
		pic_out_byte(MASTER_PIC_DATA, mask);
		// exit function
		return;
	}

	// If interrupt number corresponds to slave
	// take care of unmasking slave in master
	if (interrupt_number > 7 && interrupt_number < 16)
	{
		uint8_t mask = io_in_byte(SLAVE_PIC_DATA);
		interrupt_number -= 8;
		mask &= ~(1 << interrupt_number);
		pic_out_byte(SLAVE_PIC_DATA, mask);

		// unmask slave in master
		mask = io_in_byte(MASTER_PIC_DATA);
		mask &= ~(1 << 2);
		pic_out_byte(MASTER_PIC_DATA, mask);
		return;
	}
}

void pic_disable_irq(uint8_t interrupt_number)
{
	// If interrupt number correpsonds to master
	if (interrupt_number < 8)
	{
		uint8_t mask = io_in_byte(MASTER_PIC_DATA);
		mask |= (1 << interrupt_number);
		pic_out_byte(MASTER_PIC_DATA, mask);
		// exit function
		return;
	}

	// If interrupt number corresponds to slave
	if (interrupt_number > 7 && interrupt_number < 16)
	{
		uint8_t mask = io_in_byte(SLAVE_PIC_DATA);
		interrupt_number -= 8;
		mask |= (1 << interrupt_number);
		pic_out_byte(SLAVE_PIC_DATA, mask);
		return;
	}
}

uint8_t pic_handle_irq(uint8_t interrupt_number)
{
	uint16_t pic_isr = PIC_GET_ISR;
	// check if irq is spurious
	// master
	if (interrupt_number == 7 && !(pic_isr && (1 << 7)))
	{
		// pic_get_isr returns 16 wide value, we need only 8th bit:
		// if the bit is set 1, the IRQ is true, but if not it's spurious
		spurious_count++;
		return 0;
	}

	// slave
	else if (interrupt_number == 15 && !(pic_isr && (1 << 15)))
	{
		// pic_get_isr returns 16 wide value, we need only 16th bit:
		// if the bit is set 1, the IRQ is true, but if not it's spurious
		// Send EOI ONLY to master
		pic_out_byte(MASTER_PIC_COMMAND, PIC_EOI);
		spurious_count++;
		return 0;
	}

	return 1;
}

void pic_send_eoi(uint8_t interrupt_number)
{
	if (interrupt_number >= 8)
		pic_out_byte(SLAVE_PIC_COMMAND, PIC_EOI);

	pic_out_byte(MASTER_PIC_COMMAND, PIC_EOI);
}

uint16_t __pic_get_reg(uint8_t OCW3)
{
	pic_out_byte(MASTER_PIC_COMMAND, OCW3);
	pic_out_byte(SLAVE_PIC_COMMAND, OCW3);
	return (io_in_byte(SLAVE_PIC_COMMAND) << 8) | io_in_byte(MASTER_PIC_COMMAND);
}

void pic_delay()
{
	for (uint8_t i = 0; i < 4; i++)
		io_out_byte(0x80, 0);
}

void pic_out_byte(uint16_t port, uint16_t value)
{
	// Write as usual
	io_out_byte(port, value);
	// Wait a bit
	pic_delay();
}