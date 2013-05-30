#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "usb_serial.h"
#include "ltc6903.h"
#include "segachip.h"
#include "aoscls_parser.h"


int main()
{
	usb_init();
	init_clocks();
	init_segachips();

	_delay_ms(20);
	//set the clock to 4mhz

	set_clock(CLOCK_ONE,0x0A, 0x3bf);
	clock_on(CLOCK_ONE);
	set_clock(CLOCK_TWO,0x0A, 0x3bf);
	clock_on(CLOCK_TWO);

	_delay_ms(20);

	//all channels at 0 volume
	
	select_chip(SEGA_CHIP_TWO);
	write_chip(SEGA_CHIP_TWO, 0xFF);
	write_chip(SEGA_CHIP_TWO, 0x9F);
	write_chip(SEGA_CHIP_TWO, 0xBF);
	write_chip(SEGA_CHIP_TWO, 0xDF);
	deselect_chip(SEGA_CHIP_TWO);

	//all channels at 0 volume
	select_chip(SEGA_CHIP_ONE);
	write_chip(SEGA_CHIP_ONE, 0xFF);
	write_chip(SEGA_CHIP_ONE, 0x9F);
	write_chip(SEGA_CHIP_ONE, 0xBF);
	write_chip(SEGA_CHIP_ONE, 0xDF);
	deselect_chip(SEGA_CHIP_ONE);
	
	uint8_t bytes = 0;

	mainloop:
	{
		bytes = usb_serial_available();

		for(int i=0; i<bytes; ++i)
		{
			aoscls_parser_cin(usb_serial_getchar());
		}
	}
	goto mainloop;
}
