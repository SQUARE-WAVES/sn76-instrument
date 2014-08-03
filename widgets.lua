require("iuplua" )
require("iupluacontrols")

-- Makes a callback cell with the specified forground color.
local cell = function(r,g,b)
	local model ={}

	model.bgcolor_cb = function(mat) return 0,0,0,iup.DEFAULT end
	model.fgcolor_cb = function(mat) return r,g,b,iup.DEFAULT end

	return model
end

-- extends cell with a label, no editing
local label_cell = function(label)
	local model = cell(0,255,255)

	model.value_cb = function() return label end
	model.check_cb = function() return false end
	model.newval_cb = function() return false end
	
	return model
end

--links up with a numerical model, takes a minimum, a maximum, a way of getting the value
--a way of setting the value and a formatting function to turn the numerical value into a string
local num_cell = function(min,max,getval,setval,format)
	local base_cell = cell(0,255,0)

	base_cell.value_cb = function() return format(getval()) end
	base_cell.check_cb = function(newval) return (newval >= min and newval<=max) end
	
	base_cell.newval_cb = function(newval)
		local comp = tonumber(newval)
		local old = getval()
		local val
		
		if(comp == nil) then
			return false;
		elseif(comp < min) then 
			val = min 
		elseif(comp > max) then 
			val = max
		else
			val = comp
		end
		
		if(val ~= old) then
			setval(val)
		end
	end
	
	local key_handlers = {}
	key_handlers[iup.K_mUP] = function()
		base_cell.newval_cb(getval() + 1)
		return true
	end
	
	key_handlers[iup.K_equal] = key_handlers[iup.K_mUP]
	key_handlers[iup.K_plus] = key_handlers[iup.K_mUP]
	
	key_handlers[iup.K_mDOWN] = function()
		base_cell.newval_cb(getval() - 1)
		return true
	end
	
	key_handlers[iup.K_minus] = key_handlers[iup.K_mDOWN]
	
	base_cell.key_cb = function(key)
		local handler = key_handlers[key]
		if nil ~= handler then
			return handler()
		else
			return false
		end
	end
	
	return base_cell
end

--creates an IUP matrix out of a table of cells (as well as user specified number of columns)
local callback_table = function(matmod,title)
	
	local cols = matmod.cols
	local rows = #matmod.cells
	
	local mat = iup.matrix(
	{
		numlin=rows,
		numcol=cols,
		bgcolor="0 0 0"
	})
	
	local box = iup.vbox(
	{
		mat,
		tabtitle=title,
		bgcolor="0 0 0",
	})
	
	mat.bgcolor_cb = function(mat,lin,col)
		return matmod.cells[lin][col].bgcolor_cb()
	end
	
	mat.fgcolor_cb = function(mat,lin,col)
		return matmod.cells[lin][col].fgcolor_cb()
	end
	
	mat.value_cb = function(mat,lin,col)
		return matmod.cells[lin][col].value_cb()
	end
	
	mat.value_edit_cb = function(mat,lin,col,val)
		return matmod.cells[lin][col].newval_cb(val)
	end
	
	mat.action_cb = function(mat,key,lin,col,ed,val)
	
		local cb = matmod.cells[lin][col].key_cb
		local modded = false
		if cb ~= nil then 
			modded = cb(key)
		end
		
		if modded then
			mat.REDRAW = "ALL"
			return iup.CONTINUE
		end
		
		return iup.DEFAULT
	end
	
	local ret = {}
	ret.box = box
	ret.matrix = mat
	return ret
end

--The model portion of our thing, creates a module for making cells out of 
--a table for a chip
local param_table = function(vals,chip,channel,headers)

	local jmpformat = (function()
	local tab ={"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15"}
	tab[-2] = "freeze"
	tab[-1] = "--"
	tab[0] = "0"

	return function(val)
		return tab[val]
	end
	
	end)()

	local ampformat = (function()
		local tab = {"0x01","0x02","0x03","0x04","0x05","0x06","0x07","0x08","0x09","0x0A","0x0B","0x0C","0x0D","0x0E","0x0F"}
		tab[0] = "0x00"

		return function(val)
			return tab[val]
		end
		
	end)()

	local noteformat = (function()
		local tab = {}
		for i=-24,24 do
			tab[i] = ''..i
		end

		return function(val)
			return tab[val]
		end
		
	end)()

	local freqformat = (function()
		local tab = {}
		for i=-50,50 do
			tab[i] = ''..i
		end

		return function(val)
			return tab[val]
		end
		
	end)()

	local waveformat = (function()
		local tab = {"med_periodic","fast_periodic","sync_periodic","slow_white","med_white","fast_white","sync_white"}
		tab[0] = "slow"

		return function(val)
			return tab[val]
		end
		
	end)()

	local mod = {}
	local getter = function(y,x,jmp) 
	
		if(jmp ~= nil) then
			return function() 
				return vals[y][x]["jmp"]
			end
		else
			return function()
				return vals[y][x]["val"]
			end
		end
	end
	
	local setter = function(y,x,on_change)
		return function(val)
			vals[y][x]["val"] = val
			on_change(vals[y],y)
		end
	end
	
	local jmpsetter = function(y,x,on_change)
		return function(val)
			vals[y][x]["jmp"] = val
			on_change(vals[y],y)
		end
	end
		
	local ampchange = function(chipnum,chan)
		return function(row,index)
			print(string.format("setting amp! chip: %d channel: %d pos: %d amp: %d jump: %d",chipnum,chan,index-1,row["amp"]["val"],row["amp"]["jmp"]))
			sn76:set_amp(chipnum,chan,index-1,row["amp"]["val"],row["amp"]["jmp"])
		end
	end
	
	local freqchange = function(chipnum,chan)
		return function(row,index)
			print(string.format("setting freq! chip: %d channel: %d pos: %d freq: %d jump: %d",chipnum,chan,index-1,row["freq"]["val"],row["freq"]["jmp"]))
			sn76:set_freq(chipnum,chan,index-1,row["freq"]["val"],row["freq"]["jmp"])
		end
	end
	
	local notechange = function(chipnum,chan)
		return function(row,index)
			print(string.format("setting note! chip: %d channel: %d pos: %d note: %d jump: %d",chipnum,chan,index-1,row["note"]["val"],row["note"]["jmp"]))
			sn76:set_note(chipnum,chan,index-1,row["note"]["val"],row["note"]["jmp"])
		end
	end
	
	local noiseampchange = function(chipnum,chan)
		return function(row,index)
			print(string.format("setting noiseamp! chip: %d channel: %d pos: %d amp: %d jump: %d",chipnum,chan,index-1,row["noiseamp"]["val"],row["noiseamp"]["jmp"]))
			sn76:set_noiseamp(chipnum,chan,index-1,row["noiseamp"]["val"],row["noiseamp"]["jmp"])
		end
	end
	
	local wavechange = function(chipnum,chan)
		return function(row,index)
			print(string.format("setting wave! chip: %d channel: %d pos: %d note: %d jump: %d",chipnum,chan,index-1,row["wave"]["val"],row["wave"]["jmp"]))
			sn76:set_wave(chipnum,chan,index-1,row["wave"]["val"],row["wave"]["jmp"])
		end
	end
	
	local funks =
	{
		amp = ampchange,
		note = notechange,
		freq = freqchange,
		noiseamp = noiseampchange,
		wave = wavechange
	}
	
	local formats =
	{
		amp = ampformat,
		note = noteformat,
		freq = freqformat,
		noiseamp = ampformat,
		wave = waveformat,
		jump = jmpformat
	}
	
	local ranges =
	{
		amp = {0,15},
		note = {-24,24},
		freq = {-50,50},
		noiseamp = {0,15},
		wave = {0,7},
		jump = {-2,15}
	}
	
	mod.vals = vals
	
	for index, row in pairs(mod.vals) do
		print(string.format("setting amp! chip: %d channel: %d pos: %d amp: %d jump: %d",chip,channel,index-1,row["amp"]["val"],row["amp"]["jmp"]))
		sn76:set_amp(chip,channel,index - 1,row["amp"]["val"],row["amp"]["jmp"])
		
		print(string.format("setting freq! chip: %d channel: %d pos: %d freq: %d jump: %d",chip,channel,index-1,row["freq"]["val"],row["freq"]["jmp"]))
		sn76:set_freq(chip,channel,index - 1,row["freq"]["val"],row["freq"]["jmp"])
		
		print(string.format("setting note! chip: %d channel: %d pos: %d note: %d jump: %d",chip,channel,index-1,row["note"]["val"],row["note"]["jmp"]))
		sn76:set_note(chip,channel,index - 1,row["note"]["val"],row["note"]["jmp"])
		
		if(row["noiseamp"] ~= nil) then
			print(string.format("setting noiseamp! chip: %d channel: %d pos: %d amp: %d jump: %d",chip,channel,index-1,row["noiseamp"]["val"],row["noiseamp"]["jmp"]))
			sn76:set_noiseamp(chip,channel,index-1,row["noiseamp"]["val"],row["noiseamp"]["jmp"])
		end
		
		if(row["wave"] ~= nil) then
			print(string.format("setting wave! chip: %d channel: %d pos: %d note: %d jump: %d",chip,channel,index-1,row["wave"]["val"],row["wave"]["jmp"]))
			sn76:set_wave(chip,channel,index-1,row["wave"]["val"],row["wave"]["jmp"])
		end
	
	end

	mod.param_cell = function(param,y)
		return num_cell(ranges[param][1], ranges[param][2], getter(y,param), setter(y,param,funks[param](chip,channel)), formats[param])
	end
	
	mod.jump_cell = function(param,y)
		return num_cell(ranges["jump"][1], ranges["jump"][2], getter(y,param,"jmp"), jmpsetter(y,param,funks[param](chip,channel)), formats["jump"])
	end
	
	return mod
end

local table_view = function(ptab,title,heads)
	local matmod = {}
	matmod.cells = {};
	
	local headers = {label_cell("/")}
	
	if heads ~= nil then
		for i,v in ipairs(heads) do
			table.insert(headers,label_cell(v))
			table.insert(headers,label_cell("jump"))
		end
		
		table.insert(matmod.cells,headers)
		matmod.cols = #headers
		
		for index,value in ipairs(ptab.vals) do
			local thisrow = {label_cell(index)}
			
			for i,v in ipairs(heads) do
				table.insert(thisrow,ptab.param_cell(v,index))
				table.insert(thisrow,ptab.jump_cell(v,index))
			end
			
			table.insert(matmod.cells,thisrow)
		end
	
	else
	
		for k,v in pairs(ptab.vals[1]) do
			table.insert(headers,label_cell(k))
			table.insert(headers,label_cell("jump"))
		end

		table.insert(matmod.cells,headers)
		
		matmod.cols = #headers
	
	
		for index,value in ipairs(ptab.vals) do
			local thisrow = {label_cell(index)}
			for k,v in pairs(value) do
				table.insert(thisrow,ptab.param_cell(k,index))
				table.insert(thisrow,ptab.jump_cell(k,index))
			end
			table.insert(matmod.cells,thisrow)
		end
	end
	
	return callback_table(matmod,title)
end

local exports = {}
exports.ptab = param_table
exports.matrix = table_view

return exports