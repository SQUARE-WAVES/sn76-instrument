#include "neo_player.h"

neo_port::neo_port(event_loop* looper,neo_player* boss, int midi_in_port):
midi_port(looper,midi_in_port),
boss(boss)
{
	ignore_type(true,false,true);
}

void neo_port::midi_callback(unsigned char status, unsigned char data1, unsigned char data2)
{
	uint8_t channel = status & 0x0F;
	uint8_t msg = status & 0xF0;

	if(status >= 0xF0)
	{
		boss->midi_in(status,0,data1,data2);
	}
	else
	{
		boss->midi_in(msg,channel,data1,data2);
	}
	
}

neo_player::neo_player()
{
	//ok null out the channel map
	scrub_channel_map();
	port = NULL;
}

neo_player::~neo_player()
{
	close_midi_port();
}

void neo_player::scrub_channel_map()
{
	memset(channel_map,0x00,sizeof(channel_map));
}

bool neo_player::is_open()
{
	return port != NULL;
}

bool neo_player::set_voicer(int channel_number,chip_voicer* voicer)
{
	if(channel_number >= 0 && channel_number <16)
	{
		channel_map[channel_number] = voicer;
		return true;
	}
	else
	{
		return false;
	}
}

bool neo_player::unset_voicer(int channel_number)
{
	if(channel_number >= 0 && channel_number <16)
	{
		channel_map[channel_number] = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

bool neo_player::open_midi_port(int midi_in_port,event_loop* looper)
{
	if(is_open())
	{
		return false;
	}
	else
	{
		port = new neo_port(looper,this,midi_in_port);
		return true;
	}
}

void neo_player::close_midi_port()
{
	if(is_open())
	{
		delete port;
		port = NULL;
	}
}

void neo_player::send_to_all_voicers(unsigned char message, unsigned char data1, unsigned char data2)
{
	for(int i=0;i<16;++i)
	{
		chip_voicer* vc = channel_map[i];
		if(vc != NULL)
		{
			vc->midi_message(message,data1,data2);
		}
	}
}

void neo_player::midi_in(unsigned char message,int channel, unsigned char data1, unsigned char data2)
{
	if(message >= 0xF0) //its a system message, send it to everyone
	{
		send_to_all_voicers(message,data1,data2);
	}
	else //its a channel message send it to the right guy
	{
		chip_voicer* vc = channel_map[channel];
		if(vc != NULL)
		{
			vc->midi_message(message,data1,data2);
		}
	}
}