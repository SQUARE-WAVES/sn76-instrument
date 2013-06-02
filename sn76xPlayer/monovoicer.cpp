#include "monovoicer.h"
#include<iostream>

monovoicer::monovoicer()
{
	present_note = 0xFF;
	for(int i=0;i<128;++i)
	{
		notes_on[i] = false;
	}
}

void monovoicer::note_on(unsigned char note,unsigned char vel)
{
	if(present_note > 0x7F) //midi ends at 7f
	{
		notes_on[note] = true;
		present_note = note;
		start_voice(note,vel);
	}
	else
	{
		notes_on[note] = true;
		present_note = note;
		change_voice(note,vel);
	}
}

void monovoicer::note_off(unsigned char note,unsigned char vel)
{
	notes_on[note] = false;
	
	if(note == present_note) //we have to change or end the voice
	{
		//early exit search the notes for the lowest note on
		for(int i=0;i<128;++i)
		{
			if(true == notes_on[i])
			{
				present_note = i;
				change_voice(i,vel);
				return; //break out, we are done
			}
		}
		//if we get to here that means there weren't any notes on at all.
		present_note = 0xFF;
		end_voice(note,vel);
	}	
}