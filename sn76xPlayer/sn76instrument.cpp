#include "sn76instrument.h"

sn76_instrument::sn76_instrument(int size,int noise_size):
amp_table(size),
note_table(size),
freq_table(size),
noise_amp_table(noise_size),
wave_table(noise_size)
{
}

void sn76_instrument::set_table(int table,int pos,int val,int jmp)
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
			throw "invalid table guess";
		break;
	}
}