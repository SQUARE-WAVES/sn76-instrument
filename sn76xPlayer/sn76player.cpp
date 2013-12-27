#include "sn76player.h"
#include "mtof.h"

sn76_voicer::sn76_voicer(sn76core* coreref,int chipnum, int channel):
core(coreref),
chipnum(chipnum),
channel(channel),
instrument(128,128)
{
	instrument.amp_table.set_entry(0,0x0F,-2);
	instrument.note_table.set_entry(0,0,-2);
	instrument.freq_table.set_entry(0,0,-2);
}

void sn76_voicer::calculate_freq(unsigned char note)
{
	int offset_note = note + instrument.note_table.present_val();
	uint16_t fval = mtof[offset_note];
	int detune = instrument.freq_table.present_val();
	fval = fval + detune;

	uint8_t fhigh = fval >> 4;
	uint8_t flow = fval & 0x0F;
	core->set_freq(chipnum,channel,fhigh,flow);
}

void sn76_voicer::start_voice(unsigned char note,unsigned char vel)
{
	//reset the tables
	instrument.amp_table.reset();
	instrument.note_table.reset();
	instrument.freq_table.reset();

	
	calculate_freq(note);
	core->set_amp(chipnum,channel,0x0F - instrument.amp_table.present_val());
}

void sn76_voicer::change_voice(unsigned char note,unsigned char vel)
{
	int offset_note = note + instrument.note_table.present_val();
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
		if(instrument.amp_table.tick())
		{
			core->set_amp(chipnum,channel,0x0f - instrument.amp_table.present_val());
		}

		bool freqd = instrument.freq_table.tick();
		bool noted = instrument.note_table.tick();
		if(freqd || noted)
		{
			calculate_freq(present_note);
		}
	}
}

sn76_noise_voicer::sn76_noise_voicer(sn76core* coreref,int chipnum):
sn76_voicer(coreref,chipnum,2),
core(coreref),
chipnum(chipnum)
{
	instrument.amp_table.set_entry(0,0x0F,-2);
	instrument.note_table.set_entry(0,0,-2);
	instrument.freq_table.set_entry(0,0,-2);
	instrument.noise_amp_table.set_entry(0,0x00,-2);
	instrument.wave_table.set_entry(0,0x03,-2);
}

void sn76_noise_voicer::calculate_freq(unsigned char note)
{
	int offset_note = note + instrument.note_table.present_val();
	uint16_t fval = mtof[offset_note];
	int detune = instrument.freq_table.present_val();
	fval = fval + detune;

	uint8_t fhigh = fval >> 4;
	uint8_t flow = fval & 0x0F;
	core->set_freq(chipnum,2,fhigh,flow);
}

void sn76_noise_voicer::start_voice(unsigned char note,unsigned char vel)
{
		//reset the tables
	instrument.amp_table.reset();
	instrument.noise_amp_table.reset();
	instrument.wave_table.reset();
	instrument.note_table.reset();
	instrument.freq_table.reset();

	calculate_freq(note);
	core->set_amp(chipnum,2,0x0F - instrument.amp_table.present_val());
	core->set_noise(chipnum,instrument.wave_table.present_val());
	core->set_amp(chipnum,3,0x0F - instrument.noise_amp_table.present_val());
}

void sn76_noise_voicer::change_voice(unsigned char note,unsigned char vel)
{
	int offset_note = note + instrument.note_table.present_val();
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
		if(instrument.amp_table.tick())
		{
			core->set_amp(chipnum,2,0x0f - instrument.amp_table.present_val());
		}

		bool freqd = instrument.freq_table.tick();
		bool noted = instrument.note_table.tick();
		if(freqd || noted)
		{
			calculate_freq(present_note);
		}

		if(instrument.noise_amp_table.tick())
		{
			core->set_amp(chipnum,3,0x0f - instrument.noise_amp_table.present_val());
		}

		if(instrument.wave_table.tick())
		{
			core->set_noise(chipnum,instrument.wave_table.present_val());
		}
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
	sn76_voicer* voice = NULL;
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
				//voice->set_instrument(data1);
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