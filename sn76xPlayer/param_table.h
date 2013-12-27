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

template<class T> class param_table
{
	private:
		int pos;

	public:
		std::vector<int> jump;
		std::vector<T> val;
		int max;
		int reset_pos;

		param_table(unsigned int size):
		jump(size,0),
		val(size,0)
		{
			pos = 0;
			reset_pos = 0;
			max = size -1;
		}

		T present_val()
		{
			return val[pos];
		}

		void reset()
		{
			pos = reset_pos;
		}

		void set_entry(int pos, T newval, int newjmp)
		{
			int clipped_pos = std::min(pos,max);
			clipped_pos = std::max(clipped_pos,0);

			jump[clipped_pos] = newjmp;
			val[clipped_pos] = newval;
		}

		void set_reset(int new_reset)
		{
			int clipped = std::min(new_reset,max);
			clipped = std::max(new_reset,0);

			reset_pos = clipped;
		}

		bool tick()
		{
			int jmp = jump[pos];
			int nextpos = pos;
			bool val_changed = false;

			if( jmp >= 0)
			{
				nextpos = jmp;
			}
			else if( jmp == -1)
			{
				nextpos = std::min(max,pos + 1);
			}
		
			if(val[nextpos] != val[pos])
			{
				val_changed = true;
			}

			pos = nextpos;

			return val_changed;
		}
};
#endif