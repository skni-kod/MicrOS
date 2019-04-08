#include "pc_speaker.h"
#include "assembly/io.h"

uint16_t old_value = 0;

void pc_speaker_enable_sound(uint32_t freq)
{
	uint32_t div;
	uint8_t tmp;

	div = 1193180 / freq;
	io_out_byte(0x43, 0xb6);
	old_value = io_in_byte(0x42);
	old_value |= ((uint16_t)io_in_byte(0x42)) << 8;
	io_out_byte(0x42, (uint8_t)(div));
	io_out_byte(0x42, (uint8_t)(div >> 8));

	tmp = io_in_byte(0x61);
	if (tmp != (tmp | 3))
	{
		io_out_byte(0x61, tmp | 3);
	}
}

void pc_speaker_disable_sound()
{
	uint8_t tmp = io_in_byte(0x61) & 0xFC;
	io_out_byte(0x61, tmp);
	io_out_byte(0x43, 0xb6);
	io_out_byte(0x42, (uint8_t)(old_value));
	io_out_byte(0x42, (uint8_t)(old_value >> 8));
}