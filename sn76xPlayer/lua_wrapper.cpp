#include "lua_wrapper.h"
#include "lua_functions.h"
#include <iostream>
#include <vector>
#include "sn76_instrument.h"

const char lua_wrapper::instrument_meta_name[] = "INSTRMETA";
const char lua_wrapper::noise_instrument_meta_name[] = "NOISEINSTRMETA";
const char lua_wrapper::voicer_meta_name[] = "VOICERMETA";
const char lua_wrapper::noise_voicer_meta_name[] = "NOISEVOICERMETA";
const char lua_wrapper::player_metatable_name[] = "PLAYERMETA";

event_loop* lua_wrapper::global_loop = NULL;

lua_wrapper::lua_wrapper()
{
	L = lua_open();
	luaL_openlibs(L);	
	
	//set up the path
	luaL_dostring(L,"package.path = package.path .. ';../scripts/?.lua;../scripts/widgets/?.lua'");
	//here is some bullshit
	luaL_dostring(L,"package.path = package.path .. ';./?.lua;C:/Program Files (x86)/Lua/5.1/lua/?.lua;C:/Program Files (x86)/Lua/5.1/lua/?/init.lua;C:/Program Files (x86)/Lua/5.1/?.lua;C:/Program Files (x86)/Lua/5.1/?/init.lua;C:/Program Files (x86)/Lua/5.1/lua/?.luac'");
	luaL_dostring(L,"package.cpath = package.cpath .. './?.dll;./?51.dll;C:/Program Files (x86)/Lua/5.1/?.dll;C:/Program Files (x86)/Lua/5.1/?51.dll;C:/Program Files (x86)/Lua/5.1/clibs/?.dll;C:/Program Files (x86)/Lua/5.1/clibs/?51.dll;C:/Program Files (x86)/Lua/5.1/loadall.dll;C:/Program Files (x86)/Lua/5.1/clibs/loadall.dll'");
}

lua_wrapper::~lua_wrapper()
{
	lua_close(L);
}

int lua_wrapper::open_script(const char* path)
{
	return luaL_dofile(L,path);
}

void lua_wrapper::print_error()
{
	std::cout << lua_tostring(L,-1);	
}

void lua_wrapper::register_function(const char* name, int (*method) (lua_State* L))
{
	lua_pushcfunction(L, method);
	lua_setglobal(L, name);
}

//some helpers
void setup_voicer_metatable(lua_State* L)
{
	luaL_newmetatable(L,lua_wrapper::voicer_meta_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-1,"__index");

	lua_pushcfunction(L,lua_set_instrument);
	lua_setfield(L,-2,"set_instrument");

	lua_pushcfunction(L,lua_set_patch);
	lua_setfield(L,-2,"set_patch");

	lua_pushcfunction(L,lua_unset_pach);
	lua_setfield(L,-2,"unset_patch");

	lua_settop(L, 0);
}

void setup_noise_voicer_metatable(lua_State* L)
{
	luaL_newmetatable(L,lua_wrapper::noise_voicer_meta_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-1,"__index");

	lua_pushcfunction(L,lua_set_noise_instrument);
	lua_setfield(L,-2,"set_instrument");

	lua_pushcfunction(L,lua_set_noise_patch);
	lua_setfield(L,-2,"set_patch");

	lua_pushcfunction(L,lua_unset_pach);
	lua_setfield(L,-2,"unset_patch");

	lua_settop(L, 0);
}

void setup_instrument_metatable(lua_State* L)
{
	luaL_newmetatable(L,lua_wrapper::instrument_meta_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-1,"__index");

	lua_pushcfunction(L,lua_set_instrument_val);
	lua_setfield(L,-2,"set_val");

	lua_pushcfunction(L,lua_set_instrument_jmp);
	lua_setfield(L,-2,"set_jmp");

	lua_pushcfunction(L,lua_get_instrument_details);
	lua_setfield(L,-2,"get_details");

	lua_pushcfunction(L,lua_get_instrument_val);
	lua_setfield(L,-2,"get_val");

	lua_pushcfunction(L,lua_get_instrument_jmp);
	lua_setfield(L,-2,"get_jmp");

	lua_pushcfunction(L,lua_add_instrument_row);
	lua_setfield(L,-2,"add_row");

	lua_pushcfunction(L,lua_remove_instrument_row);
	lua_setfield(L,-2,"remove_row");

	lua_pushcfunction(L,lua_dump_regular_instrument);
	lua_setfield(L,-2,"dump");

	lua_settop(L, 0);
}

void setup_noise_instrument_metatable(lua_State* L)
{
	luaL_newmetatable(L,lua_wrapper::noise_instrument_meta_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-1,"__index");

	lua_pushcfunction(L,lua_set_noise_instrument_val);
	lua_setfield(L,-2,"set_val");

	lua_pushcfunction(L,lua_set_noise_instrument_jmp);
	lua_setfield(L,-2,"set_jmp");

	lua_pushcfunction(L,lua_get_noise_instrument_details);
	lua_setfield(L,-2,"get_details");

	lua_pushcfunction(L,lua_get_noise_instrument_val);
	lua_setfield(L,-2,"get_val");

	lua_pushcfunction(L,lua_get_noise_instrument_jmp);
	lua_setfield(L,-2,"get_jmp");

	lua_pushcfunction(L,lua_add_noise_instrument_row);
	lua_setfield(L,-2,"add_row");

	lua_pushcfunction(L,lua_remove_noise_instrument_row);
	lua_setfield(L,-2,"remove_row");

	lua_pushcfunction(L,lua_dump_noise_instrument);
	lua_setfield(L,-2,"dump");

	lua_settop(L, 0);
}

void setup_player_metatable(lua_State* L)
{
	luaL_newmetatable(L,lua_wrapper::player_metatable_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-1,"__index");

	lua_pushcfunction(L,lua_open_midi_port);
	lua_setfield(L,-2,"open_midi_port");

	lua_pushcfunction(L,lua_set_voicer);
	lua_setfield(L,-2,"set_voicer");

	lua_pushcfunction(L,lua_set_noise_voicer);
	lua_setfield(L,-2,"set_noise_voicer");

	lua_pushcfunction(L,lua_unset_channel);
	lua_setfield(L,-2,"unset_channel");

	lua_settop(L, 0);
}

void lua_wrapper::setup_lua_env(event_loop* loop)
{
	lua_wrapper::global_loop = loop;

	setup_voicer_metatable(L);
	setup_noise_voicer_metatable(L);

	setup_instrument_metatable(L);
	setup_noise_instrument_metatable(L);

	setup_player_metatable(L);

	lua_pushcfunction(L,lua_open_player);
	lua_setglobal(L,"open_player");

	lua_pushcfunction(L,lua_create_instrument);
	lua_setglobal(L,"create_instrument");

	lua_pushcfunction(L,lua_create_noise_instrument);
	lua_setglobal(L,"create_noise_instrument");

	lua_settop(L, 0);
}

void lua_wrapper::lua_thread(const char* script, event_loop* loop)
{
	lua_wrapper L;

	L.setup_lua_env(loop);

	if(0 != L.open_script(script))
	{
		L.print_error();
		return;
	}

	loop->stop();
}