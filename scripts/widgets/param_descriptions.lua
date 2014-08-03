local format_function = function(options)
	local tab = options.table or {}

	return function(val)
		return tab[val]
	end
end

local num_format = function(val)
	return val
end

local jmp_format = function(val)

	local special_cases ={
		[-1] = '---',
		[-2] = 'freeze'
	}

	return function(val)
		return special_cases[val] or val
	end

end

local amp_format = format_function({
	table = {[0]="0x00","0x01","0x02","0x03","0x04","0x05","0x06","0x07","0x08","0x09","0x0A","0x0B","0x0C","0x0D","0x0E","0x0F"}
})

local wave_format = format_function({
	table = {[0] = "slow", "med_periodic","fast_periodic","sync_periodic","slow_white","med_white","fast_white","sync_white"}
})

local val_getter = function(instr,table,pos)
	return function()
		return instr:get_val(table,pos)
	end
end

local jmp_getter = function(instr,table,pos)
	return function()
		return instr:get_jmp(table,pos)
	end
end

local val_setter = function(instr,table,pos)
	return function(val)
		return instr:set_val(table,pos,val)
	end
end

local jmp_setter = function(instr,table,pos)
	return function(val)
		return instr:set_jmp(table,pos,val)
	end
end

local formatters= {
	amp = amp_format,
	note = num_format,
	freq = num_format,
	noise_amp = amp_format,
	wave = wave_format,
	jump = jmp_format()
}

local module = {
	formatters = formatters,
	val_getter = val_getter,
	jmp_getter = jmp_getter,
	val_setter = val_setter,
	jmp_setter = jmp_setter
}

return module
