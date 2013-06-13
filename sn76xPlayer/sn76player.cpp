#include "sn76player.h"
#include "mtof.h"

sn76_voicer::sn76_voicer(sn76core* coreref,int chipnum, int channel):
core(coreref),
chipnum(chipnum),
channel(channel),
amp_table(),
note_table(),
freq_table()
{
	amp_table.set_entry(0,0x0F,-2);
	note_table.set_entry(0,0,-2);
	freq_table.set_entry(0,0,-2);
}

void sn76_voicer::calculate_freq(unsigned char note)
{
	int offset_note = note + note_table.present_val();
	uint16_t fval = mtof[offset_note];
	int detune = freq_table.present_val();
	fval = fval + detune;

	uint8_t fhigh = fval >> 4;
	uint8_t flow = fval & 0x0F;
	core->set_freq(chipnum,channel,fhigh,flow);
}

void sn76_voicer::start_voice(unsigned char note,unsigned char vel)
{
	//reset the tables
	amp_table.reset();
	note_table.reset();
	freq_table.reset();

	
	calculate_freq(note);
	core->set_amp(chipnum,channel,0x0F - amp_table.present_val());
}

void sn76_voicer::change_voice(unsigned char note,unsigned char vel)
{
	int offset_note = note + note_table.present_val();
	calculate_freq(note);
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

		bool freqd = freq_table.tick();
		bool noted = note_table.tick();
		if(freqd || noted)
		{
			calculate_freq(present_note);
		}
	}
}

void sn76_voicer::set_amptable(int position, uint8_t value, int jump)
{
	amp_table.set_entry(position,value,jump);
}

void sn76_voicer::set_amp_instrument(int ampset)
{
	amp_table.set_reset(ampset);
}

void sn76_voicer::set_notetable(int position,int val, int jmp)
{
	note_table.set_entry(position,val,jmp);
}

void sn76_voicer::set_note_instrument(int ampset)
{
	note_table.set_reset(ampset);
}

void sn76_voicer::set_freqtable(int position, int value, int jump)
{
	freq_table.set_entry(position,value,jump);
}

void sn76_voicer::set_freq_instrument(int ampset)
{
	freq_table.set_reset(ampset);
}

void sn76_voicer::set_table(int table,int pos,int val,int jmp)
{
	switch(table)
	{
		case 0:
			amp_table.set_entry(pos,val,jmp);
		break;
		case 1:
			note_table.set_entry(pos,val,jmp);
		break;
		case 2:
			freq_table.set_entry(pos,val,jmp);
		break;
		default:
			throw "table doesn't exist";
		break;
	}
}

sn76_noise_voicer::sn76_noise_voicer(sn76core* coreref,int chipnum):
sn76_voicer(coreref,chipnum,2),
core(coreref),
chipnum(chipnum),
noise_amp_table(),
wave_table()
{
	amp_table.set_entry(0,0x0F,-2);
	note_table.set_entry(0,0,-2);
	freq_table.set_entry(0,0,-2);
	noise_amp_table.set_entry(0,0x00,-2);
	wave_table.set_entry(0,0x03,-2);
}

void sn76_noise_voicer::calculate_freq(unsigned char note)
{
	int offset_note = note + note_table.present_val();
	uint16_t fval = mtof[offset_note];
	int detune = freq_table.present_val();
	fval = fval + detune;

	uint8_t fhigh = fval >> 4;
	uint8_t flow = fval & 0x0F;
	core->set_freq(chipnum,2,fhigh,flow);
}

void sn76_noise_voicer::start_voice(unsigned char note,unsigned char vel)
{
		//reset the tables
	amp_table.reset();
	noise_amp_table.reset();
	wave_table.reset();
	note_table.reset();
	freq_table.reset();

	calculate_freq(note);
	core->set_amp(chipnum,2,0x0F - amp_table.present_val());
	core->set_noise(chipnum,wave_table.present_val());
	core->set_amp(chipnum,3,0x0F - noise_amp_table.present_val());
}

void sn76_noise_voicer::change_voice(unsigned char note,unsigned char vel)
{
	int offset_note = note + note_table.present_val();
	calculate_freq(note);
}

void sn76_noise_voicer::end_voice(unsigned char note,unsigned char vel)
{
	core->set_amp(chipnum,2,0x0F);
	core->set_amp(chipnum,3,0x0F);
}

void sn76_noise_voicer::tick()
{
	if(present_note != 0xFF)
	{
		if(amp_table.tick())
		{
			core->set_amp(chipnum,2,0x0f - amp_table.present_val());
		}

		bool freqd = freq_table.tick();
		bool noted = note_table.tick();
		if(freqd || noted)
		{
			calculate_freq(present_note);
		}

		if(noise_amp_table.tick())
		{
			core->set_amp(chipnum,3,0x0f - noise_amp_table.present_val());
		}

		if(wave_table.tick())
		{
			core->set_noise(chipnum,wave_table.present_val());
		}
	}
}

void sn76_noise_voicer::set_noise_amp_table(int position, uint8_t value, int jump)
{
	noise_amp_table.set_entry(position,value,jump);
}

void sn76_noise_voicer::set_noise_amp_instrument(int ampset)
{
	noise_amp_table.set_reset(ampset);
}

void sn76_noise_voicer::set_wave_table(int position, uint8_t value, int jump)
{
	wave_table.set_entry(position,value,jump);
}

void sn76_noise_voicer::set_wave_instrument(int ampset)
{
	wave_table.set_reset(ampset);
}

void sn76_noise_voicer::set_table(int table,int pos,int val,int jmp)
{
	switch(table)
	{
		case 0:
			amp_table.set_entry(pos,val,jmp);
		break;
		case 1:
			note_table.set_entry(pos,val,jmp);
		break;
		case 2:
			freq_table.set_entry(pos,val,jmp);
		break;
		case 3:
			noise_amp_table.set_entry(pos,val,jmp);
		break;
		case 4:
			wave_table.set_entry(pos,val,jmp);
		break;
		default:
			throw "crazed table guess";
		break;
	}
}

sn76_player::sn76_player(event_loop* lp,sn76core* core,int midi_port):
midi_port(lp,midi_port),
chip1_ch1(core,0,0),
chip1_ch2(core,0,1),
chip1_noise(core,0),
chip2_ch1(core,1,0),
chip2_ch2(core,1,1),
chip2_noise(core,1)
{

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
		chip1_ch2.tick();
		chip1_noise.tick();

		chip2_ch1.tick();
		chip2_ch2.tick();
		chip2_noise.tick();
	}
}

sn76_voicer* sn76_player::get_voicer(int chip,int channel)
{
	switch(chip)
	{
		case 0:
			switch(channel)
			{
				case 0:
					return &chip1_ch1;
				break;
				case 1:
					return &chip1_ch2;
				break;
				case 2:
					return &chip1_noise;
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
					return &chip2_ch1;
				break;
				case 1:
					return &chip2_ch2;
				break;
				case 2:
					return &chip2_noise;
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