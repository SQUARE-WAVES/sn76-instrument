-- Makes a callback cell with the specified forground color.
local cell = function(r,g,b)
	local model ={}
	model.bgcolor_cb = function(mat) return 0,0,0,iup.DEFAULT end
	model.fgcolor_cb = function(mat) return r,g,b,iup.DEFAULT end
	return model
end

---------------------------------------------------------------------------------------------------
-- extends cell with a label, disables editing
local label_cell = function(label)
	local model = cell(0,255,255)

	local keyset = {
		[iup.K_UP] = iup.DEFAULT,
		[iup.K_DOWN] = iup.DEFAULT,
		[iup.K_LEFT] = iup.DEFAULT,
		[iup.K_RIGHT] = iup.DEFAULT
	}

	model.value_cb = function() return label end
	model.newval_cb = function() return false end
	
	model.key_cb = function(key)
		return false,(keyset[key] or iup.IGNORE)
	end
	
	return model
end

---------------------------------------------------------------------------------------------------
--links up with a numerical model, takes a way of getting the value
--a way of setting the value and a formatting function to turn the numerical value into a string
local num_cell = function(getval,setval,format)
	local base_cell = cell(0,255,0)

	base_cell.value_cb = function() return format(getval()) end
	
	base_cell.newval_cb = function(newval)
		local cmp = tonumber(newval)
		if cmp ~= nil then
			setval(cmp)
		end
	end
	
	local key_handlers = {}

	key_handlers[iup.K_mUP] = function()
		base_cell.newval_cb(getval() + 1)
		return true, iup.CONTINUE
	end
	
	key_handlers[iup.K_equal] = key_handlers[iup.K_mUP]
	key_handlers[iup.K_plus] = key_handlers[iup.K_mUP]
	
	key_handlers[iup.K_mDOWN] = function()
		base_cell.newval_cb(getval() - 1)
		return true, iup.CONTINUE
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

---------------------------------------------------------------------------------------------------
--creates an IUP matrix out of a table of cells (as well as user specified number of columns)
local cell_matrix = function(vm)

	local viewmodel = vm
	local rows = #viewmodel.cells
	local cols = viewmodel.cols
	
	local rows_visible = vm.rows_visible or rows
	local cols_visible = vm.cols_visible or cols
	
	local mat = iup.matrix(
	{
		numlin = rows,
		numcol = cols,
		numlin_visible=rows_visible,
		numcol_visible=cols_visible,
		bgcolor = "0 0 0",
		limitexpand="YES",
		tabtitle = title,
		font_cb = function() 
			return "fixedsys, 10" 
		end
	})

	local get_cell = function(lin,col)
		if lin < 1 or lin > rows then
			return false
		elseif col < 1 or col > cols then
			return false
		else
			return viewmodel.cells[lin][col]
		end
	end

	mat.bgcolor_cb = function(mat,lin,col)
		local cell = get_cell(lin,col)
	
		if cell ~= false then
			return cell.bgcolor_cb()
		end
	end
	
	mat.fgcolor_cb = function(mat,lin,col)
		return viewmodel.cells[lin][col].fgcolor_cb()
	end
	
	mat.value_cb = function(mat,lin,col)
		return viewmodel.cells[lin][col].value_cb()
	end
	
	mat.value_edit_cb = function(mat,lin,col,val)
		return viewmodel.cells[lin][col].newval_cb(val)
	end
	
	mat.action_cb = function(mat,key,lin,col,ed,val)
	
		local cb = viewmodel.cells[lin][col].key_cb
		local modded = false
		local code = iup.DEFAULT

		if cb ~= nil then 
			modded,retcode = cb(key)
			code = retcode or code
		end
		
		if modded then
			mat.REDRAW = string.format("%d:%d",lin,col)
		end
		
		return code
	end

	mat.dropcheck_cb = function(mat,lin,col)

		local cb = viewmodel.cells[lin][col].dropcheck_cb

		if cb ~= nil then
			return cb()
		else
			return iup.IGNORE
		end
	end

	mat.drop_cb = function(mat,drop,lin,col)

		local cb = viewmodel.cells[lin][col].drop_cb

		if cb ~= nil then
			return cb(drop)
		else
			return iup.IGNORE
		end
	end

	mat.dropselect_cb = function(mat,lin, col, drop, t, i, v)
		local cell = get_cell(lin,col)
	
		if cell ~= false then
			return cell.dropselect_cb(t,i,v)
		end

		return iup.IGNORE
	end

	mat.update_lines = function(mat)
		local numlin = #viewmodel.cells
		mat.dellin = string.format("0-%d",numlin+1)
		mat.addlin = string.format("0-%d",numlin)
		mat.redraw = "ALL"
	end

	mat.update_model = function(mat,vm)
		viewmodel = vm
		rows = #viewmodel.cells
		cols = viewmodel.cols
		mat:update_lines()
	end
	
	return mat
end

return {
	new = cell_matrix,
	cell = cell,
	num_cell = num_cell,
	label_cell = label_cell
}