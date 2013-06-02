#include <iostream>
#include <stdint.h>
#include <livelayer\livelayer.h>
#include <thread>
#include "sn76_core.h"
#include "sn76player.h"

void core_test(sn76core& core)
{
	using std::cout;
	using std::cin;

	cout << "testing chip 1, channel 1\n";
	cin.get();
	core.set_freq(0,0,0x1d,0x0e);
	core.set_amp(0,0,0x00);

	cout << "testing chip 1, channel 2\n";
	cin.get();
	core.set_amp(0,0,0x0F);
	core.set_freq(0,1,0x1a,0x0a);
	core.set_amp(0,1,0x00);

	cout << "testing chip 1, channel 3\n";
	cin.get();
	core.set_amp(0,1,0x0F);
	core.set_freq(0,2,0x17,0x0B);
	core.set_amp(0,2,0x00);

	cout << "testing chip 1, channel noise\n";
	cin.get();
	core.set_amp(0,2,0x0F);
	core.set_noise(0,0x04);
	core.set_amp(0,3,0x00);
	cin.get();

	core.set_amp(0,3,0x0F);

	cout << "test over\n";

}

void voicer_test(sn76core& core)
{
	using std::cout;
	using std::cin;
	sn76_voicer voicer(&core,1,1);

	cout<<"makin a voicer for chip 2 channel 2\n";
	cout<<"press <enter> a few times for a major chord!\n";
	cin.get();
	voicer.note_on(60,127);
	cin.get();
	voicer.note_on(64,127);
	cin.get();
	voicer.note_on(67,127);
	cin.get();
	voicer.note_off(67,127);
	cin.get();
	voicer.note_off(64,127);
	cin.get();
	voicer.note_off(60,127);
	cout<<"it's done!\n";


}

int main()
{
	event_loop loop;
	serial_port port(&loop,"COM3");
	sn76core core(&port);

	int ports = midi_port::count_midi_ports();
	for(int i=0;i<ports;++i)
	{
		std::cout << i <<" : "<< midi_port::port_name(i) <<"\n";
	}

	int inport;
	std::cin >> inport;

	sn76_player player(&loop,&core,inport);

	loop.run();
	std::cin.get();
	return 0;
}