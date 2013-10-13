widgets = dofile("../widgets.lua")
require("iuplua" )
require("iupluacontrols")

iup.SetGlobal('DLGBGCOLOR','0 0 0')

local sqr_proto_row = function()
	return {amp={val=15,jmp=-2}, note={val=0,jmp=-2}, freq={val=0,jmp=-2}}
end

local noise_proto_row = function()
	return {amp={val=15,jmp=-2}, note={val=0,jmp=-2}, freq={val=0,jmp=-2},noiseamp={val=0,jmp=-2},wave={val=0,jmp=-2}}
end

function rep_row(proto_maker,times)
	local tab = {}
	for i=1,times do
		table.insert(tab,proto_maker())
	end
	
	return tab
end

local sqr_heads = {"amp","note","freq"}
local noise_heads = {"amp","note","freq","noiseamp","wave"}

local chip1_channel1_mod = widgets.ptab(rep_row(sqr_proto_row,16),0,0)
local chip1_channel1_view = widgets.matrix(chip1_channel1_mod,"chip1 channel1",sqr_heads)

local chip1_channel2_mod = widgets.ptab(rep_row(sqr_proto_row,16),0,1)
local chip1_channel2_view = widgets.matrix(chip1_channel2_mod,"chip1 channel2",sqr_heads)

local chip1_noise_mod = widgets.ptab(rep_row(noise_proto_row,16),0,2)
local chip1_noise_view = widgets.matrix(chip1_noise_mod,"chip1 noise",noise_heads)

local tabs = iup.tabs({chip1_channel1_view.box,chip1_channel2_view.box,chip1_noise_view.box})
tabs.bgcolor = "0 0 0"

dg = iup.dialog{tabs, size="960x420",bg_color="0 0 0"}
dg:show()

if (iup.MainLoopLevel()==0) then
  iup.MainLoop()
end