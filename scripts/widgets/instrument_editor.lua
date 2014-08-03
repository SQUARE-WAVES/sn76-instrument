local params = require("param_descriptions")
local cell_matrix = require("cell_matrix")

local label_cell = cell_matrix.label_cell
local num_cell = cell_matrix.num_cell

local create_viewmodel = function(instrument)
	
	local label_cells = function(labels)

		local cells = {label_cell('/')}
		
		for i,v in ipairs(labels) do
			table.insert(cells,label_cell(v))
			table.insert(cells,label_cell(v..' jump'))
		end

		return cells
	end

	local numeric_row = function(labels,count)
		local cells = {label_cell(count-1)}

		for i,v in ipairs(labels) do
			local tabnum = i -1;
			local pos = count - 1

			local valset = params.val_setter(instrument,tabnum,pos)
			local jmpset = params.jmp_setter(instrument,tabnum,pos)
			local valget = params.val_getter(instrument,tabnum,pos)
			local jmpget = params.jmp_getter(instrument,tabnum,pos)

			table.insert(cells, num_cell(valget,valset,params.formatters[v]))
			table.insert(cells, num_cell(jmpget,jmpset,params.formatters.jump))
		end

		return cells
	end

	local rows,labels = instrument:get_details()
	local cols = #labels

	--ok get the label cells;
	local viewmodel = {}
	viewmodel.cols = cols*2 + 1
	
	viewmodel.cells = {label_cells(labels)}

	for i=1,rows do
		table.insert(viewmodel.cells,numeric_row(labels,i))
	end

	viewmodel.rows_visible = 24

	return viewmodel
end

local instrument_editor = function(instr,tabs)

	local name = instr.name
	local instrument = instr.handle

	local viewmodel = create_viewmodel(instrument)
	local mat = cell_matrix.new(viewmodel,instrument_name);

	local add_row = function()
		instrument:add_row()
		viewmodel = create_viewmodel(instrument)
		mat:update_model(viewmodel)
	end

	local remove_row = function()
		instrument:remove_row()
		viewmodel = create_viewmodel(instrument)
		mat:update_model(viewmodel)
	end

	local add_btn = iup.button{title='add row',bgcolor='0 0 0',fgcolor='0 0 0'}
	add_btn.action = add_row

	local delete_btn = iup.button{title='delete row',bgcolor='0 0 0',fgcolor='0 0 0'}
	delete_btn.action = remove_row

	local close_button = iup.button{title='close'}

	local rename_text = iup.text{expand="vertical",size="80x"}
	local rename_button = iup.button{title='rename',bgcolor='0 0 0',fgcolor='0 0 0'}
	local titlebar = iup.label{font='fixedsys, Bold 36',title=name, expand="horizontal",alignment="ACENTER:ATOP", fgcolor="0 255 255"}

	local ed = iup.vbox{
		iup.frame{
			iup.vbox{
				titlebar,
				mat,
				alignment="ACENTER"
			},

			title=nil,
			bgcolor = "0 0 0"
		},

		iup.hbox{
			iup.hbox{
				add_btn,
				delete_btn
			},

			iup.hbox{
				rename_button,
				rename_text,
			},

			iup.hbox{
				close_button,
				expand="horizontal"
			},
			

			alignment="ALEFT"
		},

		alignment="ALEFT",
		meta={
			tabname=name
		}
	}

	local rename = function(newname)
		titlebar.title = newname
		ed.meta.tabname = newname
		instr.name = newname
		tabs.refresh_names()
	end

	rename_button.action = function()
		local text = rename_text.value
		if("" ~= text) then
			rename(text)
		end
	end

	close_button.action = function()
		tabs.remove_tab(ed)
	end
	
	return ed
end

return instrument_editor