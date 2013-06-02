template<class T, int size> class param_table
{
	private:
		int pos;

	public:
		int jump[size];
		T val[size];
		int max;

		param_table()
		{
			pos = 0;
			max = size -1;
			memset(jump,0,sizeof(jump));
			memset(val,0,sizeof(val));
		}

		T present_val()
		{
			return val[pos];
		}

		void reset()
		{
			pos = 0;
		}

		void set_entry(int pos, T newval, int newjmp)
		{
			int clipped_pos = min(pos,max);
			clipped_pos = max(clipped_pos,0);

			jump[clipped_pos] = newjmp;
			val[clipped_pos] = newval;
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
				nextpos = max(max,pos + 1);

			}
		
			if(val[nextpos] != val[pos])
			{
				val_changed = true;
			}

			pos = nextpos;

			return val_changed;
		}

		
};