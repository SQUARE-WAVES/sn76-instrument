#include "sn76_instrument.h"

void chip_instrument::set_table(int table,int pos,int val,int jmp)
{
	param_table* tab = get_table(table);
	tab->set_entry(pos,val,jmp);
}

void chip_instrument::set_table_val(int table,int pos,int val)
{
	param_table* tab = get_table(table);
	tab->set_val(pos,val);
}

void chip_instrument::set_table_jmp(int table,int pos,int jmp)
{
	param_table* tab = get_table(table);
	tab->set_jmp(pos,jmp);
}

int chip_instrument::get_table_val(int table, int pos)
{
	param_table* tab = get_table(table);
	return tab->get_val(pos);
}

int chip_instrument::get_table_jmp(int table, int pos)
{
	param_table* tab = get_table(table);
	return tab->get_jmp(pos);
}

const char* sn76_instrument::LABELS[] = {"amp","note","freq"};

sn76_instrument::sn76_instrument(int size):
amp_table(size,0,0x0F),
note_table(size,-24,24),
freq_table(size,-50,50)
{
}

param_table* sn76_instrument::get_table(int table)
{
	switch(table)
	{
		case 0:
			return &amp_table;
		break;
		case 1:
			return &note_table;
		break;
		case 2:
			return &freq_table;
		break;
		default:
			throw "invalid table guess";
		break;
	}
}

void sn76_instrument::add_row(int index)
{
	amp_table.add_row(index);
	note_table.add_row(index);
	freq_table.add_row(index);
}

void sn76_instrument::remove_row(int index)
{
	amp_table.delete_row(index);
	note_table.delete_row(index);
	freq_table.delete_row(index);
}

int sn76_instrument::get_rows()
{
	return amp_table.get_length();
}

int sn76_instrument::get_cols()
{
	return sizeof(sn76_instrument::LABELS) / sizeof(const char*);
}

const char** sn76_instrument::get_labels()
{
	return sn76_instrument::LABELS;
}


const char* sn76_noise_instrument::LABELS[] = {"amp","note","freq","noise_amp","wave"};

sn76_noise_instrument::sn76_noise_instrument(int size):
amp_table(size,0,0x0F),
note_table(size,-24,24),
freq_table(size,-50,50),
noise_amp_table(size,0,0x0F),
wave_table(size,0,7)
{
}

param_table* sn76_noise_instrument::get_table(int tab)
{
	switch(tab)
	{
		case 0:
			return &amp_table;
		break;
		case 1:
			return &note_table;
		break;
		case 2:
			return &freq_table;
		break;
		case 3:
			return &noise_amp_table;
		break;
		case 4:
			return &wave_table;
		break;
		default:
			throw "invalid table guess";
		break;
	}
}

void sn76_noise_instrument::add_row(int index)
{
	amp_table.add_row(index);
	note_table.add_row(index);
	freq_table.add_row(index);
	noise_amp_table.add_row(index);
	wave_table.add_row(index);
}

void sn76_noise_instrument::remove_row(int index)
{
	amp_table.delete_row(index);
	note_table.delete_row(index);
	freq_table.delete_row(index);
	noise_amp_table.delete_row(index);
	wave_table.delete_row(index);
}

int sn76_noise_instrument::get_rows()
{
	return amp_table.get_length();
}

int sn76_noise_instrument::get_cols()
{
	return sizeof(sn76_noise_instrument::LABELS) / sizeof(const char*);
}

const char** sn76_noise_instrument::get_labels()
{
	return sn76_noise_instrument::LABELS;
}