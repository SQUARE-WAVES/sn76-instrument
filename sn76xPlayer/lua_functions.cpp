#include "lua_wrapper.h"
#include "lua_functions.h"
#include "neo_player.h"

//-------------------------------------------------------------------------------------------------
//	in this program the core and player will never be collected once created, so we don't have to
//	worry about collecting all their parts
//-------------------------------------------------------------------------------------------------

int lua_create_instrument(lua_State* L)
{
	int size = luaL_checkint(L,1);

	sn76_instrument** newinstr = (sn76_instrument**)lua_newuserdata(L,sizeof(sn76_instrument*));
	*newinstr = new sn76_instrument(size);

	luaL_getmetatable(L,lua_wrapper::instrument_meta_name);
	lua_setmetatable(L, -2);

	return 1;
}

int lua_create_noise_instrument(lua_State* L)
{
	int size = luaL_checkint(L,1);

	sn76_noise_instrument** newinstr = (sn76_noise_instrument**)lua_newuserdata(L,sizeof(sn76_noise_instrument*));
	*newinstr = new sn76_noise_instrument(size);

	luaL_getmetatable(L,lua_wrapper::noise_instrument_meta_name);
	lua_setmetatable(L, -2);

	return 1;
}

int lua_collect_instrument(lua_State* L)
{
	sn76_instrument* ud = (sn76_instrument*)luaL_checkudata(L,1,lua_wrapper::instrument_meta_name);
	delete ud;
	return 0;
}

int lua_collect_noise_instrument(lua_State* L)
{
	sn76_noise_instrument* ud = (sn76_noise_instrument*)luaL_checkudata(L,1,lua_wrapper::noise_instrument_meta_name);
	delete ud;
	return 0;
}

int lua_set_instrument(lua_State* L)
{
	chip_voicer* vc =*( (chip_voicer**)luaL_checkudata(L,1,lua_wrapper::voicer_meta_name));
	sn76_instrument* instr =*( (sn76_instrument**)luaL_checkudata(L,2,lua_wrapper::instrument_meta_name));

	change_instrument_message* msg = new change_instrument_message();
	msg->voicer = vc;
	msg->instrument = instr;

	lua_wrapper::global_loop->post_message(msg);

	return 0;
}

int lua_set_noise_instrument(lua_State* L)
{
	chip_voicer* vc =*( (chip_voicer**)luaL_checkudata(L,1,lua_wrapper::noise_voicer_meta_name));
	sn76_noise_instrument* instr =*((sn76_noise_instrument**)luaL_checkudata(L,2,lua_wrapper::noise_instrument_meta_name));

	change_instrument_message* msg = new change_instrument_message();
	msg->voicer = vc;
	msg->instrument = instr;

	lua_wrapper::global_loop->post_message(msg);

	return 0;
}

int lua_unset_pach(lua_State* L)
{
	chip_voicer* vc =*( (chip_voicer**)luaL_checkudata(L,1,lua_wrapper::voicer_meta_name));
	int patchnum = luaL_checkinteger(L,2);

	vc->set_patch(NULL,patchnum);

	return 0;
}

int lua_unset_noise_patch(lua_State* L)
{
	chip_voicer* vc =*( (chip_voicer**)luaL_checkudata(L,1,lua_wrapper::noise_voicer_meta_name));
	int patchnum = luaL_checkinteger(L,2);

	vc->set_patch(NULL,patchnum);

	return 0;
}

int lua_set_patch(lua_State* L)
{
	chip_voicer* vc =*( (chip_voicer**)luaL_checkudata(L,1,lua_wrapper::voicer_meta_name));
	sn76_instrument* instr =*((sn76_instrument**)luaL_checkudata(L,2,lua_wrapper::instrument_meta_name));
	int patchnum = luaL_checkinteger(L,3);

	vc->set_patch(instr,patchnum);

	return 0;
}

int lua_set_noise_patch(lua_State* L)
{
	chip_voicer* vc =*( (chip_voicer**)luaL_checkudata(L,1,lua_wrapper::noise_voicer_meta_name));
	sn76_noise_instrument* instr =*((sn76_noise_instrument**)luaL_checkudata(L,2,lua_wrapper::noise_instrument_meta_name));
	int patchnum = luaL_checkinteger(L,3);

	vc->set_patch(instr,patchnum);

	return 0;
}

static int get_instrument_details(chip_instrument* instr,lua_State*L)
{
	int rows = instr->get_rows();
	int cols = instr->get_cols();
	const char** labels = instr->get_labels();

	lua_pushinteger(L,rows);
	lua_newtable(L);

	for(int i=0; i < cols; ++i)
	{
		lua_pushstring(L,labels[i]);
		lua_rawseti(L,-2,i+1);
	}

	return 2;
}

int lua_get_instrument_details(lua_State* L)
{
	sn76_instrument** ud = (sn76_instrument**)luaL_checkudata(L,1,lua_wrapper::instrument_meta_name);
	sn76_instrument* instr = *ud;

	int z = instr->get_cols();
	return get_instrument_details(instr,L);
}

int lua_get_noise_instrument_details(lua_State* L)
{
	sn76_noise_instrument** ud = (sn76_noise_instrument**)luaL_checkudata(L,1,lua_wrapper::noise_instrument_meta_name);
	sn76_noise_instrument* instr = *ud;

	return get_instrument_details(instr,L);
}

static int set_tab_val(chip_instrument* instr,int table,int pos,int val,lua_State* L)
{
	try{
		instr->set_table_val(table,pos,val);
	}
	catch(const char* err)
	{
		lua_pushnil(L);
		lua_pushstring(L,err);
		return 2;
	}

	int newval = instr->get_table_val(table,pos);
	lua_pushinteger(L,newval);
	return 1;
}

static int set_tab_jmp(chip_instrument* instr,int table,int pos,int jmp,lua_State* L)
{
	try{
		instr->set_table_jmp(table,pos,jmp);
	}
	catch(const char* err)
	{
		lua_pushnil(L);
		lua_pushstring(L,err);
		return 2;
	}

	int newval = instr->get_table_jmp(table,pos);
	lua_pushinteger(L,newval);
	return 1;
}

static int lua_set_tab_val(const char* metaname, lua_State* L)
{
	chip_instrument* instr;
	int table;
	int pos;
	int val;

	instr =*( (chip_instrument**)luaL_checkudata(L,1,metaname));
	table = luaL_checkint(L,2);
	pos = luaL_checkint(L,3);
	val = luaL_checkint(L,4);

	return set_tab_val(instr,table,pos,val,L);
}

static int lua_set_tab_jmp(const char* metaname,lua_State* L)
{
	chip_instrument* instr;
	int table;
	int pos;
	int jmp;

	instr =*( (chip_instrument**)luaL_checkudata(L,1,metaname));
	table = luaL_checkint(L,2);
	pos = luaL_checkint(L,3);
	jmp = luaL_checkint(L,4);

	return set_tab_jmp(instr,table,pos,jmp,L);
}

int lua_set_instrument_val(lua_State*L)
{
	return lua_set_tab_val(lua_wrapper::instrument_meta_name,L);
}

int lua_set_instrument_jmp(lua_State*L)
{
	return lua_set_tab_jmp(lua_wrapper::instrument_meta_name,L);
}

int lua_set_noise_instrument_val(lua_State*L)
{
	return lua_set_tab_val(lua_wrapper::noise_instrument_meta_name,L);
}

int lua_set_noise_instrument_jmp(lua_State*L)
{
	return lua_set_tab_jmp(lua_wrapper::noise_instrument_meta_name,L);
}

static int get_tab_val(chip_instrument* instr,int table,int pos,lua_State* L)
{
	try
	{
		int val = instr->get_table_val(table,pos);
		lua_pushinteger(L,val);
		return 1;
	}
	catch(const char* err)
	{
		lua_pushnil(L);
		lua_pushstring(L,err);
		return 2;
	}
}

static int get_tab_jmp(chip_instrument* instr,int table,int pos,lua_State* L)
{
	try
	{
		int jmp = instr->get_table_jmp(table,pos);
		lua_pushinteger(L,jmp);
		return 1;
	}
	catch(const char* err)
	{
		lua_pushnil(L);
		lua_pushstring(L,err);
		return 2;
	}
}

static int lua_get_tab_val(const char* metaname, lua_State* L)
{
	chip_instrument* instr;
	int table;
	int pos;

	instr =*( (chip_instrument**)luaL_checkudata(L,1,metaname));
	table = luaL_checkint(L,2);
	pos = luaL_checkint(L,3);

	return get_tab_val(instr,table,pos,L);
}

static int lua_get_tab_jmp(const char* metaname,lua_State* L)
{
	chip_instrument* instr;
	int table;
	int pos;

	instr =*( (chip_instrument**)luaL_checkudata(L,1,metaname));
	table = luaL_checkint(L,2);
	pos = luaL_checkint(L,3);

	return get_tab_jmp(instr,table,pos,L);
}

int lua_get_instrument_val(lua_State*L)
{
	return lua_get_tab_val(lua_wrapper::instrument_meta_name,L);
}

int lua_get_instrument_jmp(lua_State*L)
{
	return lua_get_tab_jmp(lua_wrapper::instrument_meta_name,L);
}

int lua_get_noise_instrument_val(lua_State*L)
{
	return lua_get_tab_val(lua_wrapper::noise_instrument_meta_name,L);
}

int lua_get_noise_instrument_jmp(lua_State*L)
{
	return lua_get_tab_jmp(lua_wrapper::noise_instrument_meta_name,L);
}

int lua_add_instrument_row(lua_State* L)
{
	sn76_instrument** ud = (sn76_instrument**)luaL_checkudata(L,1,lua_wrapper::instrument_meta_name);
	sn76_instrument* instr = *ud;

	int len = instr->get_rows();
	instr->add_row(len);

	lua_pushinteger(L,instr->get_rows());

	return 1;
}

int lua_add_noise_instrument_row(lua_State* L)
{
	sn76_noise_instrument** ud = (sn76_noise_instrument**)luaL_checkudata(L,1,lua_wrapper::noise_instrument_meta_name);
	sn76_noise_instrument* instr = *ud;

	int len = instr->get_rows();
	instr->add_row(len);

	lua_pushinteger(L,instr->get_rows());

	return 1;
}

int lua_remove_instrument_row(lua_State* L)
{
	sn76_instrument** ud = (sn76_instrument**)luaL_checkudata(L,1,lua_wrapper::instrument_meta_name);
	sn76_instrument* instr = *ud;

	int len = instr->get_rows();
	instr->remove_row(len);

	lua_pushinteger(L,instr->get_rows());

	return 1;
}

int lua_remove_noise_instrument_row(lua_State* L)
{
	sn76_noise_instrument** ud = (sn76_noise_instrument**)luaL_checkudata(L,1,lua_wrapper::noise_instrument_meta_name);
	sn76_noise_instrument* instr = *ud;

	int len = instr->get_rows();
	instr->remove_row(len);

	lua_pushinteger(L,instr->get_rows());

	return 1;
}

int lua_open_midi_port(lua_State* L)
{
	neo_player** ud = (neo_player**)luaL_checkudata(L,1,lua_wrapper::player_metatable_name);
	neo_player* player = *ud;

	int port_number = luaL_checkinteger(L,2);

	//first off we have to get the global loop
	event_loop* loop = lua_wrapper::global_loop;

	bool worked = player->open_midi_port(port_number,loop);

	lua_pushboolean(L,worked);
	return 1;
}

int lua_set_voicer(lua_State* L)
{
	neo_player** ud = (neo_player**)luaL_checkudata(L,1,lua_wrapper::player_metatable_name);
	neo_player* player = *ud;

	chip_voicer* vc =*( (chip_voicer**)luaL_checkudata(L,2,lua_wrapper::voicer_meta_name));

	int channel = luaL_checkinteger(L,3);

	bool worked = player->set_voicer(channel,vc);

	lua_pushboolean(L,worked);
	return 1;
}

int lua_set_noise_voicer(lua_State* L)
{
	neo_player** ud = (neo_player**)luaL_checkudata(L,1,lua_wrapper::player_metatable_name);
	neo_player* player = *ud;

	chip_voicer* vc =*( (chip_voicer**)luaL_checkudata(L,2,lua_wrapper::noise_voicer_meta_name));

	int channel = luaL_checkinteger(L,3);
	bool worked = player->set_voicer(channel,vc);

	lua_pushboolean(L,worked);
	return 1;
}

int lua_unset_channel(lua_State* L)
{
	neo_player** ud = (neo_player**)luaL_checkudata(L,1,lua_wrapper::player_metatable_name);
	neo_player* player = *ud;

	int channel = luaL_checkinteger(L,3);

	bool worked = player->unset_voicer(channel);

	lua_pushboolean(L,worked);
	return 1;
}

int lua_open_player(lua_State* L)
{
	event_loop* loop = lua_wrapper::global_loop;
	const char* com_port = luaL_checkstring(L,1);

	//first we need to create the core
	serial_port* port = new serial_port(loop, com_port);
	sn76_core* core = new sn76_core(port);

	//ok we need to create the voicers, then the player
	sn76_voicer* chip1_ch1 = new sn76_voicer(core,0,0);
	sn76_voicer* chip1_ch2 = new sn76_voicer(core,0,1);
	sn76_noise_voicer* chip1_nz = new sn76_noise_voicer(core,0);

	sn76_voicer* chip2_ch1 = new sn76_voicer(core,1,0);
	sn76_voicer* chip2_ch2 = new sn76_voicer(core,1,1);
	sn76_noise_voicer* chip2_nz = new sn76_noise_voicer(core,1);

	neo_player* player = new neo_player();

	//now we need to make UD for the voicers and send them on their way
	neo_player** player_ud = (neo_player**)lua_newuserdata(L,sizeof(neo_player*));
	*player_ud = player;
	luaL_getmetatable(L,lua_wrapper::player_metatable_name);
	lua_setmetatable(L,-2);

	sn76_voicer** chip1_ch1_ud = (sn76_voicer**)lua_newuserdata(L,sizeof(sn76_voicer*));
	*chip1_ch1_ud = chip1_ch1;
	luaL_getmetatable(L,lua_wrapper::voicer_meta_name);
	lua_setmetatable(L, -2);

	sn76_voicer** chip1_ch2_ud = (sn76_voicer**)lua_newuserdata(L,sizeof(sn76_voicer*));
	*chip1_ch2_ud = chip1_ch2;
	luaL_getmetatable(L,lua_wrapper::voicer_meta_name);
	lua_setmetatable(L, -2);

	sn76_noise_voicer** chip1_nz_ud = (sn76_noise_voicer**)lua_newuserdata(L,sizeof(sn76_noise_voicer*));
	*chip1_nz_ud = chip1_nz;
	luaL_getmetatable(L,lua_wrapper::noise_voicer_meta_name);
	lua_setmetatable(L, -2);

	sn76_voicer** chip2_ch1_ud = (sn76_voicer**)lua_newuserdata(L,sizeof(sn76_voicer*));
	*chip2_ch1_ud = chip2_ch1;
	luaL_getmetatable(L,lua_wrapper::voicer_meta_name);
	lua_setmetatable(L, -2);

	sn76_voicer** chip2_ch2_ud = (sn76_voicer**)lua_newuserdata(L,sizeof(sn76_voicer*));
	*chip2_ch2_ud = chip2_ch2;
	luaL_getmetatable(L,lua_wrapper::voicer_meta_name);
	lua_setmetatable(L, -2);

	sn76_noise_voicer** chip2_nz_ud = (sn76_noise_voicer**)lua_newuserdata(L,sizeof(sn76_noise_voicer*));
	*chip2_nz_ud = chip2_nz;
	luaL_getmetatable(L,lua_wrapper::noise_voicer_meta_name);
	lua_setmetatable(L, -2);

	return 7;
}

int lua_dump_instrument(chip_instrument* instr,lua_State* L)
{
	const char** labels = instr->get_labels();
	int cols = instr->get_cols();
	int rows = instr->get_rows();

	lua_createtable(L,cols,1); //-5

	for(int col = 0;col<cols;++col)
	{
		lua_createtable(L,0,3); //the big one -4
		const char* label = labels[col];
		lua_pushstring(L,label);
		lua_setfield(L,-2,"label");

		lua_createtable(L,rows,0);//val -3
		lua_createtable(L,rows,0);//jmp -2

		for(int row=0;row<rows;++row)
		{
			int jmp = instr->get_table_jmp(col,row);
			int val = instr->get_table_val(col,row);

			lua_pushinteger(L,val);
			lua_rawseti(L,-3,row+1);
			lua_pushinteger(L,jmp);
			lua_rawseti(L,-2,row+1);
		}
		
		//jmps on top
		lua_setfield(L,-3,"jump");
		lua_setfield(L,-2,"val");
		lua_rawseti(L,-2,col+1);

	}
	
	lua_pushinteger(L,rows);
	return 2;
}

int lua_dump_regular_instrument(lua_State* L)
{
	chip_instrument** ud = (chip_instrument**)luaL_checkudata(L,1,lua_wrapper::instrument_meta_name);
	chip_instrument* instr = *ud;

	return lua_dump_instrument(instr,L);
}

int lua_dump_noise_instrument(lua_State* L)
{
	chip_instrument** ud = (chip_instrument**)luaL_checkudata(L,1,lua_wrapper::noise_instrument_meta_name);
	chip_instrument* instr = *ud;

	return lua_dump_instrument(instr,L);
}