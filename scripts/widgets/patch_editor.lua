require("cdlua")
require("iuplua")
require("iupluacd")
require("iupluacontrols")

local instrument_editor = require("widgets.instrument_editor")
local cm = require("widgets.cell_matrix")
local cell = cm.cell
local label_cell = cm.label_cell

local instrument_cell = function(instr_list,set_instrument)
  local model = cell(0,255,0)
  local current_instrument = nil

  local setter = function(instrument)
  	current_instrument = instrument
  	set_instrument(instrument)
	end
  
  model.value_cb = function()     
    if current_instrument == nil then
    	return '---'
    else
    	return current_instrument.name
    end
  end

  model.dropcheck_cb = function()
    return iup.DEFAULT
  end

  model.drop_cb = function(drop)
    drop[1] = '---'
    
    for i=1,instr_list:len() do
      drop[i+1] = instr_list:at(i).name
    end

    return iup.DEFAULT
  end

  model.dropselect_cb = function(text,index,onoff)

    if(onoff == 1) then
      local val;

      if(index == 1) then
        val = nil
      else
        val = instr_list:at(index-1)
      end

      setter(val)
    end

    return iup.CONTINUE
  end

  model.newval_cb = function(val)
  end

  return model
end

local viewmodel = function(voicer,instr_list)
	
	local cell_setter = function(i,voicer)
		
		return function(instrument)
			if instrument ~= nil then
				voicer:set_patch(instrument.handle,i)
			else
				voicer:unset_patch(i)
			end
		end
	end

	local vm = {cols=2,rows_visible=16};
	
	vm.cells = {
		{label_cell('patch number'),label_cell('instrument')}
	}

	for i=1,128 do
		local patchrow = {label_cell(i-1),instrument_cell(instr_list,cell_setter(i-1,voicer))}
		table.insert(vm.cells,patchrow)
	end

	return vm
end

local patch_editor = function(name,voicer,instr_list)
	
	local mat = cm.new(viewmodel(voicer,instr_list))

	return iup.frame{
    mat,
    title=name,
    expand="vertical"
  }

end

local chip_editor = function(name,v1,v2,v3,i1,i2,i3,tabs)

  local new_instrname = iup.text{alignment="CENTER",expand="VERTICAL",size="100x",fontsize="18"}
  local new_instr_button = iup.button{
    expand="VERTICAL",
    title="new instrument",
    
    action = function()
      local text = new_instrname.value
  
      if text ~= nil and text ~= "" then
        local i = i1:new(text,1)
        local ed = instrument_editor(i,tabs)
        tabs.add_tab(ed)
        new_instrname.value = nil
      end
    end
  }

  local new_nzname = iup.text{alignment="CENTER",expand="VERTICAL",size="100x",fontsize="18"}
  local new_nz_button = iup.button{
    expand="VERTICAL",
    title="new noise instrument",
    
    action = function()
      local text = new_nzname.value
  
      if text ~= nil and text ~= "" then
        local i = i3:new(text,1)
        local ed = instrument_editor(i,tabs)
        tabs.add_tab(ed)
        new_nzname.value = nil
      end
    end
  }

  local new_instr_creator = iup.hbox{
    new_instrname,
    new_instr_button,
    padding="5x5"
  }

  local new_nz_creator = iup.hbox{
    new_nzname,
    new_nz_button,
    padding="5x5"
  }


  return iup.hbox{
    patch_editor("channel 1",v1,i1,tabs),
    patch_editor("channel 2",v2,i2,tabs),
    patch_editor("channel 3",v3,i3,tabs),
    iup.vbox{
      new_instr_creator,
      new_nz_creator,
      expand="no"
    },

    padding="10x10",
    meta={
      tabname=name
    }
  }
end
 
return {
  patch_editor = patch_editor,
  chip_editor = chip_editor
}
