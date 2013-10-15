#ifndef SNSEVENSIXPLAYER_DOT_H
#define SNSEVENSIXPLAYER_DOT_H

#include <stdint.h>
#include <livelayer/livelayer.h>
#include "sn76_core.h"
#include "monovoicer.h"
#include "param_table.h"

class sn76_voicer : public monovoicer
{
	protected:
		void start_voice(unsigned char note,unsigned char vel);
		void change_voice(unsigned char note,unsigned char vel);
		void end_voice(unsigned char note,unsigned char vel);

		sn76core* core;
		int chipnum;
		int channel;

		param_table<uint8_t,128> amp_table;
		param_table<int,128> note_table;
		param_table<int8_t,128> freq_table;

		void calculate_freq(unsigned char note);

	public:
		sn76_voicer(sn76core* coreref,int chipnum, int channel);
		void tick();

		virtual void set_table(int table, int position, int val,int jmp);
		
		void set_amptable(int position,uint8_t val, int jmp);
		void set_amp_instrument(int ampset);

		void set_notetable(int position,int val, int jmp);
		void set_note_instrument(int noteset);

		void set_freqtable(int position,int val, int jmp);
		void set_freq_instrument(int freqset);
		
		virtual void set_instrument(int pos);
};

//voicer for channels 3 and 4
class sn76_noise_voicer: public sn76_voicer
{
	protected:
		void start_voice(unsigned char note,unsigned char vel);
		void change_voice(unsigned char note,unsigned char vel);
		void end_voice(unsigned char note,unsigned char vel);

		param_table<uint8_t,128> noise_amp_table;
		param_table<uint8_t,128> wave_table;

		void calculate_freq(unsigned char note);

		sn76core* core;
		int chipnum;

	public:
		sn76_noise_voicer(sn76core* coreref,int chipnum6);

		void tick();

		void set_table(int table, int position, int val,int jmp);

		void set_noise_amp_table(int position,uint8_t val, int jmp);
		void set_noise_amp_instrument(int ampset);

		void set_wave_table(int position,uint8_t val,int jmp);
		void set_wave_instrument(int ampset);

		void set_instrument(int pos);
};

class sn76_player : public midi_port
{
	protected:

		void midi_callback(unsigned char status, unsigned char data1, unsigned char data2);
		void read_callback(uint8_t* buffer,int length);

	public:
		sn76_player(event_loop* looper,sn76core* core,int midi_in_port);

		sn76_voicer chip1_ch1;
		sn76_voicer chip1_ch2;
		sn76_noise_voicer chip1_noise;

		sn76_voicer chip2_ch1;
		sn76_voicer chip2_ch2;
		sn76_noise_voicer chip2_noise;

		sn76_voicer* get_voicer(int chip,int channel);
};

#endif