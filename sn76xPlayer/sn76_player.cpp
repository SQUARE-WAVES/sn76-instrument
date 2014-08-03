#include "sn76_player.h"

sn76_chip::sn76_chip(sn76_voicer* ch1, sn76_voicer* ch2,sn76_noise_voicer* chn):
channel1(ch1),
channel2(ch2),
noise(chn)
{
}

sn76_player::sn76_player(event_loop* looper,sn76_core* core,sn76_chip* ch1, sn76_chip* ch2,int midi_in_port):
midi_port(looper,midi_in_port),
chip1(ch1),
chip2(ch2)
{

}

void sn76_player::midi_callback(unsigned char status, unsigned char data1, unsigned char data2)
{
	uint8_t channel = status & 0x0F;
	uint8_t msg = status & 0xF0;
	chip_voicer* voice = NULL;

	bool tick_track = false;

	switch(channel)
	{
		case 0x00:
			voice = chip1->channel1;
		break;
		case 0x01:
			voice = chip1->channel2;
		break;
		case 0x02:
			voice = chip1->noise;
		break;
		case 0x03:
			voice = chip2->channel1;
		break;
		case 0x04:
			voice = chip2->channel2;
		break;
		case 0x05:
			voice = chip2->noise;
		break;
		case 0x0F:
			tick_track = true;
		break;
		default:
		break;
	}

	if(voice != NULL)
	{
		switch(msg)
		{
			case 0x90: //note on
				if(data2 == 0)
				{
					voice->note_off(data1,data2);
				}
				else
				{
					voice->note_on(data1,data2);
				}
			break;
			case 0x80: //note off
				voice->note_off(data1,data2);
			break;
			case 0xC0: //prog change
				voice->patch_change(data1);
			break;
			case 0xF8: //TICK
				
			default:
				//std::cout<<"unknown msg, status: 0x" << std::hex <<(unsigned int)status<<std::endl;
			break;
		}
	}
}

chip_voicer* sn76_player::get_voicer(int chip,int channel)
{
	switch(chip)
	{
		case 0:
			switch(channel)
			{
				case 0:
					return chip1->channel1;
				break;
				case 1:
					return chip1->channel2;
				break;
				case 2:
					return chip1->noise;
				break;
				default:
					return NULL;
				break;
			}
		break;

		case 1:
			switch(channel)
			{
				case 0:
					return chip2->channel1;
				break;
				case 1:
					return chip2->channel2;
				break;
				case 2:
					return chip2->noise;
				break;
				default:
					return NULL;
				break;
			}
		break;
		default:
			return NULL;
		break;
	}
}