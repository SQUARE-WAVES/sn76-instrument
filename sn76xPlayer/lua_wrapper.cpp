#include "lua_wrapper.h"
#include <iostream>
#include <vector>


struct lua_interface_data
{
	event_loop* loop;
	sn76_player* player;
};

//lua metatable functions and messages
class table_entry_message: public event
{
	public:
		sn76_player* player;
		int chip;
		int channel;
		int pos;
		int val;
		int jump;
		int table;

		void handle(int iobytes)
		{
			sn76_voicer* voicer = player->get_voicer(chip,channel);
			voicer->set_table(table,pos,val,jump);	
		}
};

const char lua_wrapper::interface_meta_name[] = "LOOPMETA";

int lua_set_amp(lua_State* L)
{
	lua_interface_data* ud;
	int chip;
	int channel;
	int pos;
	uint8_t val;
	int jump;

	ud = (lua_interface_data*)luaL_checkudata(L,1,lua_wrapper::interface_meta_name);
	chip = luaL_checkint(L,2);
	channel = luaL_checkint(L,3);
	pos = luaL_checkint(L,4);
	val = luaL_checkint(L,5);
	jump = luaL_checkint(L,6);

	table_entry_message* msg = new table_entry_message();
	msg->table = 0;
	msg->player = ud->player;
	msg->chip = chip;
	msg->channel = channel;
	msg->pos = pos;
	msg->val = val;
	msg->jump = jump;

	ud->loop->post_message(msg);

	return 0;
}

int lua_set_note(lua_State* L)
{
	lua_interface_data* ud;
	int chip;
	int channel;
	int pos;
	int val;
	int jump;

	ud = (lua_interface_data*)luaL_checkudata(L,1,lua_wrapper::interface_meta_name);
	chip = luaL_checkint(L,2);
	channel = luaL_checkint(L,3);
	pos = luaL_checkint(L,4);
	val = luaL_checkint(L,5);
	jump = luaL_checkint(L,6);

	table_entry_message* msg = new table_entry_message();
	msg->table = 1;
	msg->player = ud->player;
	msg->chip = chip;
	msg->channel = channel;
	msg->pos = pos;
	msg->val = val;
	msg->jump = jump;

	ud->loop->post_message(msg);

	return 0;
}

int lua_set_freq(lua_State* L)
{
	lua_interface_data* ud;
	int chip;
	int channel;
	int pos;
	int val;
	int jump;

	ud = (lua_interface_data*)luaL_checkudata(L,1,lua_wrapper::interface_meta_name);
	chip = luaL_checkint(L,2);
	channel = luaL_checkint(L,3);
	pos = luaL_checkint(L,4);
	val = luaL_checkint(L,5);
	jump = luaL_checkint(L,6);

	table_entry_message* msg = new table_entry_message();
	msg->table = 2;
	msg->player = ud->player;
	msg->chip = chip;
	msg->channel = channel;
	msg->pos = pos;
	msg->val = val;
	msg->jump = jump;
		
	ud->loop->post_message(msg);

	return 0;
}

int lua_set_noiseamp(lua_State* L)
{
	lua_interface_data* ud;
	int chip;
	int channel;
	int pos;
	uint8_t val;
	int jump;

	ud = (lua_interface_data*)luaL_checkudata(L,1,lua_wrapper::interface_meta_name);
	chip = luaL_checkint(L,2);
	channel = luaL_checkint(L,3);
	pos = luaL_checkint(L,4);
	val = luaL_checkint(L,5);
	jump = luaL_checkint(L,6);

	table_entry_message* msg = new table_entry_message();
	msg->table = 3;
	msg->player = ud->player;
	msg->chip = chip;
	msg->channel = channel;
	msg->pos = pos;
	msg->val = val;
	msg->jump = jump;
		
	ud->loop->post_message(msg);

	return 0;
}

int lua_set_wave(lua_State* L)
{
	lua_interface_data* ud;
	int chip;
	int channel;
	int pos;
	uint8_t val;
	int jump;

	ud = (lua_interface_data*)luaL_checkudata(L,1,lua_wrapper::interface_meta_name);
	chip = luaL_checkint(L,2);
	channel = luaL_checkint(L,3);
	pos = luaL_checkint(L,4);
	val = luaL_checkint(L,5);
	jump = luaL_checkint(L,6);

	table_entry_message* msg = new table_entry_message();
	msg->table = 4;
	msg->player = ud->player;
	msg->chip = chip;
	msg->channel = channel;
	msg->pos = pos;
	msg->val = val;
	msg->jump = jump;
		
	ud->loop->post_message(msg);

	return 0;
}

int lua_open_player(lua_State* L)
{
	lua_interface_data* ud = (lua_interface_data*)luaL_checkudata(L,1,lua_wrapper::interface_meta_name);
	int midi_port = luaL_checkint(L,2);
	const char* com_port = luaL_checkstring(L,3);

	if(ud->player != NULL)
	{
		lua_pushnil(L);
		lua_pushstring(L,"sn76Player is allready open");

		return 2;
	}

	serial_port* the_port = new serial_port(ud->loop,com_port);
	sn76core* the_core = new sn76core(the_port);
	ud->player = new sn76_player(ud->loop,the_core,midi_port);

	luaL_getmetatable(L,lua_wrapper::interface_meta_name);

	//now establish the real metamethods
	lua_pushcfunction(L,lua_set_amp);
	lua_setfield(L,-2,"set_amp");
	
	lua_pushcfunction(L,lua_set_note);
	lua_setfield(L,-2,"set_note");

	lua_pushcfunction(L,lua_set_freq);
	lua_setfield(L,-2,"set_freq");

	lua_pushcfunction(L,lua_set_noiseamp);
	lua_setfield(L,-2,"set_noiseamp");

	lua_pushcfunction(L,lua_set_wave);
	lua_setfield(L,-2,"set_wave");
	//pop the metatable
	lua_pop(L,1);

	lua_pushboolean(L,true);
	return 2;
}

lua_wrapper::lua_wrapper():
interface_registered(false)
{
	L = lua_open();
	luaL_openlibs(L);	
	
	//set up the path
	luaL_dostring(L,"package.path = package.path .. ';scripts/?.lua'");
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

void lua_wrapper::register_interface(const char* name,event_loop* loop)
{
	if(interface_registered)
	{
		throw "can't have more than one interface!";
	}

	lua_interface_data* ud = (lua_interface_data*)lua_newuserdata(L,sizeof(lua_interface_data));
	ud->loop = loop;
	ud->player = NULL;
	
	luaL_newmetatable(L,interface_meta_name);
	lua_pushvalue(L,-1);
	lua_setfield(L,-1,"__index");

	lua_pushcfunction(L,lua_open_player);
	lua_setfield(L,-2,"open");

	lua_setmetatable(L,-2);
	lua_setglobal(L,name);

	interface_registered = true;
}

void lua_wrapper::lua_thread(const char* script, event_loop* loop)
{
	lua_wrapper L;

	L.register_interface("sn76",loop);

	if(0 != L.open_script(script))
	{
		L.print_error();
		return;
	}

	loop->stop();
}