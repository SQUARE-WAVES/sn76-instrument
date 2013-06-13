#include <iostream>
#include <stdint.h>
#include <livelayer\livelayer.h>
#include <thread>

#include "sn76_core.h"
#include "sn76player.h"
#include "udp_tabler.h"
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
	serial_port port(&main_loop,"COM3");
	sn76core core(&port);

	std::cout<<"opeing the midi port\n\n";
	int inport = get_midi_port();
	sn76_player player(&main_loop,&core,inport);

	std::thread lthread(lua_wrapper::lua_thread,"../config.lua",&main_loop,&player);
	lthread.detach();

	std::cout << "\n\nStarting the Main Loop\n";
	main_loop.run();
	std::cout << "it's over\n";
	return 0;
}