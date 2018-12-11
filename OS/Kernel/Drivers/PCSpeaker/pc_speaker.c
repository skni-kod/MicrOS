#include "pc_speaker.h"
#include "../../Assembly/io.h"

uint16_t old_value = 0;

void pc_speaker_sound(uint32_t freq)
{
	uint32_t Div;
	uint8_t tmp;

	Div = 1193180 / freq;
	outb(0x43, 0xb6);
	old_value = inb(0x42);
	old_value |= ((uint16_t)inb(0x42)) << 8;
	outb(0x42, (uint8_t)(Div));
	outb(0x42, (uint8_t)(Div >> 8));

	tmp = inb(0x61);
	if (tmp != (tmp | 3))
	{
		outb(0x61, tmp | 3);
	}
}

void pc_speaker_no_sound()
{
	uint8_t tmp = inb(0x61) & 0xFC;
	outb(0x61, tmp);
	outb(0x43, 0xb6);
	outb(0x42, (uint8_t)(old_value));
	outb(0x42, (uint8_t)(old_value >> 8));
}