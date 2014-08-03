
require("cdlua")
require("iuplua")
require("iupluacd")
require("iupluacontrols")

local cm = require("widgets.cell_matrix")
local cell = cm.cell
local label_cell = cm.label_cell

local instr = function(name) return {name=name} end

local instrs = {
  instr('jim'),
  instr('bob'),
  instr('mogus')
}

local instrument_cell = function(instr_list)
  local model = cell(0,255,0)
  local current_instr = nil

  local set_instrument = function(instrument)
    if instrument == nil then
      current_instr = nil
    else
      current_instr = instrument
    end
  end
  

  model.value_cb = function() 
    if current_instr == nil then
      return '---'
    else
      return current_instr.name
    end
  end

  model.dropcheck_cb = function()
    return iup.DEFAULT
  end

  model.drop_cb = function(drop)
    drop[1] = '---'
    
    for i=1,#instr_list do
      drop[i+1] = instr_list[i].name
    end

    return iup.DEFAULT
  end

  model.dropselect_cb = function(text,index,onoff)
    if(onoff == 1) then

      local val;

      if(index == 1) then
        val = nil
      else
        val = instr_list[index-1]
      end

      set_instrument(val)
    end

    return iup.CONTINUE
  end

  model.newval_cb = function(val)
  end

  return model
end

local topLabels = {label_cell('number'),label_cell('instrument')}
local row = {label_cell(0),instrument_cell(instrs)}
local vm = {cols=2,cells={topLabels,row}}


local box = iup.vbox{
  cm.new(vm),
  padding="10x10",
  expand="NO"
}

local dlg = iup.dialog{box; title="DO DOPE FUCK HOPE"}

dlg:show()
iup.MainLoop()
