#ifndef SNSEVENSIXPLAYER_DOT_H
#define SNSEVENSIXPLAYER_DOT_H

#include <stdint.h>
#include <livelayer/livelayer.h>
#include <vector>
#include "sn76_core.h"
#include "sn76_voicer.h"
#include "sn76_instrument.h"

//just a dumb struct
class sn76_chip
{
	public:
		sn76_voicer* channel1;
		sn76_voicer* channel2;
		sn76_noise_voicer* noise;

		sn76_chip(sn76_voicer* ch1, sn76_voicer* ch2,sn76_noise_voicer* chn);

};

class sn76_player : public midi_port
{
	protected:

		void midi_callback(unsigned char status, unsigned char data1, unsigned char data2);
		void read_callback(uint8_t* buffer,int length);

		sn76_chip* chip1;
		sn76_chip* chip2;

		chip_voicer* channel_map[16];

	public:
		sn76_player(event_loop* looper,sn76_core* core,sn76_chip* ch1, sn76_chip* ch2,int midi_in_port);
		chip_voicer* get_voicer(int chip,int channel);
};

#endif