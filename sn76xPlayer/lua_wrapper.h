#ifndef CLUA_DOT_H
#define CLUA_DOT_H

#include <lua.hpp>
#include <livelayer/livelayer.h>
#include "sn76_player.h"

class lua_wrapper
{
	private:
		lua_State* L;

	public:
		lua_wrapper();
		~lua_wrapper();
		
		inline operator lua_State* () {return L;}

		const static char player_metatable_name[];
		const static char instrument_meta_name[];
		const static char noise_instrument_meta_name[];
		const static char voicer_meta_name[];
		const static char noise_voicer_meta_name[];
		
		//WOW THIS IS AWFUL PLEASE DO A BETTER JOB OF THIS
		static event_loop* global_loop;
		static sn76_player* global_player;

		int open_script(const char* path);
		void print_error();
		void register_function(const char* name, int (*method) (lua_State* Lwrap));
		void setup_lua_env(event_loop* loop);

		static void lua_thread(const char* path,event_loop* loop);
};

#endif