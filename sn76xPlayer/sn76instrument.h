#ifndef SN76_INSTRUMENT_DOT_H
#define SN76_INSTRUMENT_DOT_H
#include<stdint.h>
#include "param_table.h"

class sn76_instrument
{
	public:
		param_table<uint8_t> amp_table;
		param_table<int> note_table;
		param_table<int8_t> freq_table;
		param_table<uint8_t> noise_amp_table;
		param_table<uint8_t> wave_table;

		sn76_instrument(int size,int noise_size);

		void set_table(int table,int pos,int val,int jmp);
};
#endif