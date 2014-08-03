require("json")

local SN76_NOISE = 'sn76_noise'
local SN76_SQR = 'sn76_square'

local instrument_constructors = {
	[SN76_NOISE] = create_noise_instrument,
	[SN76_SQR] = create_instrument
}

local instr = function(name,handle,type)
	return{
		name = name,
		handle = handle,
		type = type
	}
end

local dump_instr = function(instr)
	local z= {
		name=instr.name,
		type=instr.type,
		data = nil,
		rows = nil
	}

	z.data,z.rows = instr.handle:dump()

	require('pl.pretty').write(z.data)

	return z
end

local rebuild_instr = function(dump)
	local handle = instrument_constructors[dump.type](dump.rows)

	for index,col in ipairs(dump.data) do
		
		local col_offset = index - 1

		for i=1,dump.rows do
			local row_offset = i-1
			
			local val = col.val[i]
			local jmp = col.jump[i]

			handle:set_val(col_offset,row_offset,val)
			handle:set_jmp(col_offset,row_offset,jmp)
		end
	end

	return instr(dump.name,handle,dump.type)
end

local ilmeta = {

	add = function(self,instr)
		table.insert(self.list,instr)
		self.set[instr.handle] = instr
	end,

	new = function(self,name,size)
		local handle = self.create_instrument(size)
		local instrument = instr(name,handle,self.instr_type)

		self:add(instrument)
		return instrument
	end,

	len = function(self)
		return #self.list
	end,

	at = function(self,index)
		return self.list[index]
	end,

	by_handle = function(self,handle)
		return self.set[handle]
	end,

	contains = function(self,instrument)
		return self.set[handle] ~= nil
	end,

	save = function(self,path)
		local serialized = {}
		print(#self.list)
		for i,v in ipairs(self.list) do
			serialized[i] = dump_instr(v)
		end

		print(require('pl.pretty').write(serialized))

		local sf = io.open(path,"w")
		sf:write(json.encode(serialized))
		sf:close()

	end,

	load = function(self,path)
		local sf = io.open(path,"r")
		local serialized = json.decode(sf:read("*a"))

		for i,v in ipairs(serialized) do
			self:add(rebuild_instr(v))
		end
	end
}

ilmeta.__index = ilmeta

local instrument_list = function(create_function,instr_type)
	local ilist = {}
	ilist.list = {}
	ilist.set = {}

	ilist.create_instrument = create_function
	ilist.instr_type = instr_type

	setmetatable(ilist,ilmeta)

	return ilist
end

return {
	noise = instrument_list(create_noise_instrument,SN76_NOISE),
	square = instrument_list(create_instrument,SN76_SQR)
}
