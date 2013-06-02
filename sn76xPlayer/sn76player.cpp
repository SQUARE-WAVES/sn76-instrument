#include "sn76player.h"
#include "mtof.h"

sn76_voicer::sn76_voicer(sn76core* coreref,int chipnum, int channel):
core(coreref),
chipnum(chipnum),
channel(channel),
amp_table(),
pos(0)
{
	
}

void sn76_voicer::start_voice(unsigned char note,unsigned char vel)
{
	core->set_freq(chipnum,channel,mtof_high[note],mtof_low[note]);

	//reset the table
	amp_table.reset();
	core->set_amp(chipnum,channel,0x0F - amp_table.val[pos]);
}

void sn76_voicer::change_voice(unsigned char note,unsigned char vel)
{
	core->set_freq(chipnum,channel,mtof_high[note],mtof_low[note]);
}

void sn76_voicer::end_voice(unsigned char note,unsigned char vel)
{
	core->set_amp(chipnum,channel,0x0F);
}

void sn76_voicer::tick()
{
	if(present_note != 0xFF)
	{
		if(amp_table.tick())
		{
			core->set_amp(chipnum,channel,0x0f - amp_table.present_val());
		}
	}
	/*
	int jmp = amp_table.jump[pos];
	int nextpos;
	if(present_note != 0xFF)
	{
		if( jmp >= 0)
		{
			nextpos = jmp;
		}
		else if( jmp == -1)
		{
			nextpos = pos + 1;
		}
		
		if(amp_table.val[nextpos] != amp_table.val[pos])
		{
			core->set_amp(chipnum,channel,0x0F - amp_table.val[nextpos]);
		}

		pos = nextpos;
	}
	*/
}

void sn76_voicer::set_amptable(int position, uint8_t value, int jump)
{
	amp_table.set_entry(position,value,jump);
}

sn76_noise_voicer::sn76_noise_voicer(sn76core* coreref,int chipnum):
core(coreref),
chipnum(chipnum)
{

}

void sn76_noise_voicer::start_voice(unsigned char note,unsigned char vel)
{
	core->set_freq(chipnum,2,mtof_high[note],mtof_low[note]);
	core->set_amp(chipnum,2,0x00);
}

void sn76_noise_voicer::change_voice(unsigned char note,unsigned char vel)
{
	core->set_freq(chipnum,2,mtof_high[note],mtof_low[note]);
}

void sn76_noise_voicer::end_voice(unsigned char note,unsigned char vel)
{
	core->set_amp(chipnum,2,0x0F);
}

sn76_player::sn76_player(event_loop* lp,sn76core* core,int portnum):
midi_port(lp,portnum),
chip1_ch1(core,0,0),
chip1_ch2(core,0,1),
chip1_noise(core,0),
chip2_ch1(core,1,0),
chip2_ch2(core,1,1),
chip2_noise(core,1)
{
	chip1_ch1.set_amptable(0,0x0F,-2);
	chip1_ch2.set_amptable(0,0x0F,-2);
	chip2_ch1.set_amptable(0,0x0F,-2);
	chip2_ch2.set_amptable(0,0x0F,-2);
}

void sn76_player::midi_callback(unsigned char status, unsigned char data1, unsigned char data2)
{
	uint8_t channel = status & 0x0F;
	uint8_t msg = status & 0xF0;
	monovoicer* voice = NULL;
	bool tick_track = false;

	switch(channel)
	{
		case 0x00:
			voice = &chip1_ch1;
		break;
		case 0x01:
			voice = &chip1_ch2;
		break;
		case 0x02:
			voice = &chip1_noise;
		break;
		case 0x03:
			voice = &chip2_ch1;
		break;
		case 0x04:
			voice = &chip2_ch2;
		break;
		case 0x05:
			voice = &chip2_noise;
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
			case 0x90:
				if(data2 == 0)
				{
					voice->note_off(data1,data2);
				}
				else
				{
					voice->note_on(data1,data2);
				}
			break;
			case 0x80:
				voice->note_off(data1,data2);
			break;
			default:
				//std::cout<<"unknown msg, status: 0x" << std::hex <<(unsigned int)status<<std::endl;
			break;
		}
	}

	if(tick_track && msg == 0x90)
	{
		chip1_ch1.tick();
	}
}