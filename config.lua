widgets = dofile("../widgets.lua")
require("iuplua" )
require("iupluacontrols")
require('json')

print("opening interface")
print(sn76:open(0,"COM6"))

iup.SetGlobal('DLGBGCOLOR','0 0 0')
--[[
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
end]]--

local save_data_file = io.open('savedata.json','r')
local save_data = json.decode(save_data_file:read("*all"))

local sqr_heads = {"amp","note","freq"}
local noise_heads = {"amp","note","freq","noiseamp","wave"}

local chip1_channel1_mod = widgets.ptab(save_data.chip1_channel1,0,0)
local chip1_channel1_view = widgets.matrix(chip1_channel1_mod,"chip1 channel1",sqr_heads)

local chip1_channel2_mod = widgets.ptab(save_data.chip1_channel2,0,1)
local chip1_channel2_view = widgets.matrix(chip1_channel2_mod,"chip1 channel2",sqr_heads)

local chip1_noise_mod = widgets.ptab(save_data.chip1_noise,0,2)
local chip1_noise_view = widgets.matrix(chip1_noise_mod,"chip1 noise",noise_heads)

local chip2_channel1_mod = widgets.ptab(save_data.chip2_channel1,1,0)
local chip2_channel1_view = widgets.matrix(chip2_channel1_mod,"chip2 channel1",sqr_heads)

local chip2_channel2_mod = widgets.ptab(save_data.chip2_channel2,1,1)
local chip2_channel2_view = widgets.matrix(chip2_channel2_mod,"chip2 channel2",sqr_heads)

local chip2_noise_mod = widgets.ptab(save_data.chip2_noise,1,2)
local chip2_noise_view = widgets.matrix(chip2_noise_mod,"chip2 noise",noise_heads)

local tabs = iup.tabs(
{
	chip1_channel1_view.box,
	chip1_channel2_view.box,
	chip1_noise_view.box,
	chip2_channel1_view.box,
	chip2_channel2_view.box,
	chip2_noise_view.box
})

local save_button = iup.button(
{
	title="save"
})

save_button.action = function(self)
	print("gonna save stuff");

	local savedata = json.encode({
		chip1_channel1 = chip1_channel1_mod.vals,
		chip1_channel2 = chip1_channel2_mod.vals,
		chip1_noise = chip1_noise_mod.vals,
		
		chip2_channel1 = chip2_channel1_mod.vals,
		chip2_channel2 = chip2_channel2_mod.vals,
		chip2_noise = chip2_noise_mod.vals
	})
	
	local save_file = io.open('savedata.json','w')
	save_file:write(savedata)
	save_file:close()
	
	print('shit done saved')
end

local layout = iup.vbox({tabs,save_button})

tabs.bgcolor = "0 0 0"

dg = iup.dialog{layout, size="960x420",bg_color="0 0 0"}
dg:show()

if (iup.MainLoopLevel()==0) then
  iup.MainLoop()
end