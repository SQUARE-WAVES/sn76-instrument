#include "param_table.h"

param_table::param_table(unsigned int size,int min, int max):
jump(size,0),
val(size,0),
val_min(min),
val_max(max)
{
	reset_pos = 0;
	jmp_min = -2;
}

int param_table::present_val(int pos)
{
	return val[pos];
}

void param_table::reset(int& pos)
{
	pos = reset_pos;
}

int param_table::clip_val(int val)
{
	int clipped = std::min(val,val_max);
	return std::max(clipped,val_min);
}

int param_table::clip_pos(int pos)
{
	int max = jump.size() - 1;
	int clipped_pos = std::min(pos,max);
	return  std::max(clipped_pos,0);
}

int param_table::clip_jmp(int jmp)
{
	int max = jump.size() - 1;
	int clipped_jmp = std::min(jmp,max);
	return  std::max(clipped_jmp,jmp_min);
}

void param_table::set_val(int pos, int newval)
{
	int clipped_pos = clip_pos(pos);
	int clipped_val = clip_val(newval);

	val[clipped_pos] = clipped_val;
}

void param_table::set_jmp(int pos, int newjmp)
{
	int clipped_pos = clip_pos(pos);
	int clipped_jmp = clip_jmp(newjmp);

	jump[clipped_pos] = clipped_jmp;
}

int param_table::get_val(int pos)
{
	int clipped_pos = clip_pos(pos);
	return val.at(clipped_pos);
}

int param_table::get_jmp(int pos)
{
	int clipped_pos = clip_pos(pos);
	return jump.at(clipped_pos);
}

void param_table::set_entry(int pos, int newval, int newjmp)
{
	set_val(pos,newval);
	set_jmp(pos,newjmp);
}

void param_table::set_reset(int new_reset)
{
	int max = jump.size() - 1;
	int clipped = std::min(new_reset,max);
	clipped = std::max(new_reset,0);

	reset_pos = clipped;
}

bool param_table::tick(int& pos)
{
	int max = jump.size() - 1;
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

void param_table::delete_row(int index)
{
	if(jump.size() == 0){
		return;
	}

	int max = jump.size() -1;
	int clipped = std::min(index,max);

	val.erase(val.begin() + clipped,val.begin() + (clipped + 1));
	jump.erase(jump.begin() + clipped,jump.begin() + (clipped + 1));
}

void param_table::add_row(int index)
{
	int max = jump.size();
	int clipped = std::min(index,max);

	val.insert(val.begin() + clipped,0);
	jump.insert(jump.begin() + clipped,0);
}

int param_table::get_length()
{
	return val.size();
}