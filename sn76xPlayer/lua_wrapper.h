#ifndef CLUA_DOT_H
#define CLUA_DOT_H

#include <lua.hpp>
#include <livelayer/livelayer.h>
#include "sn76player.h"

class lua_wrapper
{
	private:
		lua_State* L;
		event_loop* loop;

		bool interface_registered;

	public:
		lua_wrapper();
		~lua_wrapper();
		
		inline operator lua_State* () {return L;}
		
		const static char interface_meta_name[];

		int open_script(const char* path);
		void print_error();
		void register_function(const char* name, int (*method) (lua_State* Lwrap));
		void register_interface(const char* name,event_loop* loop);

		static void lua_thread(const char* path,event_loop* loop);
};

#endif