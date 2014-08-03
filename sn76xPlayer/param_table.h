#ifndef PARAM_TABLE_DOT_H
#define PARAM_TABLE_DOT_H
#include <vector>
#include <algorithm>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

class param_table
{
	public:
		std::vector<int> jump;
		std::vector<int> val;
		int reset_pos;
		int val_min;
		int val_max;
		int jmp_min;

		param_table(unsigned int size,int min, int max);

		int present_val(int pos);
		void reset(int& pos);

		int clip_val(int val);
		int clip_pos(int pos);
		int clip_jmp(int jmp);
		
		void set_val(int pos, int newval);
		void set_jmp(int pos, int newjmp);

		int get_val(int pos);
		int get_jmp(int pos);

		void set_entry(int pos, int newval, int newjmp);
		void set_reset(int new_reset);
		bool tick(int& pos);
		void delete_row(int index);
		void add_row(int index);
		int get_length();
};

#endif