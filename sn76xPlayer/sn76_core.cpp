#include "sn76_core.h"

sn76core::sn76core(serial_port* portref):
port(portref)
{
}

void sn76core::set_clock(int chip, uint8_t oct, uint8_t dac_high, uint8_t dac_low)
{
	char buff[] = {0x05,SET_CLOCK,chip,oct,dac_high,dac_low};
	port->write(buff,sizeof(buff));
}

void sn76core::set_freq(int chip, int channel, uint8_t freq_high, uint8_t freq_low)
{
	char buff[] = {0x05,SET_FREQ,chip,channel,freq_high,freq_low};
	port->write(buff,sizeof(buff));
}

void sn76core::set_amp(int chip, int channel,uint8_t amp)
{
	char buff[] = {0x04,SET_AMP,chip,channel,amp};
	port->write(buff,sizeof(buff));
}

void sn76core::set_noise(int chip, uint8_t setting)
{
	char buff[] = {0x03,SET_NOISE,chip,setting};
	port->write(buff,sizeof(buff));
}