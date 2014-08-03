#ifndef SN76_INSTRUMENT_DOT_H
#define SN76_INSTRUMENT_DOT_H
#include<stdint.h>
#include "param_table.h"

typedef int table_entry;

class chip_instrument
{
	public:

		virtual param_table* get_table(int table) = 0;
		virtual void set_table(int table,int pos,int val,int jmp);
		virtual void set_table_val(int table,int pos,int val);
		virtual void set_table_jmp(int table,int pos,int jmp);

		virtual void add_row(int index) = 0;
		virtual void remove_row(int index) = 0;

		virtual int get_rows() = 0;
		virtual int get_cols() = 0;
		virtual const char** get_labels() = 0;

		virtual int get_table_val(int table,int pos);
		virtual int get_table_jmp(int table,int pos);
};

class sn76_instrument : public chip_instrument
{
	public:
		static const char* LABELS[];

		param_table amp_table;
		param_table note_table;
		param_table freq_table;

		sn76_instrument(int size);


		param_table* get_table(int table);

		void add_row(int index);
		void remove_row(int index);

		int get_rows();
		int get_cols();
		const char** get_labels();
};

class sn76_noise_instrument : public chip_instrument
{
	public:
		static const char* LABELS[];

		param_table amp_table;
		param_table note_table;
		param_table freq_table;
		param_table noise_amp_table;
		param_table wave_table;

		sn76_noise_instrument(int size);

		param_table* get_table(int table);

		void add_row(int index);
		void remove_row(int index);

		int get_rows();
		int get_cols();
		const char** get_labels();
};

#endif