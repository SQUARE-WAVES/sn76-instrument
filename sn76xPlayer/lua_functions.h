#ifndef LUA_FUNCTIONS_DOT_H
#define LUA_FUNCTIONS_DOT_H

#pragma region STRUCTS
struct lua_interface_data
{
	event_loop* loop;
	sn76_player* player;
};

//lua metatable functions and messages
class table_entry_message: public event
{
	public:
		chip_instrument* instrument;
		int pos;
		int val;
		int jump;
		int table;

		void handle(int iobytes)
		{
			instrument->set_table(table,pos,val,jump);	
		}
};

class add_row_message: public event
{
	public:
		chip_instrument* instrument;
		int index;

		void handle(int iobytes)
		{
			instrument->add_row(index);
		}
};

class remove_row_message: public event
{
	public:
		chip_instrument* instrument;
		int index;

		void handle(int iobytes)
		{
			instrument->remove_row(index);
		}
};

class change_instrument_message: public event
{
	public:
		chip_instrument* instrument;
		chip_voicer* voicer;

		void handle(int iobytes)
		{
			voicer->set_instrument(instrument);
		}
};

class insert_patch_message: public event
{
	public:
		chip_instrument* instr;
		chip_voicer* voicer;
		int patch_number;

		void handle(int iobytes)
		{
			voicer->set_patch(instr,patch_number);
		}
};

#pragma endregion

//--------------------------------
//instrument functions
int lua_create_instrument(lua_State* L);
int lua_collect_instrument(lua_State* L);

int lua_create_noise_instrument(lua_State* L);
int lua_collect_noise_instrument(lua_State* L);

int lua_set_instrument(lua_State* L);
int lua_set_noise_instrument(lua_State* L);

int lua_get_instrument_details(lua_State* L);
int lua_get_noise_instrument_details(lua_State* L);

int lua_set_instrument_val(lua_State* L);
int lua_set_instrument_jmp(lua_State* L);

int lua_set_noise_instrument_val(lua_State* L);
int lua_set_noise_instrument_jmp(lua_State* L);

int lua_add_instrument_row(lua_State* L);
int lua_add_noise_instrument_row(lua_State* L);

int lua_remove_instrument_row(lua_State* L);
int lua_remove_noise_instrument_row(lua_State* L);

int lua_get_instrument_val(lua_State* L);
int lua_get_instrument_jmp(lua_State* L);

int lua_get_noise_instrument_val(lua_State* L);
int lua_get_noise_instrument_jmp(lua_State* L);

int lua_dump_regular_instrument(lua_State* L);
int lua_dump_noise_instrument(lua_State* L);

//--other stuff
int lua_unset_pach(lua_State* L);

int lua_set_patch(lua_State* L);
int lua_set_noise_patch(lua_State* L);

int lua_open_midi_port(lua_State* L);
int lua_set_voicer(lua_State* L);
int lua_set_noise_voicer(lua_State* L);
int lua_unset_channel(lua_State* L);

int lua_open_player(lua_State* L);

#endif