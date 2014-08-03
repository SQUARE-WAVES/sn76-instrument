#include <iostream>
#include <stdint.h>
#include <livelayer\livelayer.h>
#include <thread>
#include <vector>

#include "sn76_core.h"
#include "sn76_player.h"
#include "lua_wrapper.h"

int get_midi_port()
{
	int ports = midi_port::count_midi_ports();
	for(int i=0;i<ports;++i)
	{
		std::cout << i <<" : "<< midi_port::port_name(i) <<"\n";
	}

	int inport;
	std::cin >> inport;

	return inport;
}

int main()
{
	udp_port::initialize();

	event_loop main_loop;

	std::thread lthread(lua_wrapper::lua_thread,"../neoconfig.lua",&main_loop);
	lthread.detach();

	std::cout << "\n\nStarting the Main Loop\n";
	main_loop.run();
	std::cout << "it's over\n";
	return 0;
}