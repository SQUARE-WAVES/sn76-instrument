#ifndef SNSEVENSIX_CORE_DOT_H
#define SNSEVENSIX_CORE_DOT_H

#include <livelayer\livelayer.h>

class sn76core
{
	private:
		//mostly constants here
		static const uint8_t DEFAULT_CLOCK_OCT = 0x0A;
		static const uint8_t DEFAULT_CLOCK_DAC_HI = 0x03;
		static const uint8_t DEFAULT_CLOCK_DAC_LOW = 0xBF;
		static const uint8_t DEFAULT_AMP = 0x0F; //this is off
		static const uint8_t DEFAULT_NOISE = 0x04; //"white" noise at max clock

		static const uint8_t SET_CLOCK = 0x00;
		static const uint8_t SET_AMP = 0x01;
		static const uint8_t SET_FREQ = 0x02;
		static const uint8_t SET_NOISE = 0x03;
		static const uint8_t WRITE_CHIP = 0x04;

		serial_port* port;

	public:
		sn76core(serial_port* portref);
		void set_clock(int chip, uint8_t oct, uint8_t dac_high, uint8_t daclow);
		void set_freq(int chip, int channel, uint8_t freq_high, uint8_t freq_low);
		void set_amp(int chip, int channel,uint8_t amp);
		void set_noise(int chip, uint8_t setting);
};

#endif