#include <avr/io.h>
#include "ltc6903.h"
#include "spi.h"

uint8_t clock_one_mode;
uint8_t clock_two_mode;

void init_clocks()
{
	init_spi();

	CLOCK_ONE_DDR |= ((1<<CLOCK_ONE_OE) | (1<<CLOCK_ONE_CS));
	CLOCK_TWO_DDR |= ((1<<CLOCK_TWO_OE)|(1<<CLOCK_TWO_CS));

	clock_one_mode = NORMAL;
	clock_two_mode = NORMAL;

	//start both off and both deselected
	CLOCK_ONE_PORT &= 1<<CLOCK_ONE_OE;
	CLOCK_ONE_PORT |= 1<<CLOCK_ONE_CS;

	CLOCK_TWO_PORT &= 1<<CLOCK_TWO_OE;
	CLOCK_TWO_PORT |= 1<<CLOCK_TWO_CS;
}

void select_clock(int clock)
{
	switch(clock)
	{
		case CLOCK_ONE:
			CLOCK_ONE_PORT &= ~(1<<CLOCK_ONE_CS);
		break;
		case CLOCK_TWO:
			CLOCK_TWO_PORT &= ~(1<<CLOCK_TWO_CS);
		break;	
	}
	
}

void deselect_clock(int clock)
{
	switch(clock)
	{
		case CLOCK_ONE:
			CLOCK_ONE_PORT |= (1<<CLOCK_ONE_CS);
		break;
		case CLOCK_TWO:
			CLOCK_TWO_PORT |= (1<<CLOCK_TWO_CS);
		break;	
	}
}

void clock_on(int clock)
{
	switch(clock)
	{
		case CLOCK_ONE:
			CLOCK_ONE_PORT |= (1<<CLOCK_ONE_OE);
		break;
		case CLOCK_TWO:
			CLOCK_TWO_PORT |= (1<<CLOCK_TWO_OE);
		break;	
	}
}

void clock_off(int clock)
{
	switch(clock)
	{
		case CLOCK_ONE:
			CLOCK_ONE_PORT &= ~(1<<CLOCK_ONE_OE);
		break;
		case CLOCK_TWO:
			CLOCK_TWO_PORT &= ~(1<<CLOCK_TWO_OE);
		break;	
	}
}

void set_clock(int clock, uint8_t oct, uint16_t dac)
{
	uint8_t hi = ((oct<<4) | ((dac<<2) &0x0F));
	uint8_t low = 0x00;
	switch(clock)
	{
		case CLOCK_ONE:
			low = (((dac<<2) & 0x0FC) | clock_one_mode);
		break;
		case CLOCK_TWO:
			low = (((dac<<2) & 0x0FC) | clock_two_mode);
		break;	
	}

	write_clock(clock,hi,low);
}

void write_clock(int clock,uint8_t hi, uint8_t low)
{
	select_clock(clock);
	spi_sendbyte(hi);
	spi_sendbyte(low);
	deselect_clock(clock);
}

void set_clock_mode(int clock,uint8_t mode)
{
	switch(clock)
	{
		case CLOCK_ONE:
			clock_one_mode = mode & 0x03;
		break;
		case CLOCK_TWO:
			clock_two_mode = mode & 0x03;
		break;	
	}
}
