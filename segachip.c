#include <avr/io.h>
#include <util/delay.h>
#include "segachip.h"



void init_segachips()
{
	//all data ports out
	SEGA_DATA_DDR = 0xFF;
	SEGA_STATUS_DDR = 0xFF;
	SEGA_STATUS_DDR &= ~((1<<SEGA_TWO_READY)|(1<<SEGA_ONE_READY));
	
	//enable the pull up on the ready channels
	SEGA_STATUS_PORT |= (1<<SEGA_TWO_READY)|(1<<SEGA_ONE_READY);

	//init the chips, WE and OE all pulled up
	SEGA_STATUS_PORT |= (1<<SEGA_ONE_OUTPUT_ENABLE)|(1<<SEGA_ONE_WRITE_ENABLE);
	SEGA_STATUS_PORT |= (1<<SEGA_TWO_OUTPUT_ENABLE)|(1<<SEGA_TWO_WRITE_ENABLE);

}

//-------------------------------------------------------------------------------------------------
//											LOW LEVEL STUFF
//-------------------------------------------------------------------------------------------------
void select_chip(int chip)
{
	switch(chip)
	{
		case SEGA_CHIP_ONE:
			SEGA_STATUS_PORT &= ~(1<<SEGA_ONE_OUTPUT_ENABLE);
		break;
		case SEGA_CHIP_TWO:
			SEGA_STATUS_PORT &= ~(1<<SEGA_TWO_OUTPUT_ENABLE);
		break;
	}
}

void write_chip(int chip, uint8_t data)
{
	SEGA_DATA_PORT = data;

	switch(chip)
	{
		case SEGA_CHIP_ONE:
			SEGA_STATUS_PORT &= ~(1<<SEGA_ONE_WRITE_ENABLE);
			_delay_us(20);	
			//spin till it's written
			while(!(SEGA_STATUS_PORT & (1<<SEGA_ONE_READY)));
			SEGA_STATUS_PORT |= (1<<SEGA_ONE_WRITE_ENABLE);
		break;
		case SEGA_CHIP_TWO:
			SEGA_STATUS_PORT &= ~(1<<SEGA_TWO_WRITE_ENABLE);
			_delay_us(20);	
			//spin till it's written
			while(!(SEGA_STATUS_PORT & (1<<SEGA_TWO_READY)));
			SEGA_STATUS_PORT |= (1<<SEGA_TWO_WRITE_ENABLE);
		break;
	}
}

void deselect_chip(int chip)
{
	switch(chip)
	{
		case SEGA_CHIP_ONE:
			SEGA_STATUS_PORT |= (1<<SEGA_ONE_OUTPUT_ENABLE);
		break;
		case SEGA_CHIP_TWO:
			SEGA_STATUS_PORT |= (1<<SEGA_TWO_OUTPUT_ENABLE);
		break;
	}
}

//-------------------------------------------------------------------------------------------------
//											HIGH LEVEL STUFF
//-------------------------------------------------------------------------------------------------

void set_amp(uint8_t chip,uint8_t channel,uint8_t amp)
{
	uint8_t setting;
	switch(channel)
	{
		case 0:
			setting = 0x90 | (amp & 0x0F);
		break;
		case 1:
			setting = 0xB0 | (amp & 0x0F);
		break;
		case 2:
			setting = 0xD0 | (amp & 0x0F);
		break;
		case 3:
			setting = 0xF0 | (amp & 0x0F);
		break;
		default:
			return;
		break;
	}
	
	select_chip(chip);
	write_chip(chip,setting);
	deselect_chip(chip);
}

void set_noise(uint8_t chip,uint8_t noise)
{
	uint8_t setting = 0xE0 | (noise & 0x0F);

	select_chip(chip);
	write_chip(chip,setting);
	deselect_chip(chip);	
}

void set_freq(uint8_t chip,uint8_t channel, uint8_t hi, uint8_t low)
{
	uint8_t setting1;
	uint8_t setting2;

	switch(channel)
	{
		case 0:
			setting1 = 0x80 | (low & 0x0F);
			setting2 = hi & 0x3F;
		break;
		case 1:
			setting1 = 0xA0 | (low & 0x0F);
			setting2 = hi & 0x3F;
		break;
		case 2:
			setting1 = 0xC0 | (low & 0x0F);
			setting2 = hi & 0x3F;
		break;
		default:
			return;
		break;
	}

	select_chip(chip);
	write_chip(chip,setting1);
	write_chip(chip,setting2);
	deselect_chip(chip);
}
