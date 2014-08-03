require("iuplua")
require("iupluacontrols")

local instruments = require("instruments")
local isntrument_editor = require("widgets.instrument_editor")
local patch_editor = require("widgets.patch_editor")
local main_tabs = require("widgets.main_tabs")
local make_menu = require('widgets.menu')

local player,chip1_channel1, chip1_channel2, chip1_nz, chip2_channel1, chip2_channel2, chip2_nz = open_player("COM3")

local worked = player:open_midi_port(0)

if true ~= worked then print("couldn't open midi") end

local tabs,tabiface = main_tabs{nil,margin="5x5"}

player:set_voicer(chip1_channel1,0);
player:set_voicer(chip1_channel2,1);
player:set_noise_voicer(chip1_nz,2);

player:set_voicer(chip2_channel1,3);
player:set_voicer(chip2_channel2,4);
player:set_noise_voicer(chip2_nz,5);

local chip1 = patch_editor.chip_editor("chip1",chip1_channel1,chip1_channel2,chip1_nz,instruments.square,instruments.square,instruments.noise,tabiface)
local chip2 = patch_editor.chip_editor("chip2",chip2_channel1,chip2_channel2,chip2_nz,instruments.square,instruments.square,instruments.noise,tabiface)

tabiface.add_tab(chip1)
tabiface.add_tab(chip2)

local dlg = iup.dialog{
	tabs,
	menu=make_menu(tabiface,instruments),
	size="1005x420",
	gap="5"
}

dlg:show()
iup.MainLoop()