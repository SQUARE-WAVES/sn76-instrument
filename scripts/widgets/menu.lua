require('iuplua')
require("iupluacontrols")

local instrument_editor = require('widgets.instrument_editor')

local instruments = require('instruments')

local quit_it = function()
	return iup.CLOSE
end

local load_tbl = function(instr_table)
	return function()
		local path,status = iup.GetFile('./*.json')
		
		if(status == 1 or status == -1) then
			return
		else
			instr_table:load(path)
		end
	end
end

local save_tbl = function(instr_table)
	return function()
		local path,status = iup.GetFile('./*.json')

		if(status == -1) then
			return
		else
			instr_table:save(path)
		end
	end
end

local edit_instrument = function(instr_table,tab_interface)
	return function()
		local strings = {}
		local marks = {}

		for i,v in ipairs(instr_table.list) do
			marks[i] = 0
			strings[i] = v.name
		end

		if(#strings > 0) then
			iup.ListDialog(2,'select instruments',#strings, strings, 1, 8,24,marks)
		else
			iup.Popup(iup.messagedlg{
				tile="you got nothing",
				value="no instruments to edit",
				buttons="OK"
			})
		end
		
		for i,v in ipairs(marks) do
			if(v == 1) then
				local instr = instr_table:at(i)
				local ed = instrument_editor(instr,tab_interface)
				tab_interface.add_tab(ed)
			end
		end
	end
end

local make_menu = function(tab_interface,instruments)
	local the_menu = iup.menu(
	{
		iup.submenu{
			title='file',
			iup.menu{
				iup.item{title='save instruments',action=save_tbl(instruments.square)},
				iup.item{title='save noise instruments',action=save_tbl(instruments.noise)},
				iup.separator{},
				iup.item{title='load instruments',action=load_tbl(instruments.square)},
				iup.item{title='load noise instruments',action=load_tbl(instruments.noise)},
				iup.separator{},
				iup.item{title='exit',action=quit_it}
			}
		},
		iup.submenu{
			title='edit',
			iup.menu{
				iup.item{title='edit instrument(s)',action=edit_instrument(instruments.square,tab_interface)},
				iup.item{title='edit noise instrument(s)',action=edit_instrument(instruments.noise,tab_interface)}
			}
		}
	})


	return the_menu
end

return make_menu
