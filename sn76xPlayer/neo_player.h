#ifndef NEO_PLAYER_DOT_H
#define NEO_PLAYER_DOT_H

#include <stdint.h> //for MIDI byte types
#include <livelayer/livelayer.h>
#include <vector>
#include "sn76_core.h"
#include "sn76_voicer.h"
#include "sn76_instrument.h"

class neo_player;

class neo_port : public midi_port
{
	protected:
		void midi_callback(unsigned char status, unsigned char data1, unsigned char data2);
		neo_player* boss;

	public:
		neo_port(event_loop* looper,neo_player* boss, int midi_in_port);
};

class neo_player
{

	protected:
		chip_voicer* channel_map[16];
		neo_port* port;

		void scrub_channel_map();
		void send_to_all_voicers(unsigned char message, unsigned char data1, unsigned char data2);

	public:
		neo_player();
		~neo_player();
		
		bool is_open();

		bool set_voicer(int channel_number,chip_voicer* voicer);
		bool unset_voicer(int channel_number);

		bool open_midi_port(int midi_in_port,event_loop* looper);
		void close_midi_port();

		void midi_in(unsigned char message,int channel, unsigned char data1, unsigned char data2);
};

#endif