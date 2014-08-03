#ifndef monovoicer_DOT_H
#define monovoicer_DOT_H

class monovoicer
{
	protected:
		virtual void start_voice(unsigned char note,unsigned char vel) =0;
		virtual void change_voice(unsigned char note,unsigned char vel) =0;
		virtual void end_voice(unsigned char note,unsigned char vel) =0;
		
		bool notes_on[128];
		unsigned char present_note;
		
	public:
		monovoicer();
		void note_on(unsigned char note,unsigned char vel);
		void note_off(unsigned char note,unsigned char vel);
};

#endif