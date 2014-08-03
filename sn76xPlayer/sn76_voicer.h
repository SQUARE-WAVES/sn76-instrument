#ifndef SN76_VOICER
#define SN76_VOICER

#include "sn76_core.h"
#include "monovoicer.h"
#include "sn76_instrument.h"

class chip_voicer : public monovoicer
{
	public:
		virtual void set_instrument(chip_instrument* new_instr) = 0;
		virtual void set_patch(chip_instrument* instr,int patch_number) = 0;
		virtual void patch_change(int newPatch) = 0;
		
		virtual void midi_message(unsigned char message,unsigned char data1, unsigned char data2) = 0;
};

class sn76_voicer : public chip_voicer
{
	protected:
		void start_voice(unsigned char note,unsigned char vel);
		void change_voice(unsigned char note,unsigned char vel);
		void end_voice(unsigned char note,unsigned char vel);

		sn76_core* core;
		int chipnum;
		int channel;

		void calculate_freq(unsigned char note);

		int amprunner;
		int noterunner;
		int freqrunner;

		int tick_counter;
		int tick_divider;

		sn76_instrument* instrument;
		sn76_instrument* patches[128];

	public:

		sn76_voicer(sn76_core* coreref,int chipnum, int channel);
		void tick();
		void set_instrument(chip_instrument* new_instr);
		void set_patch(chip_instrument* instr,int patch_number);
		void patch_change(int newPatch);

		void midi_message(unsigned char message,unsigned char data1, unsigned char data2);
};

//voicer for channels 3 and 4
class sn76_noise_voicer: public chip_voicer
{
	protected:
		void start_voice(unsigned char note,unsigned char vel);
		void change_voice(unsigned char note,unsigned char vel);
		void end_voice(unsigned char note,unsigned char vel);

		void calculate_freq(unsigned char note);

		sn76_core* core;
		int chipnum;

		int amprunner;
		int noterunner;
		int freqrunner;
		int noise_amprunner;
		int waverunner;

		int tick_counter;
		int tick_divider;

		sn76_noise_instrument* instrument;
		sn76_noise_instrument* patches[128];

	public:
		sn76_noise_voicer(sn76_core* coreref,int chipnum);
		void tick();
		void set_instrument(chip_instrument* new_instr);
		void set_patch(chip_instrument* instr,int patch_number);
		void patch_change(int newPatch);

		void midi_message(unsigned char message,unsigned char data1, unsigned char data2);
};


#endif