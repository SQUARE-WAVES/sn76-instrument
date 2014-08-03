require("iuplua")
require("iupluacontrols")

local tabcontroller = function(tabs)
	return {

		add_tab = function(child)
			iup.Append(tabs.handle,child.handle)
			local tc = tabs.count - 1
			local ttprop = "TABTITLE"..tc
			tabs["TABTITLE"..tc] = child.meta.tabname
			iup.Map(child.handle)
			iup.Refresh(tabs.handle)
		end,

		refresh_names = function()
			local child = iup.GetNextChild(tabs.handle,nil)
			local index = 0

			while (child ~= nil) do
				tabs["TABTITLE"..index] = child.meta.tabname
				index=index+1
				child = iup.GetNextChild(tabs.handle,child)
			end
		end,

		remove_tab = function(child)
			iup.Detach(child.handle,tabs.handle)
			iup.Refresh(tabs.handle)
		end,

		rename_tab = function(child,newname)
			local index = iup.GetChildPos(tabs.handle,child.handle)
			tabs["TABTITLE"..index] = newname
		end
	}
end

return function(options)
	options[1] = iup.vbox{}
	local ret  = iup.tabs(options)
	local interface = tabcontroller(ret)
	interface.remove_tab(options[1])
	return ret,interface
end