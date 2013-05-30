#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi.h"

void init_spi()
{
	DDRB = 0xFF;
	PORTB = 0x00;
	char clr=SPSR; //clean out the SPI data registser gotta set it to avoid optimising out;
	clr = SPDR;

	//SPI is ON, MASTER, CLOCK RESET HIGH, rising edge sample
	SPCR = 0x00;
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA); 
	SPCR |= (1<<SPR1)|(1<<SPR0); //4mhz
}

char spi_sendbyte(uint8_t cdata)
{
	SPDR = cdata;
	// Wait for transmission complete
	cli();
	while(!(SPSR & (1<<SPIF)));
	sei();

	return SPDR;
}

