#include "sn76_voicer.h"
#include "mtof.h"

sn76_voicer::sn76_voicer(sn76_core* coreref,int chipnum, int channel):
core(coreref),
chipnum(chipnum),
channel(channel),
amprunner(0),
noterunner(0),
freqrunner(0),
tick_counter(0),
tick_divider(1),
instrument(NULL)
{
	for(int i=0;i<128;++i)
	{
		patches[i] = NULL;
	}
}

void sn76_voicer::calculate_freq(unsigned char note)
{
	int offset_note = note + instrument->note_table.present_val(noterunner);
	uint16_t fval = mtof[offset_note];
	int detune = instrument->freq_table.present_val(freqrunner);
	fval = fval + detune;

	uint8_t fhigh = fval >> 4;
	uint8_t flow = fval & 0x0F;
	core->set_freq(chipnum,channel,fhigh,flow);
}

void sn76_voicer::start_voice(unsigned char note,unsigned char vel)
{
	//reset the tables
	instrument->amp_table.reset(amprunner);
	instrument->note_table.reset(noterunner);
	instrument->freq_table.reset(freqrunner);

	tick_counter = 0;
	
	calculate_freq(note);
	core->set_amp(chipnum,channel,0x0F - instrument->amp_table.present_val(amprunner));
}

void sn76_voicer::change_voice(unsigned char note,unsigned char vel)
{
	int offset_note = note + instrument->note_table.present_val(noterunner);
	calculate_freq(note);
}

void sn76_voicer::end_voice(unsigned char note,unsigned char vel)
{
	core->set_amp(chipnum,channel,0x0F);
}

void sn76_voicer::tick()
{
	if(present_note == 0xFF)
	{
		return;
	}

	if(tick_counter == 0)
	{
		if(instrument->amp_table.tick(amprunner))
		{
			core->set_amp(chipnum,channel,0x0f - instrument->amp_table.present_val(amprunner));
		}

		bool freqd = instrument->freq_table.tick(freqrunner);
		bool noted = instrument->note_table.tick(noterunner);
		if(freqd || noted)
		{
			calculate_freq(present_note);
		}
	}
	
	tick_counter++;
	if(tick_counter == tick_divider)
	{
		tick_counter = 0;
	}
}

void sn76_voicer::set_instrument(chip_instrument* new_instr)
{
	//fix the chip_instrument interface so this is safe!
	instrument = (sn76_instrument*) new_instr;
}

void sn76_voicer::set_patch(chip_instrument* instr, int patch_number)
{
	if(patch_number <= 127 && patch_number >= 0)
	{
		//POTENTIALLY UNSAFE CAST WOO-HA
		patches[patch_number] = (sn76_instrument*)instr;

		if(instrument == NULL)
		{
			set_instrument(instr);
		}
	}
}

void sn76_voicer::patch_change(int patch_number)
{
	if(patch_number > 127 || patch_number < 0)
	{
		return;
	}

	sn76_instrument* instr = patches[patch_number];

	if(instr != NULL)
	{
		instrument = instr;
	}
}

void sn76_voicer::midi_message(unsigned char message,unsigned char data1, unsigned char data2)
{
	switch(message)
		{
			case 0x90: //note on
				if(data2 == 0)
				{
					note_off(data1,data2);
				}
				else
				{
					note_on(data1,data2);
				}
			break;
			case 0x80: //note off
				note_off(data1,data2);
			break;
			case 0xC0: //prog change
				patch_change(data1);
			break;
			case 0xF8: //TICK
				tick();
			default:
				//std::cout<<"unknown msg, status: 0x" << std::hex <<(unsigned int)status<<std::endl;
			break;
		}
}

////-----------------------------------------------------------------------------------------------
///	NOISE VOICER
////-----------------------------------------------------------------------------------------------

sn76_noise_voicer::sn76_noise_voicer(sn76_core* coreref,int chipnum):
core(coreref),
chipnum(chipnum),
amprunner(0),
noterunner(0),
freqrunner(0),
noise_amprunner(0),
waverunner(0),
tick_counter(0),
instrument(NULL),
tick_divider(1)
{
	for(int i=0;i<128;++i)
	{
		patches[i] = NULL;
	}
}

void sn76_noise_voicer::calculate_freq(unsigned char note)
{
	int offset_note = note + instrument->note_table.present_val(noterunner);
	uint16_t fval = mtof[offset_note];
	int detune = instrument->freq_table.present_val(freqrunner);
	fval = fval + detune;

	uint8_t fhigh = fval >> 4;
	uint8_t flow = fval & 0x0F;
	core->set_freq(chipnum,2,fhigh,flow);
}

void sn76_noise_voicer::start_voice(unsigned char note,unsigned char vel)
{
		//reset the tables
	instrument->amp_table.reset(amprunner);
	instrument->noise_amp_table.reset(noise_amprunner);
	instrument->wave_table.reset(waverunner);
	instrument->note_table.reset(noterunner);
	instrument->freq_table.reset(freqrunner);

	tick_counter = 0;

	calculate_freq(note);
	core->set_amp(chipnum,2,0x0F - instrument->amp_table.present_val(amprunner));
	core->set_noise(chipnum,instrument->wave_table.present_val(waverunner));
	core->set_amp(chipnum,3,0x0F - instrument->noise_amp_table.present_val(noise_amprunner));
}

void sn76_noise_voicer::change_voice(unsigned char note,unsigned char vel)
{
	int offset_note = note + instrument->note_table.present_val(noterunner);
	calculate_freq(note);
}

void sn76_noise_voicer::end_voice(unsigned char note,unsigned char vel)
{
	core->set_amp(chipnum,2,0x0F);
	core->set_amp(chipnum,3,0x0F);
}

void sn76_noise_voicer::tick()
{
	if(present_note == 0xFF)
	{
		return;
	}

	if(tick_counter == 0)
	{
		if(instrument->amp_table.tick(amprunner))
		{
			core->set_amp(chipnum,2,0x0f - instrument->amp_table.present_val(amprunner));
		}

		bool freqd = instrument->freq_table.tick(freqrunner);
		bool noted = instrument->note_table.tick(noterunner);
		if(freqd || noted)
		{
			calculate_freq(present_note);
		}

		if(instrument->noise_amp_table.tick(noise_amprunner))
		{
			core->set_amp(chipnum,3,0x0f - instrument->noise_amp_table.present_val(noise_amprunner));
		}

		if(instrument->wave_table.tick(waverunner))
		{
			core->set_noise(chipnum,instrument->wave_table.present_val(waverunner));
		}
	}

	tick_counter++;
	if(tick_counter == tick_divider)
	{
		tick_counter = 0;
	}
}

void sn76_noise_voicer::set_instrument(chip_instrument* new_instr)
{
	//fix the chip_instrument interface so this is safe!
	instrument = (sn76_noise_instrument*) new_instr;
}

void sn76_noise_voicer::set_patch(chip_instrument* instr, int patch_number)
{
	if(patch_number <= 127 && patch_number >= 0)
	{
		//POTENTIALLY UNSAFE CAST WOO-HA
		patches[patch_number] = (sn76_noise_instrument*)instr;
	}

	if(instrument == NULL)
	{
		set_instrument(instr);
	}
}

void sn76_noise_voicer::patch_change(int patch_number)
{
	if(patch_number > 127 || patch_number < 0)
	{
		return;
	}

	sn76_noise_instrument* instr = patches[patch_number];
	if(instr != NULL)
	{
		instrument = instr;
	}
}

void sn76_noise_voicer::midi_message(unsigned char message,unsigned char data1, unsigned char data2)
{
	switch(message)
		{
			case 0x90: //note on
				if(data2 == 0)
				{
					note_off(data1,data2);
				}
				else
				{
					note_on(data1,data2);
				}
			break;
			case 0x80: //note off
				note_off(data1,data2);
			break;
			case 0xC0: //prog change
				patch_change(data1);
			break;
			case 0xF8: //TICK
				tick();
			default:
				//std::cout<<"unknown msg, status: 0x" << std::hex <<(unsigned int)status<<std::endl;
			break;
		}
}